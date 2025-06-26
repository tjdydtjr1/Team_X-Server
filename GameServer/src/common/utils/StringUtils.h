#pragma once

/*================================
			StringUtils
================================*/

#pragma once
#include <string>
#include <vector>
#include <sstream>  

class StringUtils
{
public:
    /**
	* @brief : string을 wstring으로 변환하는 함수
	* @param : str = 변환할 string
	* @return : 변환된 wstring
    */
    static std::wstring StringToWstring(const std::string& str);

    /**
	* @brief : wstring을 string으로 변환하는 함수
	* @param : wstr = 변환할 wstring
	* @return : 변환된 string
    */
    static std::string WstringToString(const std::wstring& wstr);
    
    /*
	* @brief : null 또는 공백인지 확인하는 함수
	* @param : str = 확인할 문자열
	* @return : null 또는 공백이면 true, 아니면 false
    */
    static bool IsNullOrWhitespace(const std::string& str){ return str.find_first_not_of(" \t\n\r") == std::string::npos; }
    static bool IsNullOrWhitespace(const std::wstring& str) { return str.find_first_not_of(L" \t\n\r") == std::wstring::npos; }

    /**
	* @brief : 문자열을 delimiter로 나누는 함수
	* @param : str = 나눌 문자열
	* @param : delimiter = 구분자
	* @return : 나누어진 문자열 벡터
    */
    static std::vector<std::string> Split(const std::string& str, char delimiter);
    static std::vector<std::wstring> Split(const std::wstring& str, wchar_t delimiter);

    /**
	* @brief : 특정 문자열을 변경 하는 함수
	* @param : str = 변경할 문자열
	* @param : from = 변경될 특정 문자열
	* @param : to = 대체되는 특정 문자열
	* @return : 변경된 문자열
    */
    static void ReplaceAll(std::string& str, const std::string& from, const std::string& to);
    static void ReplaceAll(std::wstring& str, const std::wstring& from, const std::wstring& to);

    /**
	* @brief : 문자열을 대문자로 변환하는 함수
	* @param : str = 변환할 문자열
	* @return : 변환된 문자열
    */
    static void ToUpper(std::string& str) { std::transform(str.begin(), str.end(), str.begin(), ::toupper); }
    static void ToUpper(std::wstring& str) { std::transform(str.begin(), str.end(), str.begin(), ::toupper); }

    /**
	* @brief : 문자열을 소문자로 변환하는 함수
	* @param : str = 변환할 문자열
	* @return : 변환된 문자열
    */
    static void ToLower(std::string& str) { std::transform(str.begin(), str.end(), str.begin(), ::tolower); }
    static void ToLower(std::wstring& str) { std::transform(str.begin(), str.end(), str.begin(), ::tolower); }
};

