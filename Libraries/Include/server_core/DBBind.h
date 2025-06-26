#pragma once

/*====================================
			DBBind
======================================*/

#include "DBConnection.h"

template<int32 C>
struct FullBits { enum { value = (1 << (C - 1)) | FullBits<C - 1>::value }; };

template<>
struct FullBits<1> { enum { value = 1 }; };

template<>
struct FullBits<0> { enum { value = 0 }; };

template<int32 ParamCount, int32 ColumnCount>
class DBBind
{
public:
	DBBind(DBConnection& dbConnection, const WCHAR* query)
		: _dbConnection(dbConnection), _query(query)
	{
		::memset(_paramIndex, 0, sizeof(_paramIndex));
		::memset(_columnIndex, 0, sizeof(_columnIndex));
		dbConnection.Unbind();
	}

	/**
	* @brief 모든 파라미터와 컬럼이 바인딩되었는지 검증
	* @return 모든 항목이 바인딩 되었을 시 true 반환
	*/
	bool Validate()
	{
		return _paramFlag == FullBits<ParamCount>::value && _columnFlag == FullBits<ColumnCount>::value;
	}

	/**
	* @brief 바인딩된 쿼리 실행
	* @details 실행 전 모든 바인딩이 완료되었는지 ASSERT로 확인
	* @return 쿼리 실행 성공 시 true 반환
	*/
	bool Execute()
	{
		ASSERT_CRASH(Validate());
		return _dbConnection.Execute(_query);
	}

	/**
	* @brief 결과에서 다음 행 인출
	* @return 다음 행이 존재할 시 true 반환
	*/
	bool Fetch()
	{
		return _dbConnection.Fetch();
	}

public:
	/**
	* @brief 입력 파라미터 바인딩 일반 타입
	* @param idx 파라미터 번호 1부터 시작
	* @param value 바인딩할 변수의 참조
	*/
	template<typename T>
	void BindParam(int32 idx, T& value)
	{
		_dbConnection.BindParam(idx + 1, &value, &_paramIndex[idx]);
		_paramFlag |= (1LL << idx);
	}

	/**
	* @brief 입력 파라미터 바인딩 문자열
	* @param idx 파라미터 번호 1부터 시작
	* @param value 바인딩할 WCHAR 문자열
	*/
	void BindParam(int32 idx, const WCHAR* value)
	{
		_dbConnection.BindParam(idx + 1, value, &_paramIndex[idx]);
		_paramFlag |= (1LL << idx);
	}

	/**
	* @brief 입력 파라미터 바인딩 바이너리 데이터, 고정 배열
	* @param idx 파라미터 번호 1부터 시작
	* @param value 바인딩할 데이터 배열
	*/
	template<typename T, int32 N>
	void BindParam(int32 idx, T(&value)[N])
	{
		_dbConnection.BindParam(idx + 1, (const BYTE*)value, size32(T) * N, &_paramIndex[idx]);
		_paramFlag |= (1LL << idx);
	}

	/**
	* @brief 입력 파라미터 바인딩 바이너리 데이터 포인터
	* @param idx 파라미터 번호 1부터 시작
	* @param value 바인딩할 데이터의 시작 주소
	* @param N 데이터의 크기 바이트
	*/
	template<typename T>
	void BindParam(int32 idx, T* value, int32 N)
	{
		_dbConnection.BindParam(idx + 1, (const BYTE*)value, size32(T) * N, &_paramIndex[idx]);
		_paramFlag |= (1LL << idx);
	}

public:
	/**
	* @brief 출력 컬럼 바인딩 일반 타입
	* @param idx 컬럼 번호 1부터 시작
	* @param value 결과를 저장할 변수의 참조
	*/
	template<typename T>
	void BindCol(int32 idx, T& value)
	{
		_dbConnection.BindCol(idx + 1, &value, &_columnIndex[idx]);
		_columnFlag |= (1LL << idx);
	}

	/**
	* @brief 출력 컬럼 바인딩 문자열, 고정 배열
	* @param idx 컬럼 번호 1부터 시작
	* @param value 결과를 저장할 WCHAR 배열
	*/
	template<int32 N>
	void BindCol(int32 idx, WCHAR(&value)[N])
	{
		_dbConnection.BindCol(idx + 1, value, N - 1, &_columnIndex[idx]);
		_columnFlag |= (1LL << idx);
	}

	/**
	* @brief 출력 컬럼 바인딩 문자열, 포인터
	* @param idx 컬럼 번호 1부터 시작
	* @param value 결과를 저장할 WCHAR 버퍼의 주소
	* @param len 버퍼의 최대 길이
	*/
	void BindCol(int32 idx, WCHAR* value, int32 len)
	{
		_dbConnection.BindCol(idx + 1, value, len - 1, &_columnIndex[idx]);
		_columnFlag |= (1LL << idx);
	}

	/**
	* @brief 출력 컬럼 바인딩 바이너리 데이터, 고정 배열
	* @tparam N 배열의 크기
	* @param idx 컬럼 번호 1부터 시작
	* @param value 결과를 저장할 데이터 배열
	*/
	template<typename T, int32 N>
	void BindCol(int32 idx, T(&value)[N])
	{
		_dbConnection.BindCol(idx + 1, value, size32(T) * N, &_columnIndex[idx]);
		_columnFlag |= (1LL << idx);
	}

protected:
	DBConnection&	_dbConnection;
	const WCHAR*	_query;
	SQLLEN			_paramIndex[ParamCount > 0 ? ParamCount : 1];
	SQLLEN			_columnIndex[ColumnCount > 0 ? ColumnCount : 1];
	uint64			_paramFlag{};
	uint64			_columnFlag{};
};

