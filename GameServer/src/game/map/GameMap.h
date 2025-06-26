#pragma once

/*=========================
		 GameMap
=========================*/

#include "../type/GameMapType.h"
#include "../type/Types.h"

class Player;
class ISystem;

class GameMap : public JobQueue
{
public:
	GameMap() = default;
	virtual ~GameMap() = default;

public:
	virtual void Init() = 0;
	virtual void OnUpdate(const float deltaTime) = 0;

public:
	void Update(const float deltaTime);

public:
	void AddSystem(const shared_ptr<ISystem>& system);
	void EnterObject(const GameObjectRef& object);
	void LeaveObject(const GameObjectRef& object);

	GameObjectRef FindObject(const int64 objectId);

public:
	void Broadcast(const SendBufferRef sendBuffer, const int64 exceptId = -1);
	void BroadcastNewPlayerSpawn(const PlayerRef& player);
	void SendSpawnInfoToNewPlayer(const PlayerRef& newPlayer);

public:
	/**Getter Setter*/ 
	void SetMapType(const GameMapType mapType) noexcept { _mapType = mapType; }
	GameMapType GetMapType() const noexcept { return _mapType; }

	void SetMapId(const int64 mapId) noexcept { _mapId = mapId; }
	int64 GetMapId() const noexcept { return _mapId; }

	const Vector<shared_ptr<ISystem>>& GetSystems() const noexcept { return _systems; }
	const HashMap<int64, GameObjectRef>& GetGameObjects() const noexcept { return _gameObjects; }

private:
	int64 GenerateLocalId();
	void ReturnLocalId(const int64 localId);

private:
	HashMap<int64, GameObjectRef> _gameObjects;
	Vector<shared_ptr<ISystem>> _systems;
	Vector<GameObjectRef> _objectsLocalId{};
	GameMapType _mapType{};
	int64 _mapId{};
	int64 _nextLocalId{};
	stack<int64> _freeLocalIds{};
};

