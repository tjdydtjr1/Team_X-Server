#pragma once

/*=======================================
			BufferWriter
=======================================*/

/**
* @brief BufferWriter
* @details BufferWriter는 버퍼에 데이터를 쓰기 위한 유틸리티
*/
class BufferWriter
{
public:
	BufferWriter();
	BufferWriter(BYTE* buffer, const uint32 size, const uint32 pos = 0);
	~BufferWriter();

public:
	BYTE* Buffer()
	{
		return _buffer;
	}

	uint32 Size() const
	{
		return _size;
	}

	uint32 WriteSize() const
	{
		return _pos;
	}

	uint32 FreeSize() const
	{
		return _size - _pos;
	}

	template<typename T>
	bool Write(T* src)
	{
		return Read(src, sizeof(T));
	}
	bool Write(void* src, uint32 len);

	template<typename T>
	T* Reserve(uint16 count);

	template<typename T>
	BufferWriter& operator <<(T&& src);

private:
	BYTE* _buffer = nullptr;
	uint32 _size = 0;
	uint32 _pos = 0;
};

template<typename T>
inline T* BufferWriter::Reserve(uint16 count)
{
	if (FreeSize() < sizeof(T) * count)
	{
		return nullptr;
	}

	T* ret = reinterpret_cast<T*>(&_buffer[_pos]);
	
	_pos += sizeof(T) * count;
	return ret;
}


// 보편 참조
// const T&
// T&&
template<typename T>
inline BufferWriter& BufferWriter::operator<<(T&& src)
{
	// 레퍼런스 제거 const T& -> const T
	using DataType = std::remove_reference_t<T>;

	// 보편참조일 땐 move가아니라 forward 사용
	*reinterpret_cast<DataType*>(&_buffer[_pos]) = std::forward<DataType>(src);
	
	_pos += sizeof(T);
	return *this;
}
