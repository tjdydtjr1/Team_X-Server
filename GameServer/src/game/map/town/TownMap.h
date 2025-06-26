#pragma once

/*==========================
		TownMap
==========================*/

#include "../GameMap.h"

class TownMap : public GameMap
{
public:
	virtual void Init() override;
	virtual void OnUpdate(const float deltaTime) override;
};

