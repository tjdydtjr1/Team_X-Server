#pragma once

/*==============================
       SystemIDGenerator
==============================*/

#include <atomic>

class SystemIDGenerator
{
public:
    template<typename T>
    static int GetId()
    {
        static const int id{ _syetemId.fetch_add(1, std::memory_order_relaxed) };

        return id;
    }

private:
    inline static std::atomic<int> _syetemId{};
};

