#include "pch.h"
#include "GameServer.h"
#include "session/GameSessionManager.h"
#include "session/GameSession.h"
#include "map/GameMapManager.h"
#include "timer/WorldTimer.h"

GameServer::~GameServer()
{
	LogManager.Shutdown();
	GThreadManager->Join();
}

bool GameServer::Init()
{
	/*===========================Config==============================*/
	const string& iniPath{ "./config/ini/GameServer.ini" };
	unique_ptr<ConfigLoader> config{ std::make_unique<ConfigLoader>() };

	if (config->INILoad(iniPath) == false)
	{
		LOG_ERROR("Config File Load Failed Path: {}", iniPath);

		return false;
	}

	const auto serverSetting{ config->GetGameServerSetting() };
	auto redisSetting{ config->GetRedisSetting() };
	const auto databaseSetting{ config->GetDatabaseSetting() };
	
	if (serverSetting.has_value() == false)
	{
		LOG_ERROR("GameServerSetting Not Found");

		return false;
	}

	if (redisSetting.has_value() == false)
	{
		LOG_ERROR("RedisSetting Not Found");

		return false;
	}

	if (databaseSetting.has_value() == false)
	{
		LOG_ERROR("DatabaseSetting Not Found");

		return false;
	}

	// TODO: NetAddress::DomainToIp(redisSetting->ip);
	/*===========================Init==============================*/
	ClientPacketHandler::Init();
	DumpManager.Init();
	MonitorManager.Init();
	LOGGER_INIT(10, 1, 1024);
	MapManager.Init();
	WorldTimeManager.Init();
	
	// TODO: RedisMgr.Connect(redisSetting->ip, redisSetting->port);
	
	/*===========================Setting==============================*/
	_service = { MakeShared<ServerService>(
		NetAddress(StringUtils::StringToWstring(serverSetting->ip), static_cast<uint16>( serverSetting->port )),
		MakeShared<IocpCore>(),
		MakeShared<GameSession>,
		serverSetting->maxSession) };

	if (_service->Start() == false)
	{
		LOG_ERROR("ServerService Failed To Start");

		return false;
	}

	LOG_INFO("GameServer Ip: {}, Port: {} ", serverSetting->ip, serverSetting->port);
	
	return true;
}

void GameServer::Run()
{
	const auto threadCount{ thread::hardware_concurrency() };

	for (int32 i = 0; i < threadCount / 2; ++i)
	{
		GThreadManager->Launch([self = shared_from_this()]()
		{
			CryptoManager::Init();
			self->DoWorkerThread();
		});
	}
	
	LOG_INFO("WorkerThread Count: {}", threadCount);

	MainLoop();
}

void GameServer::DoWorkerThread()
{
	constexpr int32 WORKER_TICK_MS{ 64 };
	constexpr int32 IOCP_DISPATCH_TIMEOUT_MS{ 10 };

	while (_isRunning)
	{
		LEndTickCount = ::GetTickCount64() + WORKER_TICK_MS;
		_service->GetIocpCore()->Dispatch(IOCP_DISPATCH_TIMEOUT_MS);

		ThreadManager::DoGlobalQueueWork();
		ThreadManager::DistributeReserveJob();
	}
}

void GameServer::MainLoop()
{
	while (_isRunning)
	{
		WorldTimeManager.Tick();

		const float deltaTime{ WorldTimeManager.GetDeltaTime() };
		MapManager.Update(deltaTime);

		WorldTimeManager.MaintainFrameRate();
	}
}