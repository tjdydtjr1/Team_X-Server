#pragma once

#include <thread>
#include <functional>

/*===========================
		ThreadManager
=============================*/

/**
* @brief 쓰레드 관리 클래스
* @details 네트워크IO, Global Job의 쓰레드 간의 작업 분배를 담당
*/
class ThreadManager
{
public:
	ThreadManager();
	~ThreadManager();

	void Launch(function<void(void)> callback);
	void Join();

	static void InitTLS();
	static void DestroyTLS();

	static void DoGlobalQueueWork();
	static void DistributeReserveJob();

private:
	Mutex				_lock;
	vector<thread>		_threads;
};

