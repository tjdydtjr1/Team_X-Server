#pragma once


/*=====================================
			    Dump    
=====================================*/

#include <DbgHelp.h>
#include <string>
#include "../macro/Define.h"

/**
* Dump File »ý¼º
*/

using WRITEDUMP = BOOL(WINAPI*)( HANDLE hProcess, DWORD ProcessId, HANDLE hFile, MINIDUMP_TYPE DumpType,
                                PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam, 
								PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
								PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

class Dump
{
    SINGLETON_BASE(Dump);

public:
    void Init();

private:
    static LONG WINAPI ExceptionFilter(_EXCEPTION_POINTERS* exceptionInfo);
    static void CreateDumpFile(const std::wstring& dumpPath, _EXCEPTION_POINTERS* exceptionInfo);
};

#define DumpManager Dump::GetInstance()