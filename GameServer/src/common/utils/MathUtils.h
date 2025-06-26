#pragma once

/*===========================
        MathUtils
===========================*/

#include <random>
#include <cmath>
#include <algorithm>
#include <chrono>
#include "macro/Define.h"

class MathUtils
{
public:
#pragma region 난수
    /**
    * @brief : min ~ max 사이의 랜덤한 값을 반환하는 함수
    * @param : min = 최소값 
    * @param : max = 최대값
    * @return : 랜덤한 값
    */
    template<Arithmetic T>
    static T GetRandom(const T& min, const T& max);

#pragma endregion

#pragma region 거리 계산
    /**
    * @brief : 2D 두 벡터 사이의 거리를 계산하는 함수
    * @param : x1 = 첫 번째 점의 x 좌표
    * @param : y1 = 첫 번째 점의 y 좌표
    * @param : x2 = 두 번째 점의 x 좌표
    * @param : y2 = 두 번째 점의 y 좌표
    * @return : 두 점 사이의 거리
    */
    template<Arithmetic T>
    static T GetDistance(const T& x1, const T& y1, const T& x2, const T& y2);

    /**
    * @brief : 2D Vector2D 사이의 거리를 계산하는 함수
    * @param : a = 첫 번째 점 Vector2D
    * @param : b = 두 번째 점 Vector2D
    * @return : 두 점 사이의 거리
    */
    template<Arithmetic T>
    static T GetDistance(const Vector2<T>& a, const Vector2<T>& b);

    /**
    * @brief : 3D Vector3D 사이의 거리를 계산하는 함수
    * @param : a = 첫 번째 점 Vector3D
    * @param : b = 두 번째 점 Vector3D
    * @return : 두 점 사이의 거리
    */
    template<Arithmetic T>
    static T GetDistance(const Vector3<T>& a, const Vector3<T>& b);

#pragma endregion

#pragma region 각도 계산
    /**
    * @brief : 2D 두 벡터 사이의 각도를 계산하는 함수
    * @param : x1 = 첫 번째 점의 x 좌표
    * @param : y1 = 첫 번째 점의 y 좌표
    * @param : x2 = 두 번째 점의 x 좌표
    * @param : y2 = 두 번째 점의 y 좌표
    * @param : isDegree = true일 경우 각도를 도 단위로 반환
    * @return : 두 점 사이의 각도
    */
    template<Arithmetic T>
    static T GetAngle2D(const T& x1, const T& y1, const T& x2, const T& y2, const bool isDegree = false);

    /**
    * @brief : 2D Vector2D 두 벡터 사이의 각도를 계산하는 함수
    * @param : a = 첫 번째 점 Vector2D
    * @param : b = 두 번째 점 Vector2D
    * @param : isDegree = true일 경우 각도를 도 단위로 반환
    * @return : 두 점 사이의 각도
    */
    template<Arithmetic T>
    static T GetAngle2D(const Vector2<T>& a, const Vector2<T>& b, const bool isDegree = false);

    /**
    * @brief : 3D 두 벡터 사이의 각도를 계산하는 함수
    * @param : x1 = 첫 번째 점의 x 좌표
    * @param : y1 = 첫 번째 점의 y 좌표
    * @param : z1 = 첫 번째 점의 z 좌표
    * @param : x2 = 두 번째 점의 x 좌표
    * @param : y2 = 두 번째 점의 y 좌표
    * @param : z2 = 두 번째 점의 z 좌표
    * @param : isDegree = true일 경우 각도를 도 단위로 반환
    */
    template<Arithmetic T>
    static T GetAngle3D(const T& x1, const T& y1, const T& z1, const T& x2, const T& y2, const T& z2, const bool isDegree = false);

    /**
    * @brief : 3D Vector3D 두 벡터 사이의 각도를 계산하는 함수
    * @param : a = 첫 번째 벡터 Vector3D
    * @param : b = 두 번째 벡터 Vector3D
    * @param : isDegree = true일 경우 각도를 도 단위로 반환
    * @return : 두 벡터 사이의 각도
    */
    template<Arithmetic T>
    static T GetAngle3D(const Vector3<T>& a, const Vector3<T>& b, const bool isDegree = false);
#pragma endregion

#pragma region 내적
    /**
	* @brief : 두 벡터의 내적을 계산하는 함수
	* @param : x1 = 첫 번째 벡터의 x 좌표
	* @param : y1 = 첫 번째 벡터의 y 좌표
	* @param : x2 = 두 번째 벡터의 x 좌표
	* @param : y2 = 두 번째 벡터의 y 좌표
	* @return : 두 벡터의 내적
    */
    template<Arithmetic T>
    static T DotProduct(const T& x1, const T& y1, const T& x2, const T& y2);
   
