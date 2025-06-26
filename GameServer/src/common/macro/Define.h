#pragma once

/**===========================Macro========================================*/

template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

/**SINGLETON*/
#define SINGLETON_BASE(Class)                    \
                                                 \
public:                                          \
    static Class& GetInstance()                  \
    {                                            \
        static Class instance;                   \
                                                 \
        return instance;                         \
    }                                            \
                                                 \
private:                                         \
    Class() = default;                           \
    ~Class() = default;                          \
    Class(const Class&) = delete;                \
    Class& operator=(const Class&) = delete;     \
    Class(Class&&) = delete;                     \
    Class& operator=(Class&&) = delete;
	