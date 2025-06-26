#pragma once

/*====================================
			Service
====================================*/

/*
클라이언트	- 서버
서버			- 서버
*/

#include "NetAddress.h"
#include "IocpCore.h"
#include "Listener.h"
#include <functional>

enum class ServiceType : uint8
{
	SERVER,
	CLIENT
};

// session을 만들 함수
using SessionFactory = function<SessionRef(void)>;

class Service : public enable_shared_from_this<Service>
{
public:
	Service(ServiceType type, NetAddress address,
		IocpCoreRef core, SessionFactory factory, 
		int32 maxSessionCount = 1);

	virtual ~Service();

public:
	virtual bool Start() = 0;
	virtual void CloseService();

public:
	bool CanStart() const { return _sessionFactory != nullptr; }

	void SetSessionFactory(SessionFactory func)	{ _sessionFactory = func; }

	void Broadcast(SendBufferRef sendBuffer);
	SessionRef CreateSession();
	void AddSession(SessionRef session);
	void ReleaseSession(SessionRef session);

public:
	int32 GetCurrentSessionCount() const { return _sessionCount;}
	int32 GetMaxSessionCount() const { return _maxSessionCount;	}
	ServiceType GetServiceType() const { return _type; }
	NetAddress GetNetAddress() const { return _netAddress; }
	IocpCoreRef& GetIocpCore() { return _iocpCore; }

protected:
	USE_LOCK;

	ServiceType				_type{};
	NetAddress				_netAddress{};
	IocpCoreRef				_iocpCore{};

	Set<SessionRef>			_sessions;
	int32					_sessionCount{};
	int32					_maxSessionCount{};
	SessionFactory			_sessionFactory{};
};


/*====================================
			ClientService
====================================*/

class ClientService : public Service
{
public:
	ClientService(NetAddress targetAddress, IocpCoreRef core
		, SessionFactory factory, int32 maxSessionCount = 1);
	virtual ~ClientService() {}

public:
	virtual bool Start() override;

};


/*====================================
			ServerService
====================================*/

class ServerService : public Service
{
public:
	ServerService(NetAddress address, IocpCoreRef core
		, SessionFactory factory, int32 maxSessionCount = 1);
	virtual ~ServerService() {}

public:
	virtual bool Start() override;
	virtual void CloseService() override;
	
private:
	ListenerRef _listener{};
};