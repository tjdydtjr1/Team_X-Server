#pragma once

/*=========================
        Vector
=========================*/

/**
* @brief : 2D, 3D 벡터 구조체 x,y,z
*/

#include <cmath>
#include <concepts>
#include "../macro/Define.h"

#pragma region Vector2D, Vector3D Struct
template<Arithmetic T>
struct Vector2
{
    T x{};
    T y{};

    Vector2 operator+(const Vector2& other) const
    {
        return { x + other.x, y + other.y };
    }

    Vector2 operator-(const Vector2& other) const
    {
        return { x - other.x, y - other.y };
    }

    Vector2 operator*(const T& value) const
    {
        return { x * value, y * value };
    }

    Vector2 operator/(const T& value) const
    {
        if (value == 0)
        {
            return Vector2{};
        }

        return { x / value, y / value };
    }

    bool operator==(const Vector2& other) const
    {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Vector2& other) const
    {
        return !(*this == other);
    }

    T Magnitude() const
    {
        return std::sqrt(x * x + y * y);
    }

    Vector2 Normalize() const
    {
        const T mag{ Magnitude() };

        if (mag == 0)
        {
            return Vector2{};
        }

        return { x / mag, y / mag };
    }
};

template<Arithmetic T>
struct Vector3
{
    T x{};
    T y{};
    T z{};

    Vector3 operator+(const Vector3& other) const
    {
        return { x + other.x, y + other.y, z + other.z };
    }

    Vector3 operator-(const Vector3& other) const
    {
        return { x - other.x, y - other.y, z - other.z };
    }

    Vector3 operator*(const T& value) const
    {
        return { x * value, y * value, z * value };
    }

    Vector3 operator/(const T& value) const
    {
        if (value == 0)
        {
            return Vector3{};
        }

        return { x / value, y / value, z / value };
    }

    bool operator==(const Vector3& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const Vector3& other) const
    {
        return !(*this == other);
    }

    T Magnitude() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vector3 Normalize() const
    {
        const T mag{ Magnitude() };

        if (mag == 0)
        {
            return Vector3{};
        }

        return { x / mag, y / mag, z / mag };
    }
};

#pragma endregion

template<typename T>
struct std::hash<Vector2<T>>
{
    size_t operator()(const Vector2<T>& v) const
    {
        std::hash<T> hasher;

        size_t h1{ hasher(v.x) };
        size_t h2{ hasher(v.y) };

        return h1 ^ (h2 << 1);
    }
};

template<typename T>
struct std::hash<Vector3<T>>
{
    size_t operator()(const Vector3<T>& v) const
    {
        std::hash<T> hasher;

        size_t h1{ hasher(v.x) };
        size_t h2{ hasher(v.y) };

        return h1 ^ (h2 << 1);
    }
};