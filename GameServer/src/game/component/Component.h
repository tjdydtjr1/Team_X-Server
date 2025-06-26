#pragma once

/*=========================
		Component
=========================*/

class GameObject;

class IComponent
{
protected:
	virtual ~IComponent() = default;

public:
	GameObjectRef GetOwner() const { return _ownerGameObject.lock(); }
	void SetOwner(const GameObjectRef& object) { _ownerGameObject = object; }

private:
	weak_ptr<GameObject> _ownerGameObject;

};

