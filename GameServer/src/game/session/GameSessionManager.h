#pragma once

/*=======================================
			GameSessionManager
=======================================*/

class GameSession;

class GameSessionManager
{
	SINGLETON_BASE(GameSessionManager);

public:
	void Add(GameSessionRef session);
	void Remove(GameSessionRef session);
	void Broadcast(const SendBufferRef& sendBuffer);

	int GetSessionCount() const { return static_cast<int>( _sessions.size() ); }

private:
	USE_LOCK;

	set<GameSessionRef> _sessions;
};

#define SessionManager GameSessionManager::GetInstance()