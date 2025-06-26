#pragma once

/*==============================
     ComponentIDGenerator
==============================*/

#include <atomic>

class ComponentIDGenerator
{
public:
    template<typename T>
    static int GetId()
    {
        static const int id{ _componentId.fetch_add(1, std::memory_order_relaxed) };

        return id;
    }

private:
    inline static std::atomic<int> _componentId{};
};

template<typename T>
inline int GetComponentId()
{
    return ComponentIDGenerator::GetId<T>();
}