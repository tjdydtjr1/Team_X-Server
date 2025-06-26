#pragma once

/*=======================================
			GameSession
=======================================*/

class Player;

class GameSession : public PacketSession
{
public:
	~GameSession();

public:
	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual void OnRecvPacket(BYTE* buffer, int32 len) override;
	virtual void OnSend(int32 len) override;

public:
	/**Getter, Setter*/
	void SetPlayer(PlayerRef player) { _player = player; }
	PlayerRef GetPlayer() const { return _player; }

	void SetMapId(const int64 mapId) { _mapId = mapId; }
	int64 GetMapId() const { return _mapId; }

	void SetObjectId(const int64 object) { _object = object; }
	int64 GetObjectId() const { return _object; }

private:
	PlayerRef _player;
	int64 _mapId{};
	int64 _object{};
};

