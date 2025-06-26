#pragma once

/*=======================================
			RW SpinLock
=======================================*/

/**
* @brief Read-Write SpinLock
* [WWWWWWWW] [WWWWWWWW] [RRRRRRRR][RRRRRRRR]
* W: WriteFlag    (Exclusive Lock Owner ThreadId)
* R: ReadFlag		(Shared Lock Count)
*/

// Lock 방식
// W -> W (O)
// W -> R (O)
// R -> W (X)

/**
* @brief Read-Write SpinLock
* @details Read-Write SpinLock은 다수의 Reader가 동시에 접근할 수 있도록 하면서 Writer가 접근할 때는 다른 Reader와 Writer가 접근하지 못하도록 하는 Lock
*/
class Lock
{
	enum : uint32
	{
		ACQUIRE_TIMEOUT_TICK = 10000,
		MAX_SPIN_COUNT		 = 5000,
		WRITE_THREAD_MASK	 = 0xFFFF'0000,
		READ_COUNT_MASK		 = 0x0000'FFFF,
		EMPTY_FLAG			 = 0x0000'0000
	};

public:
	void WriteLock(const char* name);
	void WriteUnlock(const char* name);
	void ReadLock(const char* name);
	void ReadUnlock(const char* name);

private:
	Atomic<uint32> _lockFlag = EMPTY_FLAG;
	uint16 _writeCount = 0;

};


/*=======================================
			LockGuards
=======================================*/

class ReadLockGuard
{
public:
	ReadLockGuard(Lock& lock, const char* name)
		: _lock(lock)
		, _name(name)
	{
		_lock.ReadLock(_name);
	}

	~ReadLockGuard()
	{
		_lock.ReadUnlock(_name);
	}

private:
	Lock& _lock;
	const char* _name;
};

class WriteLockGuard
{
public:
	WriteLockGuard(Lock& lock, const char* name)
		: _lock(lock)
		, _name(name)
	{
		_lock.WriteLock(_name);
	}

	~WriteLockGuard()
	{
		_lock.WriteUnlock(_name);
	}

private:
	Lock& _lock;
	const char* _name;
};