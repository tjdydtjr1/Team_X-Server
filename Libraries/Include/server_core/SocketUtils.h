#pragma once

/*====================================
			SocketUtils
====================================*/

#include "NetAddress.h"

/**
* @brief 소켓 유틸리티 클래스
* @details SocketUtils는 소켓 생성, 바인딩, 리스닝, 클로징 등 소켓 관련 작업을 수행
* @details Windows에서 IOCP를 사용하기 위한 ConnectEx, DisconnectEx, AcceptEx 함수 포인터를 관리
*/
class SocketUtils
{
public:
	/**
	* @brief Windows에서 IOCP를 사용하기 위한 비동기 함수 포인터
	*/
	static LPFN_CONNECTEX		ConnectEx;
	static LPFN_DISCONNECTEX	DisconnectEx;
	static LPFN_ACCEPTEX		AcceptEx;

public:
	/**
	* @brief 비동기 소켓 작업을 위한 함수 포인터를 초기화
	*/
	static void Init();
	static void Clear();

	/**
	* @brief Windows에서 제공하는 비동기 소켓 함수 포인터를 바인딩
	*/
	static bool BindWindowsFunction(SOCKET socket, GUID guid, LPVOID* fn);

	/**
	* @brief 소켓을 생성
	*/
	static SOCKET CreateSocket();

	/**
	* @brief 소켓을 지정된 주소에 바인딩
	*/
	static bool Bind(SOCKET socket, NetAddress netAddr);

	/**
	* @brief 소켓을 지정된 IP, 포트에 바인딩
	*/
	static bool BindAnyAddress(SOCKET socket, uint16 port);

	/**
	* @brief 소켓을 리스닝 상태로 설정
	*/
	static bool Listen(SOCKET socket, int32 backlog = SOMAXCONN);
	static void Close(SOCKET& socket);
	
	/**
	* @brief 소켓에 대한 다양한 옵션을 설정
	*/
	static bool SetLinger(SOCKET socket, uint16 onoff, uint16 linger);
	static bool SetReuseAddress(SOCKET socket, bool flag);
	static bool SetRecvBufferSize(SOCKET socket, int32 size);
	static bool SetSendBufferSize(SOCKET socket, int32 size);
	static bool SetTcpNoDelay(SOCKET socket, bool flag);
	static bool SetUpdateAcceptSocket(SOCKET socket, SOCKET listenSocket);
};

template<typename T>
static inline bool SetSockOpt(SOCKET socket, int32 level, int32 optName, T optVal)
{
	return SOCKET_ERROR != ::setsockopt(socket, level, optName, reinterpret_cast<char*>(&optVal), sizeof(T));
}

