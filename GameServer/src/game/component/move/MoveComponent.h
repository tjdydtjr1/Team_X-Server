#pragma once

/*=====================
	 MoveComponent
=====================*/

#include "../Component.h"

class MoveComponent : public IComponent
{
public:
	MoveComponent() = default;
	MoveComponent(const Vector3<float>& position, float yaw = 0.f, float speed = 0.f) noexcept
		: _position(position), _yaw(yaw), _speed(speed) {}
	virtual ~MoveComponent() = default;

public:
	/**Getter Setter*/ 
	const Vector3<float>& GetPosition() const noexcept { return _position; }
	void SetPosition(const Vector3<float>& position) noexcept { _position = position; }

	float GetYaw() const noexcept { return _yaw; } 
	void SetYaw(const float yaw) noexcept { _yaw = yaw; }

	float GetX() const noexcept { return _position.x; }
	void SetX(const float x) noexcept { _position.x = x; }

	float GetY() const noexcept { return _position.y; }
	void SetY(const float y) noexcept { _position.y = y; }

	float GetZ() const noexcept { return _position.z; }
	void SetZ(const float z) noexcept { _position.z = z; }

	float GetSpeed() const noexcept { return _speed; }
	void SetSpeed(const float speed) noexcept { _speed = speed; }

private:
	Vector3<float> _position{};
	float _yaw{};
	float _speed{};
};

