#pragma once


#include "Allocator.h"

/*====================================
			Memory
====================================*/

class MemoryPool;

class Memory
{
	enum
	{
		// ~1024까지 32 Byte, ~2048(128 Byte), ~4096(256 Byte)
		POOL_COUNT = (1024 / 32) + (1024 / 128) + (2048 / 256),
		MAX_ALLOC_SIZE = 4096
	};

public:
	Memory();
	~Memory();

	/**
	* @brief 메모리 할당
	* @details 요청된 크기에 가장 적합한 메모리 풀을 조회 테이블에서 찾아 할당 진행
	* @param size 할당할 메모리 크기
	* @return 할당된 메모리 블록의 주소
	*/
	void*	Allocate(int32 size);

	/**
	* @brief 메모리 반납
	* @details 메모리 블록의 헤더를 읽어 어떤 풀에서 할당되었는지 파악 후 해당 풀에 반납
	* @param ptr 반납할 메모리 블록의 주소
	*/
	void	Release(void* ptr);

private:
	vector<MemoryPool*> _pools;

	// 메모리 크기 <-> 메모리 풀
	// O(1) 빠르게 찾기 위한 테이블
	MemoryPool* _poolTable[MAX_ALLOC_SIZE + 1];
};


/*====================================
			BaseAllocator
====================================*/

/**
* @brief 커스텀 메모리 풀을 이용한 객체 생성 new 대체
* @tparam Type 생성할 객체 타입
* @param args 객체 생성자로 전달될 인자
* @return 생성된 객체의 포인터
*/
template<typename Type, typename... Args>
Type* ynew(Args&&... args)
{
	Type* memory{ static_cast<Type*>( PoolAllocator::Alloc(sizeof(Type)) ) };

	// placement new => 생성자를 호출할 수 있음
	// ㄴ 베레딕 템플릿 ...Args => std::forward를 통해 가변 인자를 받을 수 있음
	new(memory)Type(std::forward<Args>(args)...);
	
	return memory;
}

/**
* @brief 커스텀 메모리 풀을 이용한 객체 소멸 delete 대체
* @details 객체의 소멸자를 명시적으로 호출 후 메모리를 풀에 반납
* @tparam Type 소멸시킬 객체 타입
* @param obj 소멸시킬 객체의 포인터
*/
template<typename Type>
void ydelete(Type* obj)
{
	// 소멸자 호출
	obj->~Type();

	PoolAllocator::Release(obj);
}

/**
* @brief 커스텀 메모리 풀을 사용하는 shared_ptr 생성 std::make_shared 대체
* @details ydelete 함수를 커스텀 삭제자로 자동 등록
* @tparam Args 생성자 인자 타입
* @param args 객체 생성자로 전달될 인자
* @return 생성된 객체를 관리하는 shared_ptr
*/
template<typename Type, typename... Args>
shared_ptr<Type> MakeShared(Args&&... args)
{
	return shared_ptr<Type> { ynew<Type>(std::forward<Args>(args)...), ydelete<Type> };
}

/**
* @brief 커스텀 메모리 풀을 사용하는 unique_ptr 생성 std::make_unique 대체
* @details ydelete 함수를 커스텀 삭제자로 자동 등록
* @tparam Args 생성자 인자 타입
* @param args 객체 생성자로 전달될 인자
* @return 생성된 객체를 관리하는 unique_ptr
*/
template<typename Type, typename... Args>
unique_ptr<Type, void(*)(Type*)> MakeUnique(Args&&... args)
{
	return unique_ptr<Type, void(*)(Type*)>{ ynew<Type>(std::forward<Args>(args)...), ydelete<Type> };
}