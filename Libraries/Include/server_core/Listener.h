#pragma once

#include "IocpCore.h"
#include "NetAddress.h"

/*=======================================
				Listener
=======================================*/

class AcceptEvent;
class IocpEvent;
class ServerService;

/**
* @brief Listener 클래스
* @details Listener는 클라이언트의 연결 요청을 수신하고 AcceptEvent를 통해 세션을 생성하는 역할을 수행
*/
class Listener : public IocpObject
{
public:
	Listener() = default;
	~Listener();

public:
	/**
	* @brief Listener를 시작하고 소켓을 바인딩 및 옵션 설정 후 클라이언트의 연결 요청을 수신
	*/
	bool StartAccept(ServerServiceRef netAddress);
	void CloseSocket();

public:
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
	void RegisterAccept(AcceptEvent* acceptEvent);
	void ProcessAccept(AcceptEvent* acceptEvent);

protected:
	SOCKET _socket{ INVALID_SOCKET };
	Vector<AcceptEvent*> _acceptEvents;
	ServerServiceRef _service{};
};

