#include "pch.h"
#include "ConfigLoader.h"
#include <filesystem>

bool ConfigLoader::INILoad(const std::string& filePath)
{
    return _iniParser.Load(filePath);
}

std::optional<ServerSetting> ConfigLoader::GetGameServerSetting() const
{
    const auto ip{ _iniParser.GetValue("Server", "IP") };
    const auto port{ _iniParser.GetTypeValue<int>("Server", "PORT") };
    const auto maxSession{ _iniParser.GetTypeValue<int>("Server", "MAX_SESSION") };

    if (!ip.has_value() || !port.has_value() || !maxSession.has_value())
    {
        return std::nullopt;
    }

    return ServerSetting{ ip.value(), port.value(), maxSession.value() };
}

std::optional<RedisSetting> ConfigLoader::GetRedisSetting() const
{
    const auto ip{ _iniParser.GetValue("Redis", "IP") };
    const auto port{ _iniParser.GetTypeValue<int>("Redis", "PORT") };

    if (!ip.has_value() || !port.has_value())
    {
        return std::nullopt;
    }

    return RedisSetting{ ip.value(), port.value() };
}

std::optional<DatabaseSetting> ConfigLoader::GetDatabaseSetting() const
{
    const auto driver{ _iniParser.GetValue("Database", "DRIVER") };
	const auto ip{ _iniParser.GetValue("Database", "IP") };
	const auto database{ _iniParser.GetValue("Database", "DATABASE") };
	const auto user{ _iniParser.GetValue("Database", "USER") };
	const auto password{ _iniParser.GetValue("Database", "PASSWORD") };

    if (!driver.has_value() || !ip.has_value() || !database.has_value()
        || !user.has_value() || !password.has_value())
    {
		return std::nullopt;
    }

	return DatabaseSetting{driver.value(), ip.value(), database.value(), user.value(), password.value()};
}
