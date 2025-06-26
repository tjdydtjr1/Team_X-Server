#include "pch.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "../map/GameMapManager.h"
#include "../object/actor/Player.h"

GameSession::~GameSession()
{
	const wstring ipAddr{ GetAddress().GetIpAddress() };

	LOG_DEBUG("Disconnect - {}", StringUtils::WstringToString(ipAddr));
}

void GameSession::OnConnected()
{
	SessionManager.Add(static_pointer_cast<GameSession>(shared_from_this()));

	LOG_INFO("Session Count - {}", SessionManager.GetSessionCount());
}

void GameSession::OnDisconnected()
{
	const int64 objectId{ GetObjectId() };
	const int64 mapId{ GetMapId() };

	if (objectId != 0)
	{
		MapManager.RemovePlayer(mapId, objectId);
	}

	SessionManager.Remove(static_pointer_cast<GameSession>(shared_from_this()));

	LOG_INFO("Session Count - {}", SessionManager.GetSessionCount());
}

void GameSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	PacketSessionRef session{ GetPacketSessionRef() };
	PacketHeader* header{ reinterpret_cast<PacketHeader*>( buffer ) };

	ClientPacketHandler::HandlePacket(session, buffer, len);
}

void GameSession::OnSend(int32 len)
{

}
