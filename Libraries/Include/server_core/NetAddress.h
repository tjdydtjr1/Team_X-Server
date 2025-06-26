#pragma once

/*====================================
			NetAddress
====================================*/

class NetAddress
{
public:
	NetAddress() = default;
	~NetAddress() = default;
	NetAddress(const SOCKADDR_IN& sockAddr);
	NetAddress(const wstring& ip, const uint16 port);

public:
	/**
	* @brief 소켓 구조체를 반환
	*/
	SOCKADDR_IN&	GetSockAddr() { return _sockAddr; }

	/**
	* @brief 소켓 구조체에서 IP 주소를 문자열로 반환
	*/
	wstring			GetIpAddress() const;

	/**
	* @brief 소켓 구조체에서 Port번호 반환
	*/
	uint16			GetPort() const { return ::ntohs(_sockAddr.sin_port); }

public:
	/**
	* @brief Domain 주소를 IP 문자열로 변환
	*/
	static wstring DomainToIp(const wstring& domain);

private:
	static IN_ADDR	Ip2Address(const WCHAR* ip);

private:
	SOCKADDR_IN _sockAddr{};
};

