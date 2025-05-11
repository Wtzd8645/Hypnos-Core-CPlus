#pragma once

#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

namespace Blanketmen {

// Pre-defined Compiler Macros: https://sourceforge.net/p/predef/wiki/OperatingSystems/

// region Platform
#pragma region Platform
// DataModel: https://en.cppreference.com/w/cpp/language/types
// NOTE: Signedness char represents a character, and shouldn't care whether it is signed or unsigned.
typedef char                      char8;
typedef signed char               int8;
typedef unsigned char             uint8;
typedef std::byte                 byte;
typedef signed short int          int16;
typedef unsigned short int        uint16;
typedef signed int                int32;
typedef unsigned int              uint32;
typedef signed long long int      int64;
typedef unsigned long long int    uint64;
typedef float                     float32;
typedef double                    float64;
typedef std::string               string;
#pragma endregion

// region Memory
#pragma region Memory
template<typename T, size_t TSize = sizeof(T), size_t TAlign = alignof(T)>
using AlignedStorage = std::aligned_storage_t<TSize, TAlign>;

template<typename T>
using Unique = std::unique_ptr<T>;

template<typename T>
using Shared = std::shared_ptr<T>;

template<typename T>
using Weak = std::weak_ptr<T>;
#pragma endregion
// endregion

// region Thread
#pragma region Thread
#if defined(_MSC_VER)
#define THREAD_LOCAL __declspec(thread)
#else
#define THREAD_LOCAL __thread
#endif

constexpr size_t CACHE_LINE_SIZE = 64;

using Thread = std::thread;

template<typename T>
using Atomic = std::atomic<T>;

using Mutex = std::mutex;
using ConditionVariable = std::condition_variable;

template<typename T>
using LockGuard = std::lock_guard<T>;

template<typename T>
using UniqueLock = std::unique_lock<T>;
#pragma endregion


class Spinlock
{
public:
    void lock()
    {
        while (flag.test_and_set(std::memory_order_acquire))
        {
            std::this_thread::yield();
        }
    }

    void unlock()
    {
        flag.clear(std::memory_order_release);
    }

private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
};
// endregion

} // namespace Blanketmen