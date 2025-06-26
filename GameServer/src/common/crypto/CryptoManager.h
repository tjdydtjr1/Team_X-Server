#pragma once

/*=============================
		CryptoManager
=============================*/

#include <openssl/evp.h>
#include <string>

/**
* @brief : 쓰레드 세이프 OpenSSL
*/
class CryptoManager
{
public:
	/**
	* @brief : CryptoManager 초기화 함수
	* @detail : OpenSSL 초기화 및 알고리즘 초기화 최초 1회 실행 필요
	*/
	static void Init();

	/**
	* @brief : CryptoManager 정리 함수
	*/
	static void CleanUp();

	/*Encrypt Sha*/
	/**
	* @brief : SHA256 해시 함수
	* @param : data = 해시할 데이터 문자열
	* @param : hash = 해시 결과를 담을 문자열
	* @return : 성공여부
	*/
	static bool Sha256Encrypt(const char* data, unsigned char* hash);
	static bool Sha256Encrypt(const std::string& data, std::string& hash);
	static bool Sha256Encrypt(const std::wstring& data, std::wstring& hash);

	/**
	* @brief : SHA256 해시 검증 함수
	* @param : data = 해시할 데이터 문자열
	* @param : hash = 해시 결과를 담을 문자열
	* @return : 성공여부
	*/
	static bool Sha256Verify(const char* data, const unsigned char* hash);
	static bool Sha256Verify(const std::string& data, const std::string& hash);
	static bool Sha256Verify(const std::wstring& data, const std::wstring& hash);

	/**
	* @brief : SHA512 해시 함수
	* @param : data = 해시할 데이터 문자열
	* @param : hash = 해시 결과를 담을 문자열
	* @return : 성공여부
	*/
	static bool Sha512Encrypt(const char* data, unsigned char* hash);
	static bool Sha512Encrypt(const std::string& data, std::string& hash);
	static bool Sha512Encrypt(const std::wstring& data, std::wstring& hash);

	/**
	* @brief : SHA512 해시 검증 함수
	* @param : data = 해시할 데이터 문자열
	* @param : hash = 해시 결과를 담을 문자열
	* @return : 성공여부
	*/
	static bool Sha512Verify(const char* data, const unsigned char* hash);
	static bool Sha512Verify(const std::string& data, const std::string& hash);
	static bool Sha512Verify(const std::wstring& data, const std::wstring& hash);


	/**Encrypt Aes*/
	/**
	* @brief : AES 암호화 함수
	* @param : plaintext = 평문
	* @param : plaintextLen = 평문 길이
	* @param : key = 암호화 키
	* @param : ciphertext = 암호문
	* @return : 성공여부
	*/
	static bool Aes256Encrypt(const unsigned char* plaintext, int plaintextLen, const unsigned char* key, const unsigned char* iv, unsigned char* ciphertext);
	static bool Aes256Encrypt(const std::string& plaintext, const std::string& key, std::string& ciphertext);
	static bool Aes256Encrypt(const std::wstring& plaintext, const std::wstring& key, std::wstring& ciphertext);

	/**
	* @brief : AES 복호화 함수
	* @param : ciphertext = 암호문
	* @param : ciphertextLen = 암호문 길이
	* @param : key = 복호화 키
	* @param : plaintext = 평문
	* @return : 성공여부
	*/
	static bool Aes256Decrypt(const unsigned char* ciphertext, int ciphertextLen, const unsigned char* key, const unsigned char* iv, unsigned char* plaintext);
	static bool Aes256Decrypt(const std::string& ciphertext, const std::string& key, std::string& plaintext);
	static bool Aes256Decrypt(const std::wstring& ciphertext, const std::wstring& key, std::wstring& plaintext);

public:
	static thread_local EVP_MD_CTX* LMdctx;
	static thread_local EVP_CIPHER_CTX* LCipherctx;
	static std::once_flag _initFlag;
};




