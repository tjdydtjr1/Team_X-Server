#pragma once

/*==============================
		ComponentContainer
===============================*/

#include "../Component.h"
#include "ComponentIDGenerator.h"

class ComponentContainer
{
public:
	template<typename T>
	void AddComponent(const shared_ptr<IComponent>& component);

	template<typename T>
	shared_ptr<T> GetComponent() const;

	template<typename T>
	void RemoveComponent();

public:
	const Vector<shared_ptr<IComponent>>& GetAllComponents() const { return _components; }

private:
	Vector<shared_ptr<IComponent>> _components;
	HashMap<int, int> _idToIndex;
};

template<typename T>
inline void ComponentContainer::AddComponent(const shared_ptr<IComponent>& component)
{
	static_assert(std::is_base_of_v<IComponent, T>, "Add Component Error");

	const int id{ ComponentIDGenerator::GetId<T>() };

	if (_idToIndex.contains(id))
	{
		return;
	}

	const size_t index{ _components.size() };
	_idToIndex[id] = index;
	_components.emplace_back(std::move(component));
}

template<typename T>
inline shared_ptr<T> ComponentContainer::GetComponent() const
{
	static_assert(std::is_base_of_v<IComponent, T>, "Get Component Error");

	const int id{ ComponentIDGenerator::GetId<T>() };
	const auto it{ _idToIndex.find(id) };

	if (it == _idToIndex.end())
	{
		return nullptr;
	}

	return static_pointer_cast<T>(_components[it->second]);
}

template<typename T>
inline void ComponentContainer::RemoveComponent()
{
	static_assert(std::is_base_of_v<IComponent, T>, "Remove Component Error");

	const int id{ ComponentIDGenerator::GetId<T>() };
	const auto it{ _idToIndex.find(id) };

	if (it == _idToIndex.end())
	{
		return;
	}

	const int removeIndex{ it->second };
	const int lastIndex{ _components.size() - 1 };

	if (removeIndex != lastIndex)
	{
		_components[removeIndex] = std::move(_components[lastIndex]);

		for (auto& [compId, idx] : _idToIndex)
		{
			if (idx == lastIndex)
			{
				idx = removeIndex;

				break;
			}
		}
	}

	_components.pop_back();
	_idToIndex.erase(it);
}
