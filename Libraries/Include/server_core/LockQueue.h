#pragma once

/*======================
		LockQueue
========================*/

/**
* @brief Concurrent Queue
* @details LockQueue는 멀티 쓰레드 환경에서 안전하게 데이터를 추가하고 제거할 수 있는 큐
*/
template<typename T>
class LockQueue
{
public:
	void Push(T item)
	{
		WRITE_LOCK;
		_items.push(item);
	}

	T Pop()
	{
		WRITE_LOCK;
		if (_items.empty())
		{
			return nullptr;
		}

		T ret = _items.front();
		_items.pop();
		return ret;
	}

	void Clear()
	{
		WRITE_LOCK;
		_items = Queue<T>();
	}

	void PopAll(OUT Vector<T>& items)
	{
		WRITE_LOCK;
		while (T item = Pop())
		{
			items.push_back(item);
		}
	}

private:
	USE_LOCK;
	Queue<T> _items;
};