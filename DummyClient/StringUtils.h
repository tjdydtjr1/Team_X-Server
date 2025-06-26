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
	* @brief : string�� wstring���� ��ȯ�ϴ� �Լ�
	* @param : str = ��ȯ�� string
	* @return : ��ȯ�� wstring
    */
    static std::wstring StringToWstring(const std::string& str);

    /**
	* @brief : wstring�� string���� ��ȯ�ϴ� �Լ�
	* @param : wstr = ��ȯ�� wstring
	* @return : ��ȯ�� string
    */
    static std::string WstringToString(const std::wstring& wstr);
    
    /*
	* @brief : null �Ǵ� �������� Ȯ���ϴ� �Լ�
	* @param : str = Ȯ���� ���ڿ�
	* @return : null �Ǵ� �����̸� true, �ƴϸ� false
    */
    static bool IsNullOrWhitespace(const std::string& str){ return str.find_first_not_of(" \t\n\r") == std::string::npos; }
    static bool IsNullOrWhitespace(const std::wstring& str) { return str.find_first_not_of(L" \t\n\r") == std::wstring::npos; }

    /**
	* @brief : ���ڿ��� delimiter�� ������ �Լ�
	* @param : str = ���� ���ڿ�
	* @param : delimiter = ������
	* @return : �������� ���ڿ� ����
    */
    static std::vector<std::string> Split(const std::string& str, char delimiter);
    static std::vector<std::wstring> Split(const std::wstring& str, wchar_t delimiter);

    /**
	* @brief : Ư�� ���ڿ��� ���� �ϴ� �Լ�
	* @param : str = ������ ���ڿ�
	* @param : from = ����� Ư�� ���ڿ�
	* @param : to = ��ü�Ǵ� Ư�� ���ڿ�
	* @return : ����� ���ڿ�
    */
    static void ReplaceAll(std::string& str, const std::string& from, const std::string& to);
    static void ReplaceAll(std::wstring& str, const std::wstring& from, const std::wstring& to);

    /**
	* @brief : ���ڿ��� �빮�ڷ� ��ȯ�ϴ� �Լ�
	* @param : str = ��ȯ�� ���ڿ�
	* @return : ��ȯ�� ���ڿ�
    */
    static void ToUpper(std::string& str) { std::transform(str.begin(), str.end(), str.begin(), ::toupper); }
    static void ToUpper(std::wstring& str) { std::transform(str.begin(), str.end(), str.begin(), ::toupper); }

    /**
	* @brief : ���ڿ��� �ҹ��ڷ� ��ȯ�ϴ� �Լ�
	* @param : str = ��ȯ�� ���ڿ�
	* @return : ��ȯ�� ���ڿ�
    */
    static void ToLower(std::string& str) { std::transform(str.begin(), str.end(), str.begin(), ::tolower); }
    static void ToLower(std::wstring& str) { std::transform(str.begin(), str.end(), str.begin(), ::tolower); }
};

