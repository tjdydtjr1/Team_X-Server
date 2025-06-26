#pragma once

/*==========================
		GlobalQueue
==========================*/

/**
* 쓰레드의 일감이 몰릴 경우 JobQueue 자체를 담아줄 GlobalQueue
*/
class GlobalQueue
{
public:
	GlobalQueue();
	~GlobalQueue();

	void Push(JobQueueRef jobQueue);
	JobQueueRef Pop();

private:
	LockQueue<JobQueueRef> _jobQueues;
};

