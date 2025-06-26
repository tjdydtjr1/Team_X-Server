#pragma once

/*=========================
	  GameMapManager
=========================*/

#include "../type/GameMapType.h"

class Player;

class GameMapManager
{
	SINGLETON_BASE(GameMapManager);

public:
	void Init();
	void Clear();
	void Update(const float deltaTime);

	void CreateStaticMap();
	int64 CreateMap(const GameMapType mapType);
	void AddPlayer(const int64 mapId, const PlayerRef& player);
	void RemovePlayer(const int64 mapId, const int64 playerId);

	void ChangeToMap(const int64 mapId, const PlayerRef& player);
	GameMapRef FindMap(const int64 mapId);

public:

private:
	USE_LOCK;
	atomic<int64> _mapId{};
	HashMap<int64, GameMapRef> _maps;
};

#define MapManager GameMapManager::GetInstance()

