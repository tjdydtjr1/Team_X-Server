#pragma once

/*======================
	 GameRedisManager
======================*/

#include "macro/Define.h"
#include <cpp_redis/cpp_redis>


class GameRedisManager
{
	SINGLETON_BASE(GameRedisManager);

public:
	/**
	* @brief : RedisServer 연결 함수
	* @param : host = RedisServer IP
	* @param : port = RedisServer Port
	*/
	void Connect(const string& host = "127.0.0.1", const int port = 6379);
	void Connect(const wstring& host, const int port);

private:
	/**
	* @brief : RedisServer Key, Value 설정 함수
	* @param : key = Key
	* @param : value = Value
	* @param : expireTime = Expire Time
	*/
	void Set(const string& key, const string& value, const int expireTime = 0);

	/**
	* @brief : RedisServer Key, Value 값 가져오기 함수
	* @param : key = Key
	* @return : 키에 해당하는 값
	*/
	string Get(const string& key);

	/**
	* @brief : RedisServer Key, Value TTL 가져오기 함수
	* @param : key = Key
	* @return : TTL 값
	*/
	int GetTTL(const string& key);

	/**
	* @brief : RedisServer Key, Value 삭제 함수
	* @param : key = Key
	*/
	void Delete(const string& key);

	/**
	* @brief : RedisServer 연결 상태 설정 함수
	* @param : connect = 연결 상태 설정
	*/
	void SetIsConnected(const bool connect){ _isConnected = connect; }

	/**
	* @brief : RedisServer 연결 상태 확인 함수
	* @return : RedisServer 연결 상태
	*/
	bool GetIsConnected() const { return _isConnected; }

private:
	USE_LOCK;
	cpp_redis::client _client{};
	bool _isConnected{};
};

#define RedisManager GameRedisManager::GetInstance()
