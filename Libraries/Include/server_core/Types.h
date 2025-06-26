#pragma once

#include <mutex>
#include <atomic>

#ifdef _MSC_VER  // MSVC 컴파일러에서만 타입 사용
using BYTE		= unsigned char;
using int8		= __int8;
using int16		= __int16;
using int32		= __int32;
using int64		= __int64;

using uint8		= unsigned __int8;
using uint16	= unsigned __int16;
using uint32	= unsigned __int32;
using uint64	= unsigned __int64;
#else  // 다른 플랫폼에서는 표준 타입 사용
#include <cstdint>
using BYTE		= uint8_t;
using int8		= int8_t;
using int16		= int16_t;
using int32		= int32_t;
using int64		= int64_t;

using uint8		= uint8_t;
using uint16	= uint16_t;
using uint32	= uint32_t;
using uint64	= uint64_t;
#endif

template<typename T>
using Atomic				= std::atomic<T>;
using Mutex					= std::mutex;
using CondVar				= std::condition_variable;
using UniqueLock			= std::unique_lock<std::mutex>;
using LockGuard				= std::lock_guard<std::mutex>;

// using_shared_ptr
#define USING_SHARED_PTR(name)  using name##Ref = std::shared_ptr<class name>;

USING_SHARED_PTR(IocpCore);
USING_SHARED_PTR(IocpObject);
USING_SHARED_PTR(Session);
USING_SHARED_PTR(PacketSession);
USING_SHARED_PTR(Listener);
USING_SHARED_PTR(ServerService);
USING_SHARED_PTR(ClientService);
USING_SHARED_PTR(SendBuffer);
USING_SHARED_PTR(SendBufferChunk);
USING_SHARED_PTR(Job);
USING_SHARED_PTR(JobQueue);

#define size16(val)		static_cast<int16>(sizeof(val))
#define size32(val)		static_cast<int32>(sizeof(val))
#define len16(arr)		static_cast<int16>(sizeof(arr)/sizeof(arr[0]))
#define len32(arr)		static_cast<int32>(sizeof(arr)/sizeof(arr[0]))

#define _STOMP