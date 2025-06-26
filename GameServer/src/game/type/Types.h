#pragma once

#include <bitset>

constexpr int MAX_COMPONENT_SIZE{ 64 };
constexpr int MAX_SYSTEM_SIZE{ 32 };

using ComponentBitmask = std::bitset<MAX_COMPONENT_SIZE>;
using SystemBitmask = std::bitset<MAX_SYSTEM_SIZE>;