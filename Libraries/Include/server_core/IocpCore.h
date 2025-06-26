#pragma once

/*=======================================
			IocpObject
=======================================*/

/** IOCP 흐름
* 1. CrateIoCompletionPort 만들기
* 2. CrateIoCompletionPort 등록
* 3. Dispatch 감시
* 4. 실행
*/

class IocpEvent;

class IocpObject : public enable_shared_from_this<IocpObject>
{
public:
	virtual HANDLE GetHandle() = 0;
	virtual void Dispatch(IocpEvent* iocpEvent, int32 numOfBytes = 0) = 0;
};

/*=======================================
			IocpCore
=======================================*/

/**
* @brief IOCP Core
* @details IOCP Core는 IOCP 이벤트를 관리하고 분배하는 역할을 수행
*/
class IocpCore
{
public:
	IocpCore();
	~IocpCore();

public:
	HANDLE GetHandle() const { return _iocpHandle; }

public:
	bool Register(IocpObjectRef iocpObject) const;
	bool Dispatch(const uint32 timeoutMs = INFINITE) const;

private:
	HANDLE _iocpHandle{};
};
