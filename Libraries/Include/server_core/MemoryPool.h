#pragma once

/*====================================
			MemoryPool
====================================*/
/*
* 크기별로 메모리 관리
* [32] [64] [128] [] []
*/

enum
{
	SLIST_ALIGNMENT = 16
};

/**
* @brief 메모리 헤더
* @details MemoryHeader는 메모리 풀에서 할당된 메모리 블록의 헤더로 할당된 크기를 저장하고 SLIST_ENTRY를 상속받아 연결 리스트로 관리
*/
DECLSPEC_ALIGN(SLIST_ALIGNMENT)
struct MemoryHeader : public SLIST_ENTRY 
{
	// [MemoryHeader][Data]
	MemoryHeader(const int32 size)
		:allocSize(size)
	{
	}

	/**
	* @brief 주어진 메모리 공간에 MemoryHeader를 생성하고 데이터 영역의 포인터를 반환
	* @param header 헤더를 생성할 메모리 주소
	* @param size 이 헤더가 관리할 데이터의 크기
	* @return 데이터 영역의 시작 주소 (void*)
	* @note C++의 placement new를 사용하여 이미 할당된 메모리에 객체를 생성
	*/
	static void* AttachHeader(MemoryHeader* header, const int32 size)
	{
		// placement new
		new(header)MemoryHeader(size);

		// [MemoryHeader].[Data] Data부분으로 캐스팅 후 반환
		return reinterpret_cast<void*>(++header);
	}

	/**
	* @brief 데이터 포인터로부터 MemoryHeader의 주소를 계산하여 반환
	* @param ptr AttachHeader가 반환했던 데이터 영역 포인터
	* @return 데이터 영역에 해당하는 MemoryHeader의 포인터
	*/
	static MemoryHeader* DetachHeader(void* ptr)
	{
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;
		
		// [MemoryHeader].[Data] MemoryHeader부분으로 캐스팅 후 반환
		return header;
	}

	int32 allocSize{};
};

/**
* @brief 특정 크기의 메모리 블록을 관리하는 메모리 풀
* @details Windows의 InterlockedSList API를 사용하여 Lock 없이 단방향 연결 리스트를 관리하므로 여러 스레드에서 동시에 Push/Pop을 호출해도 안전
*/
DECLSPEC_ALIGN(SLIST_ALIGNMENT)
class MemoryPool
{
public:
	MemoryPool(const int32 allocSize);
	~MemoryPool();

	/**
	* @brief 사용이 끝난 메모리 블록을 풀에 반환
	* @param ptr 반환할 메모리 블록의 헤더 포인터
	*/
	void			Push(MemoryHeader* ptr);

	/**
	* @brief 풀에서 가용한 메모리 블록을 하나 가져옴
	* @details 만약 풀에 가용한 메모리가 없다면, 시스템에서 새로 메모리를 할당
	* @return 가용한 메모리 블록의 헤더 포인터
	*/
	MemoryHeader*	Pop();


private:
	SLIST_HEADER  _header{};
	int32		  _allocSize{};
	atomic<int32> _useCount{};
	atomic<int32> _reserveCount{};

};

