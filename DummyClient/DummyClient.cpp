#include "pch.h"
#include "ServerPacketHandler.h"
#include "ServerSession.h"
#include <chrono>

static void DomainToIp(std::wstring& domain)
{
    addrinfo hints{};
	addrinfo* res{};

    ::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

	std::string domainStr{ StringUtils::WstringToString(domain) };
	const int ret{ getaddrinfo(domainStr.c_str(), nullptr, &hints, &res) };

    if (ret != 0 || res == nullptr)
    {
        return;
    }

	char ipStr[INET_ADDRSTRLEN]{};
	void* addrPtr{ &( (sockaddr_in*)res->ai_addr )->sin_addr };

    if (inet_ntop(AF_INET, addrPtr, ipStr, sizeof(ipStr)) == nullptr)
    {
        freeaddrinfo(res);

        return;
    }

	std::string temp{ ipStr };
    domain = StringUtils::StringToWstring(temp);

    freeaddrinfo(res);
}


int main()
{
	ServerPacketHandler::Init();

	wstring domain{ L"team-x.duckdns.org" };

	DomainToIp(domain);

	uint32 port{ 9414 };
	wcout << domain << endl;

	ClientServiceRef service = MakeShared<ClientService>(
		NetAddress(domain, port),
		MakeShared<IocpCore>(),
		MakeShared<ServerSession>, 
		1);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 1; ++i)
	{
		GThreadManager->Launch([=]()
		{
			while (true)
			{
				service->GetIocpCore()->Dispatch();
			}
		}); 
	}

	GThreadManager->Join();
}