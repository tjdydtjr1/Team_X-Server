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

// Base Core
#include <server_core/CorePch.h>

#include "macro/Define.h"
#include "dump/Dump.h"
#include "monitoring/Monitoring.h"
#include "math/Vector.h"
#include "crypto/CryptoManager.h"
#include "utils/MathUtils.h"
#include "utils/StringUtils.h"
#include "utils/TimeUtils.h"
#include "parser/ConfigLoader.h"
#include "logger/Logger.h"
#include "common/utils/SparseSet.h"

// Network
#include "network/protobuf/Protocol.pb.h"
#include "network/packet/ClientPacketHandler.h"

// Std
#include <optional>


// Macro
#define SEND_PACKET(pkt)												\
SendBufferRef sendBuffer { ClientPacketHandler::MakeSendBuffer(pkt) };  \
session->Send(sendBuffer)

USING_SHARED_PTR(GameObject)
USING_SHARED_PTR(GameSession)
USING_SHARED_PTR(Player)
USING_SHARED_PTR(GameMap)
