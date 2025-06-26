#pragma once

/*===================
	   System
===================*/

class GameMap;
class GameObject;

class ISystem
{
public:
	virtual ~ISystem() = default;
	virtual void Update(const float deltaTime) = 0;
	virtual void SetOwner(const GameMapRef& gameMap) = 0;
	virtual void Register(const GameObjectRef& gameObject) = 0;
	virtual void UnRegister(const int64 unRegistId) = 0;

};

