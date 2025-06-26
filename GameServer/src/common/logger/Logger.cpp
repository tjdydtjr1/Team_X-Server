#include "pch.h"
#include "Logger.h"
#include <iostream>
#include <iomanip>
#include <sstream>

thread_local std::string LoggerManager::LFormatBuffer;

LoggerManager& LoggerManager::GetInstance() 
{
    static LoggerManager instance;

    return instance;
}

LoggerManager::~LoggerManager() 
{
    Shutdown();
}

void LoggerManager::Init(size_t queueSize, size_t threadCount, size_t bufferSize) 
{
    if (_initialized.exchange(true, std::memory_order_acq_rel)) 
    {
        return;
    }

    _maxQueueSize = queueSize;
    _bufferSize = bufferSize;


    // Release 모드에서만 파일 초기화
    if constexpr (!_isDebugMode) 
    {
        SetLogLevel(LogLevel::WARN);

        // 로그 디렉토리 생성
        EnsureLogDirectory();
        CheckAndRotateLogFile();
    }

    _workerThreads.reserve(threadCount);

    for (size_t i = 0; i < threadCount; ++i) 
    {
        _workerThreads.emplace_back(&LoggerManager::WorkerThread, this);
    }

    Log(LogLevel::INFO, std::format("LoggerManager Init Thread: {} , Queue size: {}", 
        threadCount, queueSize), std::source_location::current());
}

void LoggerManager::Trace(std::string_view message, std::source_location loc)
{
    if (LogLevel::TRACE >= _currentLevel.load(std::memory_order_relaxed)) [[likely]]
    {
        Log(LogLevel::TRACE, std::string{message}, loc);
    }
}

void LoggerManager::Debug(std::string_view message, std::source_location loc)
{
    if (LogLevel::DEBUG >= _currentLevel.load(std::memory_order_relaxed)) [[likely]]
    {
        Log(LogLevel::DEBUG, std::string{message}, loc);
    }
}

void LoggerManager::Info(std::string_view message, std::source_location loc)
{
    if (LogLevel::INFO >= _currentLevel.load(std::memory_order_relaxed)) [[likely]]
    {
        Log(LogLevel::INFO, std::string{message}, loc);
    }
}

void LoggerManager::Warn(std::string_view message, std::source_location loc)
{
    if (LogLevel::WARN >= _currentLevel.load(std::memory_order_relaxed)) [[likely]]
    {
        Log(LogLevel::WARN, std::string{message}, loc);
    }
}

void LoggerManager::Error(std::string_view message, std::source_location loc)
{
    if (LogLevel::ERR >= _currentLevel.load(std::memory_order_relaxed)) [[unlikely]]
    {
        Log(LogLevel::ERR, std::string{message}, loc);
    }
}

void LoggerManager::Critical(std::string_view message, std::source_location loc)
{
    if (LogLevel::CRITICAL >= _currentLevel.load(std::memory_order_relaxed)) [[unlikely]]
    {
        Log(LogLevel::CRITICAL, std::string{message}, loc);
    }
}

void LoggerManager::Shutdown() 
{
    if (!_initialized.load(std::memory_order_acquire) || 
        _shutdown.exchange(true, std::memory_order_acq_rel)) 
    {
        return;
    }

    _queueCv.notify_all();

    _workerThreads.clear();

    if (_logFile && _logFile->is_open()) 
    {
        _logFile->flush();
        _logFile->close();
    }
}

void LoggerManager::Log(LogLevel level, std::string&& message, std::source_location loc) 
{
    if (!_initialized.load(std::memory_order_acquire) || 
        _shutdown.load(std::memory_order_acquire)) [[unlikely]]
    {
        return;
    }

    if (level < _currentLevel.load(std::memory_order_relaxed)) [[unlikely]]
    {
        return;
    }

    LogMessage logMsg{level, std::move(message), loc};

    {
        std::lock_guard<std::mutex> lock{_queueMutex};
        if (_messageQueue.size() >= _maxQueueSize) [[unlikely]]
        {
            // 큐가 가득 찬 경우, 가장 오래된 메시지 제거
            _messageQueue.pop();
        }
        _messageQueue.emplace(std::move(logMsg));
    }

    _queueCv.notify_one();
}

void LoggerManager::WorkerThread() 
{
    // 버퍼 예약
    LFormatBuffer.reserve(_bufferSize);

    while (!_shutdown.load(std::memory_order_acquire)) 
    {
        std::unique_lock<std::mutex> lock{_queueMutex};

        _queueCv.wait(lock, [this] 
        {
            return !_messageQueue.empty() || _shutdown.load(std::memory_order_acquire);
        });

        // 배치 처리를 위한 로컬 큐
        std::queue<LogMessage> localQueue;
        std::swap(localQueue, _messageQueue);
        lock.unlock();

        while (!localQueue.empty()) 
        {
            ProcessMessage(localQueue.front());
            localQueue.pop();
        }
    }
}

