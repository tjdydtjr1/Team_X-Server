#pragma once

/*======================
		JobQueue
========================*/

#include "job.h"
#include "LockQueue.h"
#include "JobTimer.h"

class JobQueue : public enable_shared_from_this<JobQueue>
{
public:
	/**
	* @brief Worker Thread에서 실행할 일감 등록
	* @details 일감은 shared_ptr<Job>로 관리 GlobalQueue에 등록되어 분배되어 실행
	*/
	void PushJob(CallbackType&& callback)
	{
		Push(ObjectPool<Job>::MakeShared(std::move(callback)));
	}

	/**
	* @brief Worker Thread에서 실행할 일감 등록
	* @details 멤버 함수 포인터를 이용하여 일감 큐에 등록
	*/
	template<typename T, typename Ret, typename... Args>
	void PushJob(Ret(T::* memFunc)(Args...), Args... args)
	{
		shared_ptr<T> owner{ static_pointer_cast<T>( shared_from_this() ) };
		Push(ObjectPool<Job>::MakeShared(owner, memFunc, std::forward<Args>(args)...));
	}
	 
public:
	/**
	* @brief 예약된 시간에 실행할 일감 등록
	* @details 예약된 시간에 실행할 일감은 GlobalQueue에서 JobTimer에 등록되어 분배되어 실행
	*/
	void PushTimerJob(const uint64 tickMS, CallbackType&& callback)
	{
		JobRef job{ ObjectPool<Job>::MakeShared(std::move(callback)) };
		GJobTimer->Reserve(tickMS, shared_from_this(), job);
	}

	/**
	* @brief 예약된 시간에 실행할 일감 등록
	* @details 멤버 함수 포인터를 이용하여 예약된 시간에 실행할 일감 등록
	*/
	template<typename T, typename Ret, typename... Args>
	void PushTimerJob(const uint64 tickMS, Ret(T::* memFunc)(Args...), Args... args)
	{
		shared_ptr<T> owner{ static_pointer_cast<T>( shared_from_this() ) };
		JobRef job{ ObjectPool<Job>::MakeShared(owner, memFunc, std::forward<Args>(args)...) };
		GJobTimer->Reserve(tickMS, shared_from_this(), job);
	}

	void ClearJobs()
	{
		_jobs.Clear();
	}

public:
	void Execute();
	void Push(JobRef job);

protected:
	LockQueue<JobRef> _jobs;
	Atomic<int32> _jobCount{};
};

