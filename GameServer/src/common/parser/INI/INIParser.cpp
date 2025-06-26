#include "pch.h"
#include "INIParser.h"

bool INIParser::Load(const std::string& filePath)
{
	std::ifstream file(filePath);

	if (!file.is_open())
	{
		return false;
	}

	std::string line;
	std::string section;

	while (std::getline(file, line))
	{
		line = Trim(line);

		if (line.empty())
		{
			continue;
		}

		// section 추출
		if (line.front() == '[' && line.back() == ']')
		{
			// [first, ... end]
			section = line.substr(1, line.size() - 2);
			continue;
		}

		// value값 위치
		const size_t equal{ line.find('=') };
		
		// 값이 존재하지 않을 경우
		if (equal == std::string::npos)
		{
			continue;
		}

		std::string key{ Trim(line.substr(0, equal)) };
		std::string value{ Trim(line.substr(equal + 1)) };

		_data[section][key] = value;
	}

	return true;
}

// 공백제거
std::string INIParser::Trim(const std::string& str) const
{
	const auto begin{ str.find_first_not_of(" \t\r\n") };

	if (begin == std::string::npos)
	{
		return "";
	}

	const auto end{ str.find_last_not_of(" \t\r\n") };

	return str.substr(begin, end - begin + 1);
}

std::optional<std::string> INIParser::GetValue(const std::string& section, const std::string& key) const
{
	const auto iter{_data.find(section)};

	if (iter != _data.end())
	{
		const auto keyIter{ iter->second.find(key) };

		if (keyIter != iter->second.end())
		{
			return keyIter->second;
		}
	}

	return std::nullopt;
}

