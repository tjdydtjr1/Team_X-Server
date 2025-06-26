#pragma once

/*=====================
	   MoveSystem
=======================*/

#include "../base/BaseSystem.h"

class MoveSystem : public BaseSystem
{
public:
	MoveSystem();
	virtual ~MoveSystem() = default;
	virtual void Update(const float deltaTime) override;
	
};

