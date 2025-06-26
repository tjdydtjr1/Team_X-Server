#pragma once

/*==========================
		GameServer
 =========================*/

class GameServer : public enable_shared_from_this<GameServer>
{
public:
	~GameServer();

public:
	bool Init();
	void Run();
	void Shutdown() { _isRunning = false; }

private:
	void DoWorkerThread();
	void MainLoop();

private:
	ServerServiceRef _service{};
	atomic<bool> _isRunning{ true };
};

