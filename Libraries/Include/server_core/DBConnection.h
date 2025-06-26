#pragma once

/*=======================================
			DBConnection
=======================================*/

#include <sql.h>
#include <sqlext.h>

enum
{
	WVARCHAR_MAX = 4000,
	BINARY_MAX = 8000
};

class DBConnection
{
public:
	/**
	* @brief 데이터베이스 연결
	* @param henv ODBC 환경 핸들
	* @param connectionString 연결 정보 문자열
	* @return 연결 성공 시 true 반환
	*/
	bool			Connect(SQLHENV henv, const WCHAR* connectionString);
	void			Clear();

	/**
	* @brief SQL 쿼리 실행
	* @details 파라미터 바인딩이 필요 없는 간단한 쿼리에 사용
	* @param query 실행할 SQL 쿼리문
	* @return 실행 성공 시 true 반환
	*/
	bool			Execute(const WCHAR* query);

	/**
	* @brief 결과에서 다음 행 인출
	* @return 다음 행이 존재하여 인출 성공 시 true 반환
	*/
	bool			Fetch();

	/**
	* @brief 쿼리 실행 후 조회 된 행의 수 조회
	* @return 행의 수
	*/
	int32			GetRowCount() const;
	void			Unbind() const;

public:
	/**@brief 입력 파라미터 바인딩*/
	bool			BindParam(int32 paramIndex, bool* value, SQLLEN* index);
	bool			BindParam(int32 paramIndex, float* value, SQLLEN* index);
	bool			BindParam(int32 paramIndex, double* value, SQLLEN* index);
	bool			BindParam(int32 paramIndex, int8* value, SQLLEN* index);
	bool			BindParam(int32 paramIndex, int16* value, SQLLEN* index);
	bool			BindParam(int32 paramIndex, int32* value, SQLLEN* index);
	bool			BindParam(int32 paramIndex, int64* value, SQLLEN* index);
	bool			BindParam(int32 paramIndex, TIMESTAMP_STRUCT* value, SQLLEN* index);
	bool			BindParam(int32 paramIndex, const WCHAR* str, SQLLEN* index);
	bool			BindParam(int32 paramIndex, const BYTE* bin, int32 size, SQLLEN* index);

	/**@brief 출력 컬럼 바인딩*/
	bool			BindCol(int32 columnIndex, bool* value, SQLLEN* index);
	bool			BindCol(int32 columnIndex, float* value, SQLLEN* index);
	bool			BindCol(int32 columnIndex, double* value, SQLLEN* index);
	bool			BindCol(int32 columnIndex, int8* value, SQLLEN* index);
	bool			BindCol(int32 columnIndex, int16* value, SQLLEN* index);
	bool			BindCol(int32 columnIndex, int32* value, SQLLEN* index);
	bool			BindCol(int32 columnIndex, int64* value, SQLLEN* index);
	bool			BindCol(int32 columnIndex, TIMESTAMP_STRUCT* value, SQLLEN* index);
	bool			BindCol(int32 columnIndex, WCHAR* str, int32 size, SQLLEN* index);
	bool			BindCol(int32 columnIndex, BYTE* bin, int32 size, SQLLEN* index);

private:
	/**
	* @brief ODBC SQLBindParameter 함수 래퍼
	* @details 모든 public BindParam 오버로드의 실제 구현부
	* @param paramIndex 파라미터 번호 1부터 시작
	* @param cType C 데이터 타입
	* @param sqlType SQL 데이터 타입
	* @param len 데이터의 길이 바이트
	* @param ptr 바인딩할 데이터의 주소
	* @param index 데이터 길이/NULL 여부 표시자 주소
	* @return 바인딩 성공 시 true 반환
	*/
	bool			BindParam(SQLUSMALLINT paramIndex, SQLSMALLINT cType, SQLSMALLINT sqlType, SQLULEN len, SQLPOINTER ptr, SQLLEN* index);
	
	/**
	* @brief ODBC SQLBindCol 함수 래퍼
	* @details 모든 public BindCol 오버로드의 실제 구현부
	* @param columnIndex 컬럼 번호 1부터 시작
	* @param cType C 데이터 타입
	* @param len 데이터를 저장할 버퍼의 길이
	* @param value 데이터를 저장할 버퍼의 주소
	* @param index 데이터 길이/NULL 여부 표시자 주소
	* @return 바인딩 성공 시 true 반환
	*/
	bool			BindCol(SQLUSMALLINT columnIndex, SQLSMALLINT cType, SQLULEN len, SQLPOINTER value, SQLLEN* index);

	/**
	* @brief ODBC API 호출의 에러 처리
	* @param ret ODBC API가 반환한 SQLRETURN 코드
	*/
	void			HandleError(const SQLRETURN ret) const;

private:
	// DBConnection Handle
	SQLHDBC			_connection{ SQL_NULL_HANDLE };

	// DB 상태 관리
	SQLHSTMT		_statement{ SQL_NULL_HANDLE };
};