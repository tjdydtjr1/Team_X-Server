#pragma once

/*====================================
			IocpEvent
====================================*/

class Session;

enum class EventType : uint8
{
	CONNECT,
	ACCEPT,
	RECV,
	SEND,
	DISCONNECT
};

/**
* @brief IOCP 이벤트 클래스
* @details IOCP 이벤트는 OVERLAPPED 구조체를 상속받아 IOCP 이벤트를 처리하는데 사용
*/
class IocpEvent : public OVERLAPPED
{
public:
	IocpEvent(const EventType type);

public:
	void Init();

public:
	EventType _eventType{};
	IocpObjectRef _owner{};
};


/*====================================
			ConnectEvent
====================================*/

class ConnectEvent : public IocpEvent
{
public:
	ConnectEvent() : IocpEvent(EventType::CONNECT) {}
};

/*====================================
			DisconnectEvent
====================================*/

class DisconnectEvent : public IocpEvent
{
public:
	DisconnectEvent() : IocpEvent(EventType::DISCONNECT) {}
};


/*====================================
			AcceptEvent
====================================*/

class AcceptEvent : public IocpEvent
{
public:
	AcceptEvent() : IocpEvent(EventType::ACCEPT) {}

public:
	SessionRef _session{};
};

/*====================================
			RecvEvent
====================================*/

class RecvEvent : public IocpEvent
{
public:
	RecvEvent() : IocpEvent(EventType::RECV) {}
};

/*====================================
			SendEvent
====================================*/

class SendEvent : public IocpEvent
{
public:  
	SendEvent() : IocpEvent(EventType::SEND) {}

	Vector<SendBufferRef> _sendBuffers;
};