    /**
	* @brief : 두 벡터의 내적을 계산하는 함수
	* @param : a = 첫 번째 벡터 Vector2D
	* @param : b = 두 번째 벡터 Vector2D
	* @return : 두 벡터의 내적
    */
    template<Arithmetic T>
    static T DotProduct(const Vector2<T>& a, const Vector2<T>& b);
   
    /**
    * @brief : 3D 벡터 두 벡터 사이의 내적을 계산하는 함수
    * @param : a = 첫 번째 벡터 Vector3D 
    * @param : b = 두 번째 벡터 Vector3D 
    * @return : 두 벡터 사이의 내적
    */
    template<Arithmetic T>
    static T DotProduct(const Vector3<T>& a, const Vector3<T>& b);
   
    /**
    * @brief : 3D 벡터 두 벡터 사이의 내적을 계산하는 함수 (x, y, z 좌표 값 사용)
    * @param : x1 = 첫 번째 벡터의 x 좌표
    * @param : y1 = 첫 번째 벡터의 y 좌표
    * @param : z1 = 첫 번째 벡터의 z 좌표
    * @param : x2 = 두 번째 벡터의 x 좌표
    * @param : y2 = 두 번째 벡터의 y 좌표
    * @param : z2 = 두 번째 벡터의 z 좌표
    * @return : 두 벡터 사이의 내적 결과
    */
    template<Arithmetic T>
    static T DotProduct(const T& x1, const T& y1, const T& z1, const T& x2, const T& y2, const T& z2);
   
#pragma endregion

#pragma region 외적
    /**
    * @brief : 3D 벡터 두 벡터의 외적을 계산하는 함수
    * @param : a = 첫 번째 벡터 Vector3D
    * @param : b = 두 번째 벡터 Vector3D
    * @return : 두 벡터의 외적 결과를 나타내는 새로운 Vector3D
    */
    template<Arithmetic T>
    static Vector3<T> CrossProduct(const Vector3<T>& a, const Vector3<T>& b);

    /**
    * @brief : 3D 벡터 두 벡터의 외적을 계산하는 함수 (입력값을 개별적으로 받는 버전)
    * @param : x1, y1, z1 = 첫 번째 벡터의 x, y, z 값
    * @param : x2, y2, z2 = 두 번째 벡터의 x, y, z 값
    * @return : 두 벡터의 외적 결과를 나타내는 새로운 Vector3D
    */
    template<Arithmetic T>
    static Vector3<T> CrossProduct(const T& x1, const T& y1, const T& z1, const T& x2, const T& y2, const T& z2);
#pragma endregion

private:
    template<Arithmetic T>
    static T ToDegree(const T radian);

private:
    static constexpr double PI{ 3.14159265358979323846 };
    static constexpr double RAD_TO_DEG{ 180.0 / PI };
};

template<Arithmetic T>
inline T MathUtils::GetRandom(const T& min, const T& max)
{
    thread_local static std::mt19937 generator([]() 
    {
        std::random_device rd;
        return rd() ^ static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    }());

    if constexpr (std::is_integral_v<T>)
    {
        std::uniform_int_distribution<T> distribution(min, max);

        return distribution(generator);
    }
    else
    {
        std::uniform_real_distribution<T> distribution(min, max);

        return distribution(generator);
    }
}

template<Arithmetic T>
inline T MathUtils::GetDistance(const T& x1, const T& y1, const T& x2, const T& y2)
{
    const T dx{ x1 - x2 };
    const T dy{ y1 - y2 };

    return std::sqrt(dx * dx + dy * dy);
}

template<Arithmetic T>
inline T MathUtils::GetDistance(const Vector2<T>& a, const Vector2<T>& b)
{
    const T dx{ a.x - b.x };
    const T dy{ a.y - b.y };

    return std::sqrt(dx * dx + dy * dy);
}

