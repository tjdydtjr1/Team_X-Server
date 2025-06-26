#include "pch.h"
#include "Dump.h"
#include <filesystem>
#include <sstream>

void Dump::Init()
{
    ::SetUnhandledExceptionFilter(ExceptionFilter); 
}

LONG WINAPI Dump::ExceptionFilter(_EXCEPTION_POINTERS* exceptionInfo)
{
    std::wcerr << L"Creating dump file\n";

    // 덤프 파일 저장 경로 생성
    std::wstring dumpDir{ L"./dump/" };
    std::filesystem::create_directories(dumpDir);

    std::wstring now{ TimeUtils::GetTimeW(true, FormatStyle::FILE_FORMAT) };

    std::wstring dumpPath{ dumpDir + L"Dump_" + now + L".dmp" };

    // 덤프 파일 생성
    CreateDumpFile(dumpPath, exceptionInfo);

    return EXCEPTION_EXECUTE_HANDLER;
}

void Dump::CreateDumpFile(const std::wstring& dumpPath, _EXCEPTION_POINTERS* exceptionInfo)
{
    HMODULE dumpDll{ ::LoadLibrary(L"DbgHelp.dll") };

    if (!dumpDll)
    {
        std::wcerr << L"Failed to load DbgHelp.dll. Error: " << ::GetLastError() << std::endl;

        return;
    }

    WRITEDUMP dumpFunc{ ( WRITEDUMP )::GetProcAddress(dumpDll, "MiniDumpWriteDump") };

    if (!dumpFunc)
    {
        std::wcerr << L"Failed to find MiniDumpWriteDump function. Error: " << ::GetLastError() << std::endl;
        ::FreeLibrary(dumpDll);

        return;
    }

    HANDLE file{ ::CreateFile(dumpPath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL) };

    if (file == INVALID_HANDLE_VALUE)
    {
        std::wcerr << L"Failed to create dump file. Error: " << ::GetLastError() << std::endl;
        ::FreeLibrary(dumpDll);

        return;
    }

    _MINIDUMP_EXCEPTION_INFORMATION dumpInfo{};
    dumpInfo.ThreadId = ::GetCurrentThreadId();
    dumpInfo.ExceptionPointers = exceptionInfo;
    dumpInfo.ClientPointers = FALSE;

    if (!dumpFunc(::GetCurrentProcess(), ::GetCurrentProcessId(), file, MiniDumpNormal, &dumpInfo, NULL, NULL))
    {
        std::wcerr << L"Failed to write dump file. Error: " << ::GetLastError() << std::endl;
    }
    else
    {
        std::wcerr << L"Dump file create: " << dumpPath << std::endl;
    }

    ::CloseHandle(file);
    ::FreeLibrary(dumpDll);
}
