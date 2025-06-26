#pragma once

/*====================================
			RecvBuffer
====================================*/

/**
* @brief RecvBuffer 클래스
* @details RecvBuffer는 네트워크로부터 수신한 데이터를 임시로 저장하는 버퍼
* [r] [] [] [w] [] [] [] [] []
* r = _readPos
* w = _writePos
*/
class RecvBuffer
{
public:
	RecvBuffer(const int32 bufferSize);
	~RecvBuffer();

	void Clean();
	bool OnRead(const int32 numOfBytes);
	bool OnWrite(const int32 numOfBytes);

	BYTE* ReadPos() { return &_buffer[_readPos]; }
	BYTE* WritePos() { return &_buffer[_writePos]; }
	int32 DataSize() const { return _writePos - _readPos; }
	int32 FreeSize() const { return _bufferSize - _writePos; }

private:
	int32 _bufferSize{};
	int32 _readPos{};
	int32 _writePos{};
	Vector<BYTE> _buffer;
};

