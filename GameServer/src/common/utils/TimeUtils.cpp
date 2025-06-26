#include "pch.h"
#include "TimeUtils.h"

std::pair<std::wstring, std::wstring> TimeUtils::GetUTCMonthRangeW()
{
    std::chrono::system_clock::time_point nowUtc{ std::chrono::system_clock::now() };
    std::time_t t{ std::chrono::system_clock::to_time_t(nowUtc) };
    std::tm tm{};

    ::gmtime_s(&tm, &t);

    tm.tm_mday = 1;
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;

    std::time_t startOfMonth{ std::mktime(&tm) };

    tm.tm_mon += 1;
    tm.tm_mday = 1;
    std::time_t firstOfNextMonth{ std::mktime(&tm) };
    std::time_t endOfMonth{ firstOfNextMonth - 1 };

    return { FormatTimeW(std::chrono::system_clock::from_time_t(startOfMonth), false), FormatTimeW(std::chrono::system_clock::from_time_t(endOfMonth), false) };
}

std::pair<std::string, std::string> TimeUtils::GetUTCMonthRange()
{
    std::chrono::system_clock::time_point nowUtc{ std::chrono::system_clock::now() };
    std::time_t t{ std::chrono::system_clock::to_time_t(nowUtc) };
    std::tm tm{};

    ::gmtime_s(&tm, &t);

    tm.tm_mday = 1;
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;

    std::time_t startOfMonth{ std::mktime(&tm) };

    tm.tm_mon += 1;
    tm.tm_mday = 1;
    std::time_t firstOfNextMonth{ std::mktime(&tm) };
    std::time_t endOfMonth{ firstOfNextMonth - 1 };

    return { FormatTime(std::chrono::system_clock::from_time_t(startOfMonth), false), FormatTime(std::chrono::system_clock::from_time_t(endOfMonth), false) };
}

std::pair<std::wstring, std::wstring> TimeUtils::GetLocalMonthRangeW()
{
    std::chrono::system_clock::time_point nowLocal{ std::chrono::system_clock::now() };
    std::time_t t{ std::chrono::system_clock::to_time_t(nowLocal) };

    std::tm tm{};

    ::localtime_s(&tm, &t);

    tm.tm_mday = 1;
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;

    std::time_t startOfMonth{ std::mktime(&tm) };

    tm.tm_mon += 1;
    std::time_t firstOfNextMonth{ std::mktime(&tm) };

    std::time_t endOfMonth{ firstOfNextMonth - 1 };

    return { FormatTimeW(std::chrono::system_clock::from_time_t(startOfMonth), true), FormatTimeW(std::chrono::system_clock::from_time_t(endOfMonth), true) };
}

std::pair<std::string, std::string> TimeUtils::GetLocalMonthRange()
{
    std::chrono::system_clock::time_point nowLocal{ std::chrono::system_clock::now() };
    std::time_t t{ std::chrono::system_clock::to_time_t(nowLocal) };

    std::tm tm{};

    ::localtime_s(&tm, &t);

    tm.tm_mday = 1;
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;

    std::time_t startOfMonth{ std::mktime(&tm) };

    tm.tm_mon += 1;
    std::time_t firstOfNextMonth{ std::mktime(&tm) };

    std::time_t endOfMonth{ firstOfNextMonth - 1 };

    return { FormatTime(std::chrono::system_clock::from_time_t(startOfMonth), true), FormatTime(std::chrono::system_clock::from_time_t(endOfMonth), true) };
}

