#pragma once

/*========================
		JobTimer
========================*/


struct JobData
{
	JobData(weak_ptr<JobQueue> owner, JobRef job)
		: owner(owner)
		, jobs(job)
	{
	}

	weak_ptr<JobQueue>  owner;
	JobRef				jobs;
};

struct TimerItem
{
	bool operator<(const TimerItem& other) const
	{
		return executeTick > other.executeTick;
	}

	uint64 executeTick{};
	JobData* jobData{ nullptr };
};

/**
* @brief 예약된 시간에 실행할 일감을 관리하는 클래스
* @details JobQueue에서 예약된 시간에 실행할 일감을 등록하고 JobTimer에서 분배하여 실행
*/
class JobTimer
{
public:
	void Reserve(const uint64 tickAfter, weak_ptr<JobQueue> owner, JobRef job);
	void Distribute(const uint64 now);
	void Clear();

private:
	USE_LOCK;
	PriorityQueue<TimerItem> _items{};
	Atomic<bool> _distributing{};

};