template<Arithmetic T>
inline T MathUtils::GetDistance(const Vector3<T>& a, const Vector3<T>& b)
{
    const T dx{ a.x - b.x };
    const T dy{ a.y - b.y };
    const T dz{ a.z - b.z };

    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

template<Arithmetic T>
inline T MathUtils::GetAngle2D(const T& x1, const T& y1, const T& x2, const T& y2, const bool isDegree)
{
    const T dx{ x2 - x1 };
    const T dy{ y2 - y1 };

    T angle{ std::atan2(dy, dx) };

    if (angle < 0)
    {
        angle += static_cast<T>(2 * PI);
    }

    if (isDegree)
    {
        angle = ToDegree(angle);
    }

    return angle;
}

template<Arithmetic T>
inline T MathUtils::GetAngle2D(const Vector2<T>& a, const Vector2<T>& b, const bool isDegree)
{
    const T dx{ b.x - a.x };
    const T dy{ b.y - a.y };

    T angle{ std::atan2(dy, dx) };

    if (angle < 0)
    {
        angle += static_cast<T>(2 * PI);
    }

    if (isDegree)
    {
        angle = ToDegree(angle);
    }

    return angle;
}

template<Arithmetic T>
inline T MathUtils::GetAngle3D(const T& x1, const T& y1, const T& z1, const T& x2, const T& y2, const T& z2, const bool isDegree)
{
    const T dot{ x1 * x2 + y1 * y2 + z1 * z2 };
    const T magnitudeA{ std::sqrt(x1 * x1 + y1 * y1 + z1 * z1) };
    const T magnitudeB{ std::sqrt(x2 * x2 + y2 * y2 + z2 * z2) };

    const T cosAngle{ std::clamp(dot / (magnitudeA * magnitudeB), static_cast<T>(-1), static_cast<T>(1)) };
    T angle{ std::acos(cosAngle) };

    if (isDegree)
    {
        angle = ToDegree(angle);
    }

    return angle;
}

template<Arithmetic T>
inline T MathUtils::GetAngle3D(const Vector3<T>& a, const Vector3<T>& b, const bool isDegree)
{
    const T dot{ a.x * b.x + a.y * b.y + a.z * b.z };
    const T magnitudeA{ std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z) };
    const T magnitudeB{ std::sqrt(b.x * b.x + b.y * b.y + b.z * b.z) };

    const T cosAngle{ std::clamp(dot / (magnitudeA * magnitudeB), static_cast<T>(-1), static_cast<T>(1)) };
    T angle{ std::acos(cosAngle) };

    if (isDegree)
    {
        angle = ToDegree(angle);
    }

    return angle;
}

template<Arithmetic T>
inline T MathUtils::DotProduct(const T& x1, const T& y1, const T& x2, const T& y2)
{
    return x1 * x2 + y1 * y2;
}

template<Arithmetic T>
inline T MathUtils::DotProduct(const Vector2<T>& a, const Vector2<T>& b)
{
    return a.x * b.x + a.y * b.y;
}

template<Arithmetic T>
inline T MathUtils::DotProduct(const Vector3<T>& a, const Vector3<T>& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template<Arithmetic T>
inline T MathUtils::DotProduct(const T& x1, const T& y1, const T& z1, const T& x2, const T& y2, const T& z2)
{
    return x1 * x2 + y1 * y2 + z1 * z2;
}

template<Arithmetic T>
inline Vector3<T> MathUtils::CrossProduct(const Vector3<T>& a, const Vector3<T>& b)
{
    const T x{ a.y * b.z - a.z * b.y };
    const T y{ a.z * b.x - a.x * b.z };
    const T z{ a.x * b.y - a.y * b.x };

    return Vector3<T>{x, y, z};
}

template<Arithmetic T>
inline Vector3<T> MathUtils::CrossProduct(const T& x1, const T& y1, const T& z1, const T& x2, const T& y2, const T& z2)
{
    const T x{ y1 * z2 - z1 * y2 };
    const T y{ z1 * x2 - x1 * z2 };
    const T z{ x1 * y2 - y1 * x2 };

    return Vector3<T>{x, y, z};
}

template<Arithmetic T>
inline T MathUtils::ToDegree(const T radian)
{
    return radian * static_cast<T>(RAD_TO_DEG);
}