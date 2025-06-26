#pragma once

/*====================================
			Session
====================================*/

#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetAddress.h"
#include "RecvBuffer.h"

class Service;
class IocpEvent;

/**
* @brief Session
* @details Session은 클라이언트와의 연결을 관리하고 데이터를 송수신하는 역할을 수행
*/
class Session : public IocpObject
{
	friend class Listener;
	friend class IocpCore;
	friend class Service;

	enum
	{
		BUFFER_SIZE = 0x10000, // 64KB
	};

public:
	Session();
	virtual ~Session();

public:
	/**
	* @brief 데이터를 송신 등록
	* @details SendBufferRef를 통해 송신할 데이터를 등록하고 IOCP 이벤트를 통해 비동기적으로 송신
	*/
	void Send(SendBufferRef sendBuffer);
	
	/**
	* @brief 세션 연결
	* @details IOCP이벤트를 통해 연결 이벤트를 등록
	*/
	bool Connect();

	/**
	* @brief 세션 연결 해제
	* @details 연결 해제 이벤트를 등록하고 세션을 종료
	*/
	void Disconnect(const WCHAR* cause);

	shared_ptr<Service> GetService() { return _service.lock(); }
	void SetService(shared_ptr<Service> service){ _service = service;}

public:
	/** 세션 정보 관련 */
	void SetNetAddress(NetAddress address){	_netAddress = address;}
	NetAddress GetAddress() const {	return _netAddress;	}
	SOCKET GetSocket() const { return _socket; }
	bool IsConnected() const { return _connected; }
	SessionRef GetSessionRef(){	return static_pointer_cast<Session>(shared_from_this()); }

private:
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(IocpEvent* iocpEvent, const int32 numOfBytes = 0) override;

private:
	/**IOCP 이벤트 등록된 함수*/
	bool RegisterConnect();
	bool RegisterDisconnect();
	void RegisterRecv();
	void RegisterSend();

	/**이벤트에 따른 실행 함수*/
	void ProcessConnect();
	void ProcessDisconnect();
	void ProcessRecv(const int32 numOfBytes);
	void ProcessSend(const int32 numOfBytes);

	void HandleError(const int32 errorCode);

protected:
	/** 이벤트에 따른 실행 함수 컨텐츠 코드에서 오버라이딩 */
	virtual void OnConnected() = 0;
	virtual int32 OnRecv(BYTE* buffer, int32 len) = 0;
	virtual void OnSend(const int32 len) = 0;
	virtual void OnDisconnected() = 0;
	
private:
	weak_ptr<Service> _service;
	SOCKET _socket{ INVALID_SOCKET };
	NetAddress _netAddress{};
	Atomic<bool> _connected{};

private:
	USE_LOCK;
	RecvBuffer _recvBuffer;
	Queue<SendBufferRef> _sendQueue;
	Atomic<bool> _sendRegistered{};

private:
	ConnectEvent	_connectEvent;
	DisconnectEvent _disconnectEvent;
	RecvEvent		_recvEvent;
	SendEvent		_sendEvent;
};


/*====================================
			PacketSession
====================================*/

/**
* @brief 패킷 헤더 구조체
* @details 패킷 헤더는 패킷의 크기와 ID를 포함하여 패킷을 식별하는 데 사용
*/
struct PacketHeader
{
	uint16 size{};
	uint16 id{}; 
};

class PacketSession : public Session
{
public:
	PacketSession();
	virtual ~PacketSession();

	PacketSessionRef GetPacketSessionRef() { return static_pointer_cast<PacketSession>(shared_from_this());	}

protected:
	virtual int32 OnRecv(BYTE* buffer, const int32 len) sealed;
	virtual void OnRecvPacket(BYTE* buffer, const int32 len) abstract;
};