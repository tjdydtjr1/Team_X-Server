#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#ifdef _DEBUG
#pragma comment(lib, "ServerCore\\Debug\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#pragma comment(lib, "OpenSSL\\Debug\\libssl.lib")
#pragma comment(lib, "OpenSSL\\Debug\\libcrypto.lib")
#pragma comment(lib, "Redis\\Debug\\cpp_redis.lib")
#pragma comment(lib, "tacopie\\Debug\\tacopie.lib")
#else
#pragma comment(lib, "ServerCore\\Release\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib")
#pragma comment(lib, "OpenSSL\\Release\\libssl.lib")
#pragma comment(lib, "OpenSSL\\Release\\libcrypto.lib")
#pragma comment(lib, "Redis\\Release\\cpp_redis.lib")
#pragma comment(lib, "tacopie\\Release\\tacopie.lib")
#endif

#include <server_core/CorePch.h>
#include "Protocol.pb.h"

#include "ServerPacketHandler.h"

#include "StringUtils.h"