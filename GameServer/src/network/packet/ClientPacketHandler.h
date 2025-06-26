#pragma once

#if UE_BUILD_DEBUG + UE_BUILD_DEVELOPMENT + UE_BUILD_TEST + UE_BUILD_SHIPPING >= 1
#include "Protocol.pb.h"
#include "BufferReader.h"
#include "BufferWriter.h"
#include "SendBuffer.h"
#include "Team_X.h"
#endif

using PacketHandlerFunc = std::function<bool(PacketSessionRef&, BYTE*, int32)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];

enum : uint16
{
	PKT_C_LOGIN = 1000,
	PKT_S_LOGIN = 1001,
	PKT_C_ENTER_GAME = 1002,
	PKT_S_ENTER_GAME = 1003,
	PKT_S_SPAWN = 1004,
	PKT_S_DESPAWN = 1005,
	PKT_C_MOVE = 1006,
	PKT_S_MOVE = 1007,
};

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len);
bool Handle_C_LOGIN(PacketSessionRef& session, Protocol::C_LOGIN& pkt);
bool Handle_C_ENTER_GAME(PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt);
bool Handle_C_MOVE(PacketSessionRef& session, Protocol::C_MOVE& pkt);

class ClientPacketHandler
{
public:
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; ++i)
		{
			GPacketHandler[i] = Handle_INVALID;
		}

		GPacketHandler[PKT_C_LOGIN] = [](PacketSessionRef& session, BYTE* buffer, int32 len) {return HandlePacket <Protocol::C_LOGIN> (Handle_C_LOGIN, session, buffer, len); };
		GPacketHandler[PKT_C_ENTER_GAME] = [](PacketSessionRef& session, BYTE* buffer, int32 len) {return HandlePacket <Protocol::C_ENTER_GAME> (Handle_C_ENTER_GAME, session, buffer, len); };
		GPacketHandler[PKT_C_MOVE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) {return HandlePacket <Protocol::C_MOVE> (Handle_C_MOVE, session, buffer, len); };
	}

	static bool HandlePacket(PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header{ reinterpret_cast<PacketHeader*>(buffer) };
		
		return GPacketHandler[header->id](session, buffer, len);
	}
	
public:
    
    static SendBufferRef MakeSendBuffer(Protocol::S_LOGIN& pkt) { return MakeSendBuffer(pkt, PKT_S_LOGIN); }    
    static SendBufferRef MakeSendBuffer(Protocol::S_ENTER_GAME& pkt) { return MakeSendBuffer(pkt, PKT_S_ENTER_GAME); }    
    static SendBufferRef MakeSendBuffer(Protocol::S_SPAWN& pkt) { return MakeSendBuffer(pkt, PKT_S_SPAWN); }    
    static SendBufferRef MakeSendBuffer(Protocol::S_DESPAWN& pkt) { return MakeSendBuffer(pkt, PKT_S_DESPAWN); }    
    static SendBufferRef MakeSendBuffer(Protocol::S_MOVE& pkt) { return MakeSendBuffer(pkt, PKT_S_MOVE); }

private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketType pkt{};

		if (pkt.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)) == false)
		{
			return false;
		}

		return func(session, pkt);
	}

	// Buffer �۽�
	template<typename T>
	static SendBufferRef MakeSendBuffer(T& pkt, uint16 pktId)
	{
		const uint16 dataSize{ static_cast<uint16>(pkt.ByteSizeLong()) };
		const uint16 packetSize{ dataSize + sizeof(PacketHeader) };
		
#if UE_BUILD_DEBUG + UE_BUILD_DEVELOPMENT + UE_BUILD_TEST + UE_BUILD_SHIPPING >= 1
		SendBufferRef sendBuffer{ MakeShared<SendBuffer>(packetSize) };
#else
		SendBufferRef sendBuffer{ GSendBufferManager->Open(packetSize) };
#endif
		PacketHeader* header{ reinterpret_cast<PacketHeader*>(sendBuffer->Buffer()) };
		header->size = packetSize;
		header->id = pktId;
		
		pkt.SerializeToArray(&header[1], dataSize);
		sendBuffer->Close(packetSize);

		return sendBuffer;
	}
};
