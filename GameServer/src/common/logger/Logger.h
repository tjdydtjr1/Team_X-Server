#pragma once

#include <string>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <memory>
#include <fstream>
#include <chrono>
#include <format>
#include <source_location>
#include <filesystem>
#include <string_view>

enum class LogLevel : uint8
{
    TRACE = 0,
    DEBUG = 1,
    INFO = 2,
    WARN = 3,
    ERR = 4,
    CRITICAL = 5,
    LOG_ALL
};

enum class ConsoleColor : uint8 
{
    GRAY = 8,
    BLUE = 9,
    WHITE = 15,
    YELLOW = 14,
    RED = 12,
    BRIGHT_RED = 79,
    DEFAULT = 7
};

static string SimpleFuncName(const std::string& funcName)
{
    auto end{ funcName.find('(') };
    if (end == std::string::npos)
    {
        end = funcName.length();
    }

    auto start{ funcName.rfind("::", end) };

    if (start != std::string::npos) 
    {
        auto typeEnd{ funcName.rfind(' ', start) };
        size_t classFuncStart{ ( typeEnd == std::string::npos ) ? 0 : typeEnd + 1 };
        
        return funcName.substr(classFuncStart, end - classFuncStart);
    }

    auto space{ funcName.rfind(' ', end) };

    if (space != std::string::npos)
    {
        return funcName.substr(space + 1, end - (space + 1));
    }

    return funcName.substr(0, end);
}

struct LogMessage 
{
    LogLevel level{};
    std::string message;
    std::chrono::system_clock::time_point timestamp;
    std::string filename; 
    std::string funcName;
    uint32_t line{};
    std::thread::id threadId{};

    LogMessage(LogLevel lvl, std::string msg, std::source_location loc = std::source_location::current())
        : level{lvl}, message{std::move(msg)}, timestamp{std::chrono::system_clock::now()}
        , filename{ std::filesystem::path(loc.file_name()).filename().string() }, line{ loc.line() }
        , threadId{ std::this_thread::get_id() }
        , funcName{ SimpleFuncName(loc.function_name()) } 
    {
    }
};

class LoggerManager 
{
public:
    static LoggerManager& GetInstance();

    void Init(size_t queueSize = 4096, size_t threadCount = 1, size_t bufferSize = 8192);

    void Trace(std::string_view message, std::source_location loc = std::source_location::current());
    void Debug(std::string_view message, std::source_location loc = std::source_location::current());
    void Info(std::string_view message, std::source_location loc = std::source_location::current());
    void Warn(std::string_view message, std::source_location loc = std::source_location::current());
    void Error(std::string_view message, std::source_location loc = std::source_location::current());
    void Critical(std::string_view message, std::source_location loc = std::source_location::current());

    void Shutdown();

public:
    void SetLogLevel(const LogLevel level) noexcept { _currentLevel.store(level, std::memory_order_relaxed); }
    LogLevel GetLogLevel() const noexcept { return _currentLevel.load(std::memory_order_relaxed); }

private:
    LoggerManager() = default;
    ~LoggerManager();
    LoggerManager(const LoggerManager&) = delete;
    LoggerManager& operator=(const LoggerManager&) = delete;
    LoggerManager(LoggerManager&&) = delete;
    LoggerManager& operator=(LoggerManager&&) = delete;

private:
    void Log(LogLevel level, std::string&& message, std::source_location loc);
    void WorkerThread();
    void ProcessMessage(const LogMessage& msg);
    void WriteToConsole(const LogMessage& msg);
    void WriteToFile(const LogMessage& msg);
    void FormatString(const LogMessage& msg, std::string& buffer);
    static constexpr std::string_view GetLogLevelString(LogLevel level) noexcept;
    static constexpr ConsoleColor GetConsoleColor(LogLevel level) noexcept;
    std::string GetLogFileName();
    void EnsureLogDirectory();
    void CheckAndRotateLogFile();

    static constexpr std::string_view LOG_LEVEL_STRINGS[] { "TRACE", "DEBUG", "INFO ", "WARN ", "ERROR", "CRITICAL "};

    alignas( 64 ) std::atomic<bool> _initialized{ false };
    alignas( 64 ) std::atomic<bool> _shutdown{ false };
    alignas( 64 ) std::atomic<LogLevel> _currentLevel{ LogLevel::TRACE };

    std::queue<LogMessage> _messageQueue;
    mutable std::mutex _queueMutex;
    std::condition_variable _queueCv;

    std::vector<std::jthread> _workerThreads;
    size_t _maxQueueSize{};
    size_t _bufferSize{};

    std::unique_ptr<std::ofstream> _logFile;
    std::string _currentLogFilePath;
    std::string _currentDate;
    mutable std::mutex _fileMutex;
    thread_local static std::string LFormatBuffer;

#ifdef _DEBUG
    static constexpr bool _isDebugMode{ true };
#else
    static constexpr bool _isDebugMode{ false };
#endif
};

#define LogManager LoggerManager::GetInstance()
#define LOGGER_INIT(QUEUE_SIZE, THREAD_SIZE, QUEUE_BUFFER_SIZE) LogManager.Init(QUEUE_SIZE, THREAD_SIZE, QUEUE_BUFFER_SIZE)

#define LOG_FORMAT(LEVEL, ...) LoggerManager::GetInstance().LEVEL( std::format(__VA_ARGS__) )
#define LOG_TRACE(...)    LOG_FORMAT(Trace, __VA_ARGS__)
#ifdef _DEBUG
#define LOG_DEBUG(...)    LOG_FORMAT(Debug, __VA_ARGS__)
#else
#define LOG_DEBUG(...)    ((void)0)
#endif
#define LOG_INFO(...)     LOG_FORMAT(Info, __VA_ARGS__)
#define LOG_WARN(...)     LOG_FORMAT(Warn, __VA_ARGS__)
#define LOG_ERROR(...)    LOG_FORMAT(Error, __VA_ARGS__)
#define LOG_CRITICAL(...) LOG_FORMAT(Critical, __VA_ARGS__)
