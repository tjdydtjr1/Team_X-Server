#pragma once
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>
#include <utility>

/*============================
        TimeUtils
============================*/

/*시간 단위*/
enum class TimeUnit : uint8_t
{
    SECOND,
    MILLI_SECOND,
    MICRO_SECOND,
    NANO_SECOND
};

/*포맷 방식*/
enum class FormatStyle : uint8_t
{
    BASE,           // %Y-%m-%d %H:%M:%S
    FILE_FORMAT     // %Y%m%d_%H%M%S
};

class TimeUtils
{
public:
    /**
    * @brief : 현재 시간을 Local / UTC로 가져오는 함수
    * @param : isLocal = true -> Local, false -> UTC
    * @param : style = 시간 형식 enum FormatStyle
    * @return : 현재 시간 형식
    */
    static std::wstring GetTimeW(const bool isLocal = true, const FormatStyle style = FormatStyle::BASE) { return FormatTimeW(std::chrono::system_clock::now(), isLocal, style); }
    static std::string GetTime(const bool isLocal = true, const FormatStyle style = FormatStyle::BASE) { return FormatTime(std::chrono::system_clock::now(), isLocal, style); }

    /**
    * @brief : 현재 월의 시작과 끝을 UTC로 가져오는 함수
    * @return : { 시작일, 종료일 }
    */
    static std::pair<std::wstring, std::wstring> GetUTCMonthRangeW();
    static std::pair<std::string, std::string> GetUTCMonthRange();

    /**
    * @brief : 현재 월의 시작과 끝을 Local로 가져오는 함수
    * @return : { 시작일, 종료일 }
    */
    static std::pair<std::wstring, std::wstring> GetLocalMonthRangeW();
    static std::pair<std::string, std::string> GetLocalMonthRange();

    /**
    * @brief : 두 날짜의 시간 차이를 가져오는 함수
    * @param : startDateTime = 시작 날짜
    * @param : endDateTime   = 종료 날짜
    * @param : time = 시간 단위 enum TimeUnit Default = TimeUnit::SECOND
    * @return : 두 시간의 차이
    */
    static long long GetTimeDifferenceW(const std::wstring& startDateTime, const std::wstring& endDateTime, const TimeUnit time = TimeUnit::SECOND);
    static long long GetTimeDifference(const std::string& startDateTime, const std::string& endDateTime, const TimeUnit time = TimeUnit::SECOND);

    /**
    * @brief : 현재 월의 시작 날을 UTC로 가져오는 함수
    * @param : isLocal = true -> Local, false -> UTC
    * @return : 현재 월의 시작 날
    */
    static std::wstring GetStartOfDayW(const bool isLocal = true, FormatStyle style = FormatStyle::BASE);
    static std::string GetStartOfDay(const bool isLocal = true, FormatStyle style = FormatStyle::BASE);

    /**
    * @brief : 현재 월의 시작 날의 월일을 UTC로 가져오는 함수
    * @param : isLocal = true -> Local, false -> UTC
    * @return : 현재 월의 시작 날의 월일
    */
    static std::wstring GetStartOfDateW(const bool isLocal = true);
    static std::string GetStartOfDate(const bool isLocal = true);

private:
    static std::chrono::system_clock::time_point ParseTimeW(const std::wstring& timeStr);
    static std::chrono::system_clock::time_point ParseTime(const std::string& timeStr);
    static std::wstring FormatTimeW(std::chrono::system_clock::time_point timePoint, const bool isLocal = true, const FormatStyle style = FormatStyle::BASE);
    static std::string  FormatTime(std::chrono::system_clock::time_point timePoint, bool isLocal = true, FormatStyle style = FormatStyle::BASE);
};