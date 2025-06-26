#pragma once

/*=======================================
			DeadLockProfiler
=======================================*/

#include <stack>
#include <map>
#include <vector>

/**
* @brief DeadLock를 검사하는 프로파일러
* @details DeadLockProfiler는 그래프를 활용 쓰레드 간의 Lock 획득 순서를 기록하고 순환 참조가 있는지 검사하여 DeadLock을 탐지
*/

class DeadLockProfiler
{
public:
	void PushLock(const char* name);
	void PopLock(const char* name);
	void CheckCycle();

private:
	void Dfs(int32 here);

private:
	unordered_map<const char*, int64> _nameToId;
	unordered_map<int32, const char*> _idToName;
	map<int32, set<int32>>			  _lockHistory;

	Mutex							  _lock;

private:
	// 노드가 발견된 순서를 기록하는 배열
	vector<int32>					  _discoveredOrder;

	// 노드가 발견된 순서
	int32  							  _discoveredCount{};

	// Dfs(i)가 종료 되었는지 여부
	vector<bool>					 _finished;
	vector<int32>					 _parent;
};

