#pragma once

class ServerSession : public PacketSession
{
public:
	~ServerSession()
	{
		cout << "~ServerSession" << endl;
	}

	virtual void OnConnected() override
	{
		// 처음 연결 시 로그인 테스트
		Protocol::C_LOGIN loginPkt;
		loginPkt.set_token("dummy_token");

		Send(ServerPacketHandler::MakeSendBuffer(loginPkt));
	}

	virtual void OnRecvPacket(BYTE* buffer, int32 len) override
	{
		PacketSessionRef session = GetPacketSessionRef();
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

		ServerPacketHandler::HandlePacket(session, buffer, len);
	}

	virtual void OnSend(int32 len) override
	{

	}

	virtual void OnDisconnected() override
	{

	}

	int64 GetId() const { return _id; }
	void SetId(int64 id) { _id = id; }

private:
	int64 _id{};
};