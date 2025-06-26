#pragma once

/*==========================
		BaseSystem
==========================*/

#include "../System.h"
#include "../../type/Types.h"

class GameObject;

class BaseSystem : public ISystem
{
public:
	virtual void Update(const float deltaTime) = 0;
	virtual void Register(const GameObjectRef& gameObject) override;
	virtual void UnRegister(const int64 unRegistId) override;
	virtual void SetOwner(const GameMapRef& gameMap) override { _owner = gameMap; };

protected:
	GameMapRef GetOwner() const { return _owner.lock(); }

protected:
    SparseSet<uint64, GameObjectRef> _sparseSet{};
    ComponentBitmask _componentBitmask{};
	weak_ptr<GameMap> _owner{};
};

