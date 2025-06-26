#pragma once

/*=====================
	GLobal Instance
=====================*/

extern class ThreadManager*		GThreadManager;

extern class GlobalQueue*		GGlobalQueue;

extern class JobTimer*			GJobTimer;

extern class DeadLockProfiler*	GDeadLockProfiler;

extern class Memory*			GMemory;

extern class SendBufferManager* GSendBufferManager;

extern class DBConnectionPool*	GDBConnectionPool;

extern class ConsoleLog*		GConsoleLogger;