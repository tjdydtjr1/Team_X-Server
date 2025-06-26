#include "pch.h"
#include "EnumConverter.h"

Protocol::ObjectType EnumConverter::ConvertProtobufObjectType(const GameObjectType type)
{
    switch (type)
    {
        case GameObjectType::NONE:   return Protocol::OBJECT_TYPE_NONE;
        case GameObjectType::PLAYER: return Protocol::OBJECT_TYPE_PLAYER;
        case GameObjectType::NPC:    return Protocol::OBJECT_TYPE_NPC;
        case GameObjectType::ENEMY:  return Protocol::OBJECT_TYPE_ENEMY;
        default:
        return Protocol::OBJECT_TYPE_NONE;
    }
}

GameObjectType EnumConverter::ConvertGameObjectType(const Protocol::ObjectType type)
{
    switch (type)
    {
        case Protocol::OBJECT_TYPE_NONE:   return GameObjectType::NONE;
        case Protocol::OBJECT_TYPE_PLAYER: return GameObjectType::PLAYER;
        case Protocol::OBJECT_TYPE_NPC:    return GameObjectType::NPC;
        case Protocol::OBJECT_TYPE_ENEMY:  return GameObjectType::ENEMY;
        default:
        return GameObjectType::NONE;
    }
}
