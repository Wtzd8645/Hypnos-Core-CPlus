#pragma once

#include <cassert>
#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

namespace Blanketmen {

// Pre-defined Compiler Macros: https://sourceforge.net/p/predef/wiki/OperatingSystems/

#pragma region Type
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

#pragma region Status
template<typename T>
class Status
{
public:
    template<typename U>
    static Status Success(U&& val) noexcept
    {
        return Status(std::forward<U>(val));
    }

    static Status Error(int32 err, const char* msg) noexcept
    {
        assert(err != 0);
        return Status(err, msg);
    }

    bool IsSuccess() const noexcept { return error_code == 0; }
    int32 ErrorCode() const noexcept { return error_code; }
    const T& Value() const noexcept { assert(value_engaged); return storage.value; }
    const char* Message() const noexcept { assert(error_code != 0); return storage.message; }
    
    Status(const Status& other)
    {
        CopyFrom(other);
    }

    Status& operator=(const Status& other)
    {
        if (this != &other)
        {
            Release();
            CopyFrom(other);
        }
        return *this;
    }

    Status(Status&& other) noexcept
    {
        MoveFrom(std::move(other));
    }

    Status& operator=(Status&& other) noexcept
    {
        if (this != &other)
        {
            Release();
            MoveFrom(std::move(other));
        }
        return *this;
    }

    ~Status()
    {
        Release();
    }

private:
    // NOTE: Explicit construtor/destructor are required for unions with non-trivial members.
    union Storage
    {
        T value;
        const char* message;

        Storage() { }
        ~Storage() { }
    };

    int32 error_code;
    bool value_engaged;
    Storage storage;

    template<typename U>
    explicit Status(U&& val) : error_code(0), value_engaged(true)
    {
        new (&storage.value) T(std::forward<U>(val));
    }

    Status(int32 err, const char* msg) : error_code(err), value_engaged(false)
    {
        new (&storage.message) const char* (msg);
    }

    void Release()
    {
        if (value_engaged)
        {
            value_engaged = false;
            storage.value.~T();
        }
    }

    void CopyFrom(const Status& other)
    {
        error_code = other.error_code;
        value_engaged = other.value_engaged;

        if (error_code != 0)
        {
            value_engaged = false;
            new (&storage.message) const char* (other.storage.message);
            return;
        }

        if (value_engaged)
        {
            new (&storage.value) T(other.storage.value);
        }
    }

    void MoveFrom(Status&& other) noexcept
    {
        error_code = other.error_code;
        value_engaged = other.value_engaged;

        if (error_code != 0)
        {
            value_engaged = false;
            new (&storage.message) const char* (other.storage.message);
            return;
        }

        if (value_engaged)
        {
            new (&storage.value) T(std::move(other.storage.value));
            other.value_engaged = false;
            other.storage.value.~T();
        }
    }
};

template<>
class Status<void>
{
public:
    static Status Success() noexcept { return Status(); }
    static Status Error(int32 err, const char* msg) noexcept { assert(err != 0); return Status(err, msg); }

    bool IsSuccess() const noexcept { return error_code == 0; }
    int32 ErrorCode() const noexcept { return error_code; }
    const char* Message() const noexcept { assert(error_code != 0); return message; }

private:
    int32 error_code;
    const char* message;

    Status() : error_code(0) { }
    Status(int32 err, const char* msg) : error_code(err), message(msg) { }
};
#pragma endregion

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

class Spinlock
{
public:
    inline void Lock()
    {
        while (flag.test_and_set(std::memory_order_acquire))
        {
            std::this_thread::yield();
        }
    }

    inline void Unlock()
    {
        flag.clear(std::memory_order_release);
    }

private:
    std::atomic_flag flag { };
};
#pragma endregion

} // namespace Blanketmen