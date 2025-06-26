#pragma once

/*============================
        ConfigTypes
==============================*/

struct ServerSetting
{
    std::string ip;
    int port{};
    int maxSession{};
};

struct RedisSetting
{
    std::string ip;
    int port{};
};

struct DatabaseSetting
{
	std::string driver;
	std::string ip;
    std::string database;
	std::string user;
	std::string password;
};

/*============================
        ConfigLoader
==============================*/

#include "INI/INIParser.h"
#include <optional>

class ConfigLoader
{
public:
    ConfigLoader() = default;
    ~ConfigLoader() = default;

public:
    /**
	* @brief : ini파일을 읽어오는 함수
	* @param : filePath : ini파일 경로
	* @return : 성공여부
    */
    bool INILoad(const std::string& filePath);

    /**
	* @brief : INI GameServer 설정을 가져오는 함수
    */
    std::optional<ServerSetting> GetGameServerSetting() const;

    /**
	* @brief : INI Redis 설정을 가져오는 함수
    */
    std::optional<RedisSetting> GetRedisSetting() const;

    /**
	* @brief : INI Database 설정을 가져오는 함수
    */
    std::optional<DatabaseSetting> GetDatabaseSetting() const;

private:
    INIParser _iniParser{};
};

