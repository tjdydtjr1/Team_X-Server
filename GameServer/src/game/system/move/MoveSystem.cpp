#include "pch.h"
#include "MoveSystem.h"
#include "../../map/GameMap.h"
#include "../../object/GameObject.h"
#include "../../component/move/MoveComponent.h"

MoveSystem::MoveSystem()
{
	_componentBitmask.set(GetComponentId<MoveComponent>());
}

void MoveSystem::Update(const float deltaTime)
{
	Protocol::S_MOVE pkt{};

	for (const GameObjectRef& object : _sparseSet)
	{
		const auto& moveComponent{ object->GetComponent<MoveComponent>() };

		if (moveComponent)
		{
			auto* info{ pkt.add_info() };
		
			info->set_object_id(object->GetObjectId());
			info->set_yaw(moveComponent->GetYaw());
			auto* vector3{ info->mutable_position() };
			vector3->set_x(moveComponent->GetX());
			vector3->set_y(moveComponent->GetY());
			vector3->set_z(moveComponent->GetZ());
		}
	}

	SendBufferRef sendBuffer{ ClientPacketHandler::MakeSendBuffer(pkt) };

	if (const auto& gameMap = GetOwner())
	{
		gameMap->PushJob([gameMap, buffer = std::move(sendBuffer)]
		{
			gameMap->Broadcast(buffer);
		});
	}
}


