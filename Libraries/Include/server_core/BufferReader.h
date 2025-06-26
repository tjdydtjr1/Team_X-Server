#pragma once

/*=======================================
			BufferReader
=======================================*/

/**
* @brief BufferReader
* @details BufferReader는 버퍼에서 데이터를 읽는 유틸리티
*/
class BufferReader
{
public:
	BufferReader();
	BufferReader(BYTE* buffer, uint32 size, uint32 pos = 0);
	~BufferReader();

public:
	BYTE* Buffer()
	{
		return _buffer;
	}

	uint32 Size() const
	{
		return _size;
	}

	uint32 ReadSize() const
	{
		return _pos;
	}

	uint32 FreeSize() const
	{
		return _size - _pos;
	}

	template<typename T>
	bool Peek(T* dest)
	{
		return Peek(dest, sizeof(T));
	}
	bool Peek(void* dest, const uint32 len) const;

	template<typename T>
	bool Read(T* dest)
	{
		return Read(dest, sizeof(T));
	}
	bool Read(void* dest, const uint32 len);

	template<typename T>
	BufferReader& operator >>(OUT T& dest);

private:
	BYTE* _buffer{};
	uint32 _size{};
	uint32 _pos{};
};

template<typename T>
inline BufferReader& BufferReader::operator >>(OUT T& dest)
{
	dest = *reinterpret_cast<T*>(&_buffer[_pos]);
	_pos += sizeof(T);

	return *this;
}