std::chrono::system_clock::time_point TimeUtils::ParseTimeW(const std::wstring& timeStr)
{
    std::tm tm{};
    std::wstringstream wss(timeStr);

    wss >> std::get_time(&tm, L"%Y-%m-%d %H:%M:%S");

    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

std::chrono::system_clock::time_point TimeUtils::ParseTime(const std::string& timeStr)
{
    std::tm tm{};
    std::stringstream ss(timeStr);

    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");

    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

std::string TimeUtils::FormatTime(std::chrono::system_clock::time_point timePoint, bool isLocal, FormatStyle style)
{
    std::time_t time{ std::chrono::system_clock::to_time_t(timePoint) };
    std::tm tm{};

    if (isLocal)
    {
        ::localtime_s(&tm, &time);
    }
    else
    {
        ::gmtime_s(&tm, &time);
    }

    std::ostringstream oss;

    switch (style)
    {
        case FormatStyle::BASE:
        {
            oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        }
        break;
        case FormatStyle::FILE_FORMAT:
        {
            oss << std::put_time(&tm, "%Y%m%d_%H%M%S");
        }
        break;
    }

    return oss.str();
}

std::wstring TimeUtils::FormatTimeW(std::chrono::system_clock::time_point timePoint, const bool isLocal, const FormatStyle style)
{
    std::time_t time{ std::chrono::system_clock::to_time_t(timePoint) };
    std::tm tm{};

    if (isLocal)
    {
        ::localtime_s(&tm, &time);
    }
    else
    {
        ::gmtime_s(&tm, &time);
    }

    std::wostringstream woss;

    switch (style)
    {
        case FormatStyle::BASE:
        {
            woss << std::put_time(&tm, L"%Y-%m-%d %H:%M:%S");
        }
        break;
        case FormatStyle::FILE_FORMAT:
        {
            woss << std::put_time(&tm, L"%Y%m%d_%H%M%S");
        }
        break;
    }

    return woss.str();
}

long long TimeUtils::GetTimeDifferenceW(const std::wstring& startDateTime, const std::wstring& endDateTime, const TimeUnit timeUnit)
{
    const auto startPoint{ TimeUtils::ParseTimeW(startDateTime) };
    const auto endPoint{ TimeUtils::ParseTimeW(endDateTime) };

    switch (timeUnit)
    {
        case TimeUnit::SECOND:
        {
            return std::chrono::duration_cast<std::chrono::seconds>(endPoint - startPoint).count();
        }
        case TimeUnit::MILLI_SECOND:
        {
            return std::chrono::duration_cast<std::chrono::milliseconds>(endPoint - startPoint).count();
        }
        case TimeUnit::MICRO_SECOND:
        {
            return std::chrono::duration_cast<std::chrono::microseconds>(endPoint - startPoint).count();
        }
        case TimeUnit::NANO_SECOND:
        {
            return std::chrono::duration_cast<std::chrono::nanoseconds>(endPoint - startPoint).count();
        }
        default:
        {
            return 0;
        }
    }
}

long long TimeUtils::GetTimeDifference(const std::string& startDateTime, const std::string& endDateTime, const TimeUnit timeUnit)
{
    const auto startPoint{ TimeUtils::ParseTime(startDateTime) };
    const auto endPoint{ TimeUtils::ParseTime(endDateTime) };

    switch (timeUnit)
    {
        case TimeUnit::SECOND:
        {
            return std::chrono::duration_cast<std::chrono::seconds>(endPoint - startPoint).count();
        }
        case TimeUnit::MILLI_SECOND:
        {
            return std::chrono::duration_cast<std::chrono::milliseconds>(endPoint - startPoint).count();
        }
        case TimeUnit::MICRO_SECOND:
        {
            return std::chrono::duration_cast<std::chrono::microseconds>(endPoint - startPoint).count();
        }
        case TimeUnit::NANO_SECOND:
        {
            return std::chrono::duration_cast<std::chrono::nanoseconds>(endPoint - startPoint).count();
        }
        default:
        {
            return 0;
        }
    }
}

std::wstring TimeUtils::GetStartOfDayW(const bool isLocal, FormatStyle style)
{
    std::chrono::system_clock::time_point now{ std::chrono::system_clock::now() };
    std::time_t nowTime{ std::chrono::system_clock::to_time_t(now) };
    std::tm utcTm{};

    ::gmtime_s(&utcTm, &nowTime);

    utcTm.tm_hour = 0;
    utcTm.tm_min = 0;
    utcTm.tm_sec = 0;

    std::time_t midnight{ std::mktime(&utcTm) };
    std::chrono::system_clock::time_point midnightTP{ std::chrono::system_clock::from_time_t(midnight) };

    return FormatTimeW(midnightTP, isLocal, style);
}

std::string TimeUtils::GetStartOfDay(const bool isLocal, FormatStyle style)
{
    std::chrono::system_clock::time_point now{ std::chrono::system_clock::now() };
    std::time_t nowTime{ std::chrono::system_clock::to_time_t(now) };
    std::tm utcTm{};

    ::gmtime_s(&utcTm, &nowTime);

    utcTm.tm_hour = 0;
    utcTm.tm_min = 0;
    utcTm.tm_sec = 0;

    std::time_t midnight{ std::mktime(&utcTm) };
    std::chrono::system_clock::time_point midnightTP{ std::chrono::system_clock::from_time_t(midnight) };

    return FormatTime(midnightTP, isLocal, style);
}

std::wstring TimeUtils::GetStartOfDateW(const bool isLocal)
{
    std::wstring date{ GetStartOfDayW(isLocal).substr(0, 10) };

    return date;
}

std::string TimeUtils::GetStartOfDate(const bool isLocal)
{
    std::string date{ GetStartOfDay(isLocal).substr(0, 10) };

    return date;
}