#include "pch.h"
#include "GameMap.h"
#include "../object/actor/Player.h"
#include "../session/GameSession.h"
#include "../system/System.h"

void GameMap::Update(const float deltaTime)
{
    for (const auto& system : _systems)
    {
        system->Update(deltaTime);
    }

    OnUpdate(deltaTime);
}

void GameMap::EnterObject(const GameObjectRef& object)
{
    const int64_t localId{ GenerateLocalId() };
    object->SetMapLocalId(localId);

    _gameObjects[object->GetObjectId()] = object;

    if (localId >= _objectsLocalId.size())
    {
        _objectsLocalId.resize(localId + 1, nullptr);
    }

    _objectsLocalId[localId] = object;

    for (const auto& system : _systems)
    {
        system->Register(object);
    }

    if (auto player = std::dynamic_pointer_cast<Player>(object))
    {
        SendSpawnInfoToNewPlayer(player);
        BroadcastNewPlayerSpawn(player);
    }
}

void GameMap::LeaveObject(const GameObjectRef& object)
{
    if (object == nullptr)
    {
        return;
    }

    LOG_DEBUG("Player 퇴장");

    const int64 objectId{ object->GetObjectId() };
    const int64 localId{ object->GetMapLocalId() };

    if (_gameObjects.contains(objectId) == false)
    {
        return;
    }

    for (const auto& system : _systems)
    {
        system->UnRegister(localId);
    }

    ReturnLocalId(localId);
    object->SetMapLocalId(-1);

    _gameObjects.erase(objectId);
    _objectsLocalId[localId] = nullptr;
}

GameObjectRef GameMap::FindObject(const int64 objectId)
{
    const auto& it{_gameObjects.find(objectId)};

    if (it != _gameObjects.end())
    {
        return it->second;
    }
 
    return nullptr;
}

void GameMap::Broadcast(const SendBufferRef sendBuffer, const int64 exceptId)
{
	for (const auto& object : _gameObjects)
	{
		const PlayerRef& player{ dynamic_pointer_cast<Player>(object.second) };

        if (player == nullptr)
        {
            continue;
        }

		if (player->GetObjectId() == exceptId)
		{
			continue;
		}

		const GameSessionRef& session{ player->GetGameSession() };

		if (session)
		{
			session->Send(sendBuffer);
		}
	}
}

void GameMap::BroadcastNewPlayerSpawn(const PlayerRef& player)
{
    LOG_DEBUG("Player 입장");

    Protocol::S_SPAWN spawnPkt;

    Protocol::ObjectInfo* info{ spawnPkt.add_object_info() };
    info->set_object_id(player->GetObjectId());

    Protocol::MoveInfo* move = info->mutable_move_info();
    move->set_object_id(player->GetObjectId());
    move->mutable_position()->set_x(player->GetX());
    move->mutable_position()->set_y(player->GetY());
    move->mutable_position()->set_z(player->GetZ());
    move->set_yaw(player->GetYaw());

    info->set_object_type(Protocol::ObjectType::OBJECT_TYPE_PLAYER);
    info->set_name(player->GetName());

    SendBufferRef sendBuffer{ ClientPacketHandler::MakeSendBuffer(spawnPkt) };
    Broadcast(sendBuffer, player->GetObjectId());
}

void GameMap::SendSpawnInfoToNewPlayer(const PlayerRef& newPlayer)
{
    Protocol::S_SPAWN spawnPkt;

    for (const auto& [id, player] : _gameObjects)
    {
        if (player == newPlayer)
        {
            continue;
        }

        Protocol::ObjectInfo* info = spawnPkt.add_object_info();
        info->set_object_id(player->GetObjectId());

        Protocol::MoveInfo* move = info->mutable_move_info();
        move->set_object_id(player->GetObjectId());
        move->mutable_position()->set_x(player->GetX());
        move->mutable_position()->set_y(player->GetY());
        move->mutable_position()->set_z(player->GetZ());
        move->set_yaw(player->GetYaw());

        info->set_object_type(Protocol::ObjectType::OBJECT_TYPE_PLAYER);
        info->set_name(player->GetName());
    }

    if (spawnPkt.object_info_size() > 0)
    {
        SendBufferRef sendBuffer{ ClientPacketHandler::MakeSendBuffer(spawnPkt) };
        newPlayer->GetGameSession()->Send(sendBuffer);
    }
    else
    {
        // FIXME
        SendBufferRef sendBuffer{ ClientPacketHandler::MakeSendBuffer(spawnPkt) };
        newPlayer->GetGameSession()->Send(sendBuffer);
    }
    
}

int64 GameMap::GenerateLocalId()
{
    if (_freeLocalIds.empty() == false)
    {
        const int64_t id{ _freeLocalIds.top() };
        _freeLocalIds.pop();

        return id;
    }

    return _nextLocalId++;
}

void GameMap::ReturnLocalId(const int64 localId)
{
    _freeLocalIds.push(localId);
}

void GameMap::AddSystem(const shared_ptr<ISystem>& system)
{
    _systems.push_back(system);
	system->SetOwner(static_pointer_cast<GameMap>(shared_from_this()));
}
