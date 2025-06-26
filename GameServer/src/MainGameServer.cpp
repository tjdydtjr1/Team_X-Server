#include "pch.h"
#include "game/GameServer.h"

static std::shared_ptr<GameServer> GServer;

static BOOL WINAPI ConsoleCtrlHandler(DWORD ctrlType);

int main()
{
	ASSERT_CRASH(::SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE));
	
	GServer = make_shared<GameServer>();
	
	if (GServer->Init() == false)
	{
		LOG_CRITICAL("Server Init Failed");
	}

	LOG_INFO("Server Init Success");
	GServer->Run();

	LOG_INFO("Server Shutdown");
	GServer.reset();

	return 0;
}

static BOOL WINAPI ConsoleCtrlHandler(DWORD ctrlType)
{
	switch (ctrlType)
	{
		case CTRL_C_EVENT:
		case CTRL_CLOSE_EVENT:
		case CTRL_BREAK_EVENT:
		case CTRL_SHUTDOWN_EVENT:
		break;
		default:
		return FALSE; 
	}

	if (GServer)
	{
		GServer->Shutdown();
	}

	this_thread::sleep_for(2s);

	::ExitProcess(0);

	return TRUE; 
}