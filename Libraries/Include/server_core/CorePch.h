#pragma once

#include "Types.h"
#include "CoreMacro.h"
#include "CoreTLS.h"
#include "CoreGlobal.h"
#include "Container.h"

#include <windows.h>
#include <iostream>

#include <WinSock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#include "ThreadManager.h"
#include "Lock.h"
#include "ObjectPool.h"
#include "TypeCast.h"
#include "Memory.h"
#include "Service.h"
#include "SendBuffer.h"
#include "Session.h"
#include "JobQueue.h"
#include "DBConnectionPool.h"
#include "DBBind.h"
#include "ConsoleLog.h"