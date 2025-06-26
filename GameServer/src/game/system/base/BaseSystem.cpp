#include "pch.h"
#include "BaseSystem.h"
#include "../../object/GameObject.h"

void BaseSystem::Register(const GameObjectRef& gameObject)
{
    if ((gameObject->GetBitmask() & _componentBitmask) != _componentBitmask)
    {
        return;
    }

    _sparseSet.add(gameObject->GetObjectId(), gameObject);
}
void BaseSystem::UnRegister(const int64 unRegistId)
{
    _sparseSet.remove(unRegistId);
}