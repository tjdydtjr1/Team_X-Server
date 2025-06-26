#include "pch.h"
#include "StringUtils.h"

std::wstring StringUtils::StringToWstring(const std::string& str)
{
    if (str.empty())
    {
        return std::wstring();
    }

    const int size{ MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>( str.size() ), nullptr, 0) };
    
    if (size <= 0)
    {
        return std::wstring();
    }

    std::wstring result(size, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), &result[0], size);

    return result;
}

std::string StringUtils::WstringToString(const std::wstring& wstr)
{
    if (wstr.empty())
    {
        return std::string();
    }

    const int size{ WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>( wstr.size() ), nullptr, 0, nullptr, nullptr) };

    if (size <= 0)
    {
        return std::string();
    }

    std::string result(size, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.size()), &result[0], size, nullptr, nullptr);

    return result;
}

std::vector<std::string> StringUtils::Split(const std::string& str, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::stringstream tokenStream(str);

    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }

    return tokens;
}

std::vector<std::wstring> StringUtils::Split(const std::wstring& str, wchar_t delimiter)
{
    std::vector<std::wstring> tokens;
    std::wstring token;
    std::wstringstream tokenStream(str);

    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

void StringUtils::ReplaceAll(std::string& str, const std::string& from, const std::string& to)
{
    if (from.empty())
    {
        return;
    }

    size_t start_pos{};

    while ((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();  
    }
}

void StringUtils::ReplaceAll(std::wstring& str, const std::wstring& from, const std::wstring& to)
{
    if (from.empty())
    {
        return;
    }

    size_t start_pos{};

    while ((start_pos = str.find(from, start_pos)) != std::wstring::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}