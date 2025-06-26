#pragma once

/*=========================
	   EnumConverter
=========================*/

#include "../type/GameObjectType.h"

class EnumConverter
{
public:
    static Protocol::ObjectType ConvertProtobufObjectType(const GameObjectType type);
    static GameObjectType ConvertGameObjectType(const Protocol::ObjectType type);
};

