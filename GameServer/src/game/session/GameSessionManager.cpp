#include "pch.h"
#include "GameSessionManager.h"
#include "GameSession.h"

void GameSessionManager::Add(GameSessionRef session)
{
	WRITE_LOCK;
	
	_sessions.insert(session);
}

void GameSessionManager::Remove(GameSessionRef session)
{
	WRITE_LOCK;

	_sessions.erase(session);
}

void GameSessionManager::Broadcast(const SendBufferRef& sendBuffer)
{
	WRITE_LOCK;

	for (const GameSessionRef& session : _sessions)
	{
		session->Send(sendBuffer);
	}
}