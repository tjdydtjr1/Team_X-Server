#pragma once
#include "Types.h"
#include "MemoryPool.h"

/*===============================
			ObjectPool
=================================*/

/**Type(object) 전용으로 메모리 할당*/
template<typename Type>
class ObjectPool
{
public:
	/**
	* @brief 오브젝트 풀에서 객체 1개 할당 및 생성
	* @param args 객체 생성자로 전달될 인자들
	* @return 생성된 객체의 포인터
	* @note _STOMP 매크로 정의 시, 메모리 오버런 탐지를 위한 StompAllocator 사용
	*/
	template<typename ...Args>
	static Type* Pop(Args&& ...args)
	{
#ifdef _STOMP
		MemoryHeader* ptr{ reinterpret_cast<MemoryHeader*>( StompAllocator::Alloc(s_allocSize) ) };
		Type* memory{ static_cast<Type*>( MemoryHeader::AttachHeader(ptr, s_allocSize) ) };
#else
		Type* memory{ static_cast<Type*>( MemoryHeader::AttachHeader(s_pool.Pop(), s_allocSize) ) };
#endif
		new(memory)Type(forward<Args>(args)...); // placement new

		return memory;
	}

	/**
	* @brief 사용이 끝난 객체를 풀에 반납
	* @details 객체의 소멸자를 명시적으로 호출한 뒤, 메모리를 내부 풀에 반환
	* @param obj 반납할 객체의 포인터
	*/
	static void Push(Type* obj)
	{
		obj->~Type();
#ifdef _STOMP
		StompAllocator::Release(MemoryHeader::DetachHeader(obj));
#else
		s_pool.Push(MemoryHeader::DetachHeader(obj));
#endif
	}

	/**
	* @brief 오브젝트 풀을 사용하는 shared_ptr 생성
	* @details 객체가 더 이상 참조되지 않을 때 소멸자 대신 Push 함수가 자동으로 호출됨
	* @param args 객체 생성자로 전달될 인자들
	* @return 생성된 객체를 가리키는 shared_ptr
	*/
	template<typename... Args>
	static shared_ptr<Type> MakeShared(Args&&... args)
	{
		shared_ptr<Type> ptr{ Pop(std::forward<Args>(args)...), Push };

		return ptr;
	}

	/**
	* @brief 오브젝트 풀을 사용하는 unique_ptr 생성
	* @details unique_ptr의 수명이 다했을 때 소멸자 대신 Push 함수가 자동으로 호출됨
	* @param args 객체 생성자로 전달될 인자들
	* @return 생성된 객체를 가리키는 unique_ptr
	*/
	template<typename... Args>
	static std::unique_ptr<Type, void(*)(Type*)> MakeUnique(Args&&... args)
	{
		return std::unique_ptr<Type, void(*)(Type*)>{ Pop(std::forward<Args>(args)...), Push };
	}

private:
	static int32		s_allocSize;
	static MemoryPool	s_pool;
};

template<typename Type>
int32 ObjectPool<Type>::s_allocSize = sizeof(Type) + sizeof(MemoryHeader);

template<typename Type>
MemoryPool ObjectPool<Type>::s_pool{ s_allocSize };