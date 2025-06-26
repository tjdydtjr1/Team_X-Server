#pragma once

/*=========================
		GameObject
=========================*/

/**
* Object Base
*/

#include "../type/GameObjectType.h"
#include "../component/utils/ComponentContainer.h"
#include "../type/Types.h"

class GameObject : public enable_shared_from_this<GameObject>
{
public:
	GameObject() = default;
	virtual ~GameObject() = default;

public:
	template<typename T>
	void AddComponent(const shared_ptr<T>& component);

	template<typename T>
	void RemoveComponent();

	template<typename T>
	bool HasComponent() const;

public:
	/**Getter Setter*/ 
	template<typename T>
	shared_ptr<T> GetComponent() const;

	const Vector<shared_ptr<IComponent>>& GetAllComponents() const { return _components.GetAllComponents(); }

	int64 GetObjectId() const { return _objectId; };
	void SetObjectId(const int objectID) { _objectId = objectID; };

	string GetName() const { return _name; }
	void SetName(const string& name) { _name = name; }

	GameMapRef GetCurrentMap() const { return _ownerMap.lock(); }
	void SetCurrentMap(const GameMapRef& gameMap){ _ownerMap = gameMap; }

	Vector3<float> GetPosition() const { return _position; }
	void SetPosition(const Vector3<float>& position) { _position = position; }

	float GetYaw() const { return _yaw; }
	void SetYaw(const float yaw) { _yaw = yaw; }

	float GetX() const { return _position.x; }
	void SetX(const float x) { _position.x = x; }

	float GetY() const { return _position.y; }
	void SetY(const float y) { _position.y = y; }

	float GetZ() const { return _position.z; }
	void SetZ(const float z) { _position.z = z; }

	int64 GetMapLocalId() const noexcept { return _mapLocalId; }
	void SetMapLocalId(const int64 mapLocalId) noexcept { _mapLocalId = mapLocalId; }

	GameObjectType GetGameObjectType() const { return _objectType; }
	void SetGameObjectType(const GameObjectType objectType) { _objectType = objectType; }

	const ComponentBitmask& GetBitmask() const noexcept{ return _componentBitmask; }

private:
	int64 _objectId{};
	int64 _mapLocalId{ -1 };
	string _name;
	weak_ptr<GameMap> _ownerMap;
	Vector3<float> _position{};
	float _yaw{};
	GameObjectType _objectType{};
	ComponentContainer _components{};
	ComponentBitmask _componentBitmask{};
};

template<typename T>
inline void GameObject::AddComponent(const shared_ptr<T>& component)
{
	const int id{ ComponentIDGenerator::GetId<T>() };

	if (HasComponent<T>())
	{
		return;
	}

	component->SetOwner(shared_from_this());
	_components.AddComponent<T>(component);
	_componentBitmask.set(id);
}

template<typename T>
inline shared_ptr<T> GameObject::GetComponent() const
{
	return _components.GetComponent<T>();
}

template<typename T>
inline void GameObject::RemoveComponent()
{
	const int id{ ComponentIDGenerator::GetId<T>() };

	if (HasComponent<T>() == false)
	{
		return;
	}

	_components.RemoveComponent<T>();
	_componentBitmask.reset(id);
}

template<typename T>
inline bool GameObject::HasComponent() const
{
	const int id{ ComponentIDGenerator::GetId<T>() };

	return _componentBitmask.test(id);
}