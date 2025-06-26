#pragma once

/*====================================
			SendBuffer
====================================*/

class SendBufferCheck;

/**
* @brief 송신 버퍼
* @details SendBuffer는 네트워크로 송신할 데이터를 임시로 저장하는 버퍼
*/
class SendBuffer
{
public:
	SendBuffer(SendBufferChunkRef owner, BYTE* buffer, uint32 allocSize);
	~SendBuffer();

	BYTE* Buffer() const { return _buffer; }

	uint32 AllocSize() const { return _allocSize; }

	uint32 WriteSize() const { return _writeSize;}
	void Close(uint32 writeSize);

private:
	BYTE* _buffer;
	uint32 _allocSize{};
	uint32 _writeSize{};
	SendBufferChunkRef _owner;
};


/*====================================
		   SendBufferChunk
====================================*/

/**
* @brief 송신 버퍼를 큰 사이즈로 생성하여 관리
* @details SendBufferChunk는 여러 개의 SendBuffer를 관리하는 역할을 수행
*/
class SendBufferChunk : public enable_shared_from_this<SendBufferChunk>
{
	enum
	{
		SEND_BUFFER_CHUNK_SIZE = 6000 // 4096
	};

public:
	SendBufferChunk();
	~SendBufferChunk();

	void			Reset();
	SendBufferRef	Open(uint32 allocSize);
	void			Close(uint32 writeSize);
	bool			IsOpen() const { return _open; }

	BYTE* Buffer() { return &_buffer[_usedSize]; }

	uint32 FreeSize() { return static_cast<uint32>(_buffer.size()) - _usedSize; }

private:
	Array<BYTE, SEND_BUFFER_CHUNK_SIZE> _buffer{};
	bool _open{};
	uint32 _usedSize{};
};


/*====================================
		   SendBufferManager
====================================*/

class SendBufferManager
{
public:
	SendBufferRef				Open(uint32 size);

private:
	SendBufferChunkRef			Pop();
	void						Push(SendBufferChunkRef buffer);

	static void					PushGlobal(SendBufferChunk* buffer);

private:
	USE_LOCK;
	Vector<SendBufferChunkRef> _sendBufferChunks;
};