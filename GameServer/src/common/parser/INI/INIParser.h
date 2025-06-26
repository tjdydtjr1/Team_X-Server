#pragma once

/*==========================
		INIParser
==========================*/

#include <fstream>
#include <unordered_map>
#include <string>
#include <optional>
#include <charconv>
#include "../..//macro/Define.h"

class INIParser
{
public:
	/**
	* @brief : ini파일을 읽어오는 함수
	* @param : filePath : ini파일 경로
	* @return : 성공여부
	*/
	bool Load(const std::string& filePath);

	/**
	* @brief : ini파일에서 section, key에 해당하는 value를 가져오는 함수
	* @param : section = ini파일의 섹션
	* @param : key = ini파일의 키
	* @return : std::optional<std::string> 형식의 값
	*/
	std::optional<std::string> GetValue(const std::string& section, const std::string& key) const;
	
	/**
	* @brief : ini파일에서 section, key에 해당하는 value를 가져오는 함수
	* @param : section = ini파일의 섹션
	* @param : key = ini파일의 키
	* @return : std::optional<T> 형식의 값
	*/
	template<Arithmetic T>
	std::optional<T> GetTypeValue(const std::string& section, const std::string& key) const;

private:
	std::string Trim(const std::string& str) const;
	
private:
	// [section] -> [key], [value]
	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> _data;
};


template<Arithmetic T>
inline std::optional<T> INIParser::GetTypeValue(const std::string& section, const std::string& key) const
{
	const auto value{ GetValue(section, key)};

	if (value.has_value())
	{
		const std::string& str{ value.value() };
		T value{};

		std::from_chars_result result{ std::from_chars(str.data(), str.data() + str.size(), value) };
		
		if (result.ec == errc::invalid_argument || result.ec == errc::result_out_of_range)
		{
			return std::nullopt;
		}

		return value;
	}

	return std::nullopt;
}

