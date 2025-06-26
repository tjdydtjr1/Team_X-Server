#pragma once

#define OUT


/* =====================================
            Lock
=====================================*/


#define USE_MANY_LOCKS(count)       Lock _locks[count];                              // 여러개의 Lock을 사용
#define USE_LOCK                    USE_MANY_LOCKS(1)                                // 단일 Lock 사용
#define READ_LOCK_IDX(idx)          ReadLockGuard readLockGuard_##idx(_locks[idx], typeid(this).name())   // ##idx -> (idx = 1) => ??_1
#define READ_LOCK                   READ_LOCK_IDX(0)
#define WRITE_LOCK_IDX(idx)         WriteLockGuard writeLockGuard_##idx(_locks[idx], typeid(this).name()) // ##idx0 -> (idx = 1) => ??_1
#define WRITE_LOCK                  WRITE_LOCK_IDX(0)                                                     // ㄴ typeid(this).name() 해당 클래스의 이름 추출 -> 컴파일러가 해줌

/* =====================================
            Crash
=====================================*/

#define CRASH(cause)                     \
{                                        \
    uint32* crash = nullptr;             \
    __analysis_assume(crash != nullptr); \
   *crash = 0xDEADBEEF;                  \
}

#define ASSERT_CRASH(expr)               \
{                                        \
    if (!(expr))                         \
    {                                    \
        CRASH("ASSERT_CRASH");           \
        __analysis_assume(expr);         \
    }                                    \
}
