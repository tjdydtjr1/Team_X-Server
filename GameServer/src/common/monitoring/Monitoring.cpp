#include "pch.h"
#include "Monitoring.h"
#include <psapi.h>

void Monitoring::Init()
{
    // 현재 프로세스 핸들 가져오기
    _self = GetCurrentProcess();

    // 시스템 정보 가져오기
    SYSTEM_INFO sysInfo{};
    GetSystemInfo(&sysInfo);
    _processors = sysInfo.dwNumberOfProcessors;

    // 파일 시간을 초기화
    FILETIME ftime, fsys, fuser;
    GetSystemTimeAsFileTime(&ftime);
    GetProcessTimes(_self, &ftime, &ftime, &fsys, &fuser);

    // 마지막 CPU 시간을 기록
    memcpy(&_lastCPU, &ftime, sizeof(FILETIME));
    memcpy(&_lastSysCPU, &fsys, sizeof(FILETIME));
    memcpy(&_lastUserCPU, &fuser, sizeof(FILETIME));
}

double Monitoring::ProcessCPUUsage()
{
    // 현재 시간 가져오기
    FILETIME ftime, fsys, fuser;
    ULARGE_INTEGER now, sys, user;

    GetSystemTimeAsFileTime(&ftime);
    memcpy(&now, &ftime, sizeof(FILETIME));

    // 현재 프로세스의 시간 정보 가져오기
    GetProcessTimes(_self, &ftime, &ftime, &fsys, &fuser);
    memcpy(&sys, &fsys, sizeof(FILETIME));
    memcpy(&user, &fuser, sizeof(FILETIME));

    // 시스템 시간과 사용자 시간을 계산
    ULONGLONG sysTime = sys.QuadPart - _lastSysCPU.QuadPart;
    ULONGLONG userTime = user.QuadPart - _lastUserCPU.QuadPart;
    ULONGLONG totalTime = sysTime + userTime;

    ULONGLONG elapsed = now.QuadPart - _lastCPU.QuadPart;

    double percent = 0.0;

    if (elapsed > 0)
    {
        // CPU 사용량 계산
        percent = (totalTime * 100.0) / elapsed;
        percent /= _processors;  // 멀티 코어 시스템에서 각 코어별 사용량을 나누어 줌
    }

    // 마지막 CPU 시간 업데이트
    _lastCPU = now;
    _lastSysCPU = sys;
    _lastUserCPU = user;

    return percent;
}

size_t Monitoring::ProcessMemUsage() const
{
    // 프로세스 메모리 정보 가져오기
    PROCESS_MEMORY_COUNTERS pmc{};
    GetProcessMemoryInfo(_self, &pmc, sizeof(pmc));

    // WorkingSetSize는 프로세스가 사용하는 실제 메모리 크기 (페이지 파일 제외)
    return static_cast<size_t>(pmc.WorkingSetSize);
}

size_t Monitoring::PhysicMemUsage() const
{
    // 시스템 메모리 상태 가져오기
    MEMORYSTATUSEX memInfo{};
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);

    // 사용 중인 물리 메모리 크기
    return static_cast<size_t>(memInfo.ullTotalPhys - memInfo.ullAvailPhys);
}