void LoggerManager::ProcessMessage(const LogMessage& msg) 
{
    if constexpr (_isDebugMode) 
    {
        WriteToConsole(msg);
    } 
    else 
    {
        WriteToFile(msg);
    }
}

void LoggerManager::WriteToConsole(const LogMessage& msg) 
{
    FormatString(msg, LFormatBuffer);
    ConsoleColor color{ GetConsoleColor(msg.level) };

    HANDLE console{ GetStdHandle(STD_OUTPUT_HANDLE) };
    SetConsoleTextAttribute(console, static_cast<WORD>(color));
    std::cout << LFormatBuffer << '\n';
    SetConsoleTextAttribute(console, static_cast<WORD>(ConsoleColor::DEFAULT));

    std::cout.flush();
}

void LoggerManager::WriteToFile(const LogMessage& msg) 
{
    std::lock_guard<std::mutex> lock{_fileMutex};

    // 날짜가 바뀌었으면 새 파일로 로테이션
    CheckAndRotateLogFile();

    if (_logFile && _logFile->is_open()) [[likely]]
    {
        FormatString(msg, LFormatBuffer);
        _logFile->write(LFormatBuffer.data(), LFormatBuffer.size());
        _logFile->put('\n');

        // ERROR 또는 CRITICAL 레벨일 때만 즉시 디스크에 쓰기
        if (msg.level >= LogLevel::ERR) [[unlikely]]
        {
            _logFile->flush();
        }
    }
}

void LoggerManager::FormatString(const LogMessage& msg, std::string& buffer) 
{
    std::ostringstream oss;
    oss << std::hex << msg.threadId;
    std::string threadId{ oss.str() };

    buffer = std::format("[{}] [0x{}] [{}] [{}] [{}:{}] {}",
        TimeUtils::GetTime(),           // 시간 (YYYY-MM-DD hh:mm:ss)
        threadId,                       // 쓰레드 ID (0x1234abcd)
        GetLogLevelString(msg.level),   // 로그 레벨
        msg.funcName,                   // 호출 함수명 
        msg.filename,                   // 파일 이름 
        msg.line,                       // 라인 번호
        msg.message                     // 메시지
    );
}

constexpr std::string_view LoggerManager::GetLogLevelString(LogLevel level) noexcept
{
    return LOG_LEVEL_STRINGS[static_cast<int>(level)];
}

constexpr ConsoleColor LoggerManager::GetConsoleColor(LogLevel level) noexcept
{
    switch (level) 
    {
        case LogLevel::TRACE: return ConsoleColor::GRAY;
        case LogLevel::DEBUG: return ConsoleColor::BLUE;
        case LogLevel::INFO: return ConsoleColor::WHITE;
        case LogLevel::WARN: return ConsoleColor::YELLOW;
        case LogLevel::ERR: return ConsoleColor::RED;
        case LogLevel::CRITICAL: return ConsoleColor::BRIGHT_RED;
        default: return ConsoleColor::DEFAULT;
    }
}

std::string LoggerManager::GetLogFileName() 
{
    std::string date{ TimeUtils::GetStartOfDate() };

    return std::format("log/{}/{}_log.txt", date, date);
}

void LoggerManager::EnsureLogDirectory() 
{
    std::string date{ TimeUtils::GetStartOfDate() };
    std::string logDir{ std::format("log/{}", date) };

    std::error_code ec;
    std::filesystem::create_directories(logDir, ec);

    if (ec) [[unlikely]]
    {
        std::cerr << "Create Error: " << ec.message() << '\n';
    }
}

void LoggerManager::CheckAndRotateLogFile()
{
    std::string currentDate{ TimeUtils::GetStartOfDate() };

    if (currentDate != _currentDate || !_logFile || !_logFile->is_open()) [[unlikely]]
    {
        // 기존 파일 닫기
        if (_logFile && _logFile->is_open()) 
        {
            _logFile->close();
        }

        // 새 디렉토리 생성
        EnsureLogDirectory();

        // 새 파일 경로 설정
        _currentDate = currentDate;
        _currentLogFilePath = GetLogFileName();

        // 새 파일 열기
        _logFile = std::make_unique<std::ofstream>(
            _currentLogFilePath, 
            std::ios::out | std::ios::app
        );

        if (!_logFile->is_open()) [[unlikely]]
        {
            std::cerr << "FIle Open Error: " << _currentLogFilePath << '\n';
        }
    }
}