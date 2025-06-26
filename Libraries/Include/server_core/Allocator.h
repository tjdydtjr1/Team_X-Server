#pragma once

/*====================================
			BaseAllocator
====================================*/

/**
* @brief 커스텀 메모리 할당자
*/
class BaseAllocator
{
public:
	static void*		Alloc(const int32 size);
	static void			Release(void* ptr);
};



/*====================================
			StompAllocator
====================================*/

/**
* @brief VirtualAlloc 메모리 직접 할당자
* @details StompAllocator는 VirtualAlloc을 사용하여 메모리를 할당하고 메모리 오염을 방지하기 위해 할당된 메모리를 사용 불가로 초기화
*/
class StompAllocator
{
	enum
	{
		PAGE_SIZE = 0x1000
	};

public:
	static void*		Alloc(const int32 size);
	static void			Release(void* ptr);
};


/*====================================
			PoolAllocator
====================================*/

/**
* @brief 메모리 풀 할당자
* @details PoolAllocator는 고정 크기의 메모리 블록을 할당하고 해제하는 데 사용
*/
class PoolAllocator
{
public:
	static void*		Alloc(const int32 size);
	static void			Release(void* ptr);
};

/*====================================
			STL Allocator
====================================*/

/**
* @brief STL 호환 메모리 할당자
* @details STL 컨테이너에서 사용할 수 있는 커스텀 메모리 할당자
*/
template<typename T>
class StlAllocator
{
public:
	using value_type = T;

	StlAllocator(){}

	template<typename Other>
	StlAllocator(const StlAllocator<Other>&){}

	T* allocate(const size_t count)
	{
		const int32 size{ static_cast<int32>( count * sizeof(T) ) };
		
		return static_cast<T*>(PoolAllocator::Alloc(size));
	}

	void deallocate(T* ptr, const size_t count)
	{
		PoolAllocator::Release(ptr);
	}
};