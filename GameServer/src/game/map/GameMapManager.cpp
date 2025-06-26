#include "pch.h"
#include "GameMapManager.h"
#include "../object/actor/Player.h"
#include "town/TownMap.h"

void GameMapManager::Init()
{
	CreateStaticMap();
}

void GameMapManager::Clear()
{
	_maps.clear();
}

void GameMapManager::Update(const float deltaTime)
{
    for (const auto& [id, map] : _maps)
    {
        map->Update(deltaTime);
    }
}

void GameMapManager::CreateStaticMap()
{
	CreateMap(GameMapType::TOWN);
}

int64 GameMapManager::CreateMap(const GameMapType mapType)
{
    WRITE_LOCK; 

    const int64 newMapId{ _mapId.fetch_add(1) };
    GameMapRef newMap;

    switch (mapType)
    {
        case GameMapType::TOWN:
        {
            newMap = MakeShared<TownMap>();

            break;
        }
       
        default:
        {
            LOG_ERROR("Unknown GameMapType: {}", static_cast<int>(mapType));

            return -1;
        }
    }

    if (newMap)
    {
        newMap->SetMapType(mapType);
        newMap->SetMapId(newMapId);
        newMap->Init();
        _maps.insert({ newMapId, newMap }); 

        LOG_INFO("Created new map: ID={}, Type={}", newMapId, static_cast<int>(mapType));

        return newMapId;
    }
}

void GameMapManager::AddPlayer(const int64 mapId, const PlayerRef& player)
{
	WRITE_LOCK;

	const auto& mapIter{ _maps.find(mapId) };

	if (mapIter != _maps.end())
	{
        auto& map{ mapIter->second };

        map->PushJob([map, player]
        {
            map->EnterObject(player);
            player->SetCurrentMap(map);
        });

		return;
	}
}

void GameMapManager::RemovePlayer(const int64 mapId, const int64 playerId)
{
	WRITE_LOCK;

	const auto& iter{ _maps.find(mapId) };

	if (iter != _maps.end())
	{
        auto& map{ iter->second };

        map->PushJob([map, playerId]()
        {
            if (const auto& object = map->FindObject(playerId))
            {
                map->LeaveObject(object);
            }
        });
	}
}

void GameMapManager::ChangeToMap(const int64 mapId, const PlayerRef& player)
{
    WRITE_LOCK;

    const auto& fromMap{ player->GetCurrentMap() };

    if (fromMap == nullptr)
    {
        LOG_ERROR("Player Invalid Map");

        return;
    }

    const auto& toMapIter{ _maps.find(mapId) };

    if (toMapIter == _maps.end())
    {
        LOG_ERROR("Target Map Invalid", mapId);

        return;
    }

    const auto& toMap{ toMapIter->second };

    if (fromMap == toMap)
    {
        return;
    }

    fromMap->PushJob([fromMap, toMap, player]
    {
        fromMap->LeaveObject(player);

        toMap->PushJob([toMap, player]()
        {
            toMap->EnterObject(player);
        });
    });
}

GameMapRef GameMapManager::FindMap(const int64 mapId)
{
    WRITE_LOCK;

    const auto& iter{ _maps.find(mapId) };

    if (iter != _maps.end())
    {
        return iter->second;
    }

    return nullptr;
}
