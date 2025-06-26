#pragma once

/*=======================================
			DBConnectionPool
=======================================*/

#include "DBConnection.h"

class DBConnectionPool
{
public:
	DBConnectionPool();
	~DBConnectionPool();

public:
	/**
	* @brief 전체 연결 문자열을 이용해 DB 커넥션 풀 초기화
	* @param connectionCount 생성할 커넥션 개수
	* @param connectionString ODBC 전체 연결 문자열
	* @return 모든 커넥션 생성 성공 시 true 반환
	*/
	bool					Connect(const int32 connectionCount, const WCHAR* connectionString);

	/**
	* @brief 개별 연결 정보를 이용해 DB 커넥션 풀 초기화
	* @param connectionCount 생성할 커넥션 개수
	* @param driver 드라이버 이름
	* @param server 서버 주소
	* @param dbName 데이터베이스 이름
	* @param uid 사용자 ID
	* @param pwd 사용자 비밀번호
	* @return 모든 커넥션 생성 성공 시 true 반환
	*/
	bool					Connect(const int32 connectionCount, const WCHAR* driver, const WCHAR* server, const WCHAR* dbName, const WCHAR* uid, const WCHAR* pwd);
	void					Clear();

	/**
	* @brief 풀에서 사용 가능한 DB 커넥션 획득
	* @return 사용 가능한 DBConnection 포인터 만약 없다면 nullptr 반환 가능
	*/
	DBConnection*			Pop();

	/**
	* @brief 사용한 DB 커넥션을 풀에 반납
	* @param connection 반납할 DBConnection 포인터
	*/
	void					Push(DBConnection* connection);

private:
	USE_LOCK;

	// DB 환경설정
	SQLHENV					_environment{ SQL_NULL_HANDLE };
	vector<DBConnection*>	_connections;
};

