#include "pch.h"
#include "ServerPacketHandler.h"
#include <chrono>
#include "ServerSession.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{

    return true;
}

bool Handle_S_LOGIN(PacketSessionRef& session, Protocol::S_LOGIN& pkt)
{
    auto s{ pkt.success() };

    cout << s << '\n';

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	Protocol::C_ENTER_GAME enterPkt;

	session->Send(ServerPacketHandler::MakeSendBuffer(enterPkt));
  
    return true;
}

bool Handle_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_ENTER_GAME& pkt)
{
	cout << "S_ENTER_GAME\n";

	auto client{ static_pointer_cast<ServerSession>( session ) };

	client->SetId(pkt.object_id());

	return true;
}


bool Handle_S_SPAWN(PacketSessionRef& session, Protocol::S_SPAWN& pkt)
{
	cout << "S_SPAWN received\n";
	
	auto client{ static_pointer_cast<ServerSession>( session ) };

	int64 id{ client->GetId() };

	// MOVE 패킷 지속적인 테스트용
	std::thread([session, id]() 
	{
		while (session->IsConnected())
		{
			std::this_thread::sleep_for(std::chrono::seconds(3));

			Protocol::C_MOVE movePkt;
			movePkt.mutable_info()->set_object_id(id); 
			auto* pos = movePkt.mutable_info()->mutable_position();
			pos->set_x(10.0f + (rand() % 100));
			pos->set_y(0.0f + (rand() % 100));
			pos->set_z(5.0f + (rand() % 100));
			movePkt.mutable_info()->set_yaw(90.0f);
			auto a = ServerPacketHandler::MakeSendBuffer(movePkt);
			session->Send(a);
		}
	}).detach();

    return true;
}

bool Handle_S_DESPAWN(PacketSessionRef& session, Protocol::S_DESPAWN& pkt)
{

    return true;
}

bool Handle_S_MOVE(PacketSessionRef& session, Protocol::S_MOVE& pkt)
{
	cout << "S_MOVE received\n";

    return true;
}

