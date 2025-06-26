#include "pch.h"
#include "Player.h"
#include "../../component/move/MoveComponent.h"

void Player::Init()
{
	AddComponent<MoveComponent>(MakeShared<MoveComponent>());
}