#pragma once

#include <atomic>
#include <cassert>
#include <condition_variable>
#include <cstddef>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

namespace Blanketmen {

// Pre-defined Compiler Macros: https://sourceforge.net/p/predef/wiki/OperatingSystems/

#pragma region Type
// DataModel: https://en.cppreference.com/w/cpp/language/types
typedef signed char               int8;
typedef unsigned char             uint8;
typedef signed short int          int16;
typedef unsigned short int        uint16;
typedef signed int                int32;
typedef unsigned int              uint32;
typedef signed long long int      int64;
typedef unsigned long long int    uint64;
typedef float                     float32;
typedef double                    float64;
typedef std::byte                 byte;
typedef std::string               string;
#pragma endregion

#pragma region Status
enum class ErrorCode : int32
{
    Success = 0,
    // Control
    Cancelled = 100,
    NotReady = 101,
    AlreadyDone = 102,
    Unsupported = 103,
    // Resource
    OutOfMemory = 200,
    ResourceExhausted = 201,
    QuotaExceeded = 202,
    // Concurrency
    WouldBlock = 300,
    Busy = 301,
    Timeout = 302,
    RateLimited = 303,
    Unavailable = 304,
    // System
    NotFound = 400,
    AlreadyExists = 401,
    InvalidArgument = 402,
    InvalidHandle = 403,
    IOError = 404,
    InitializationFailed = 405,
    ConfigurationError = 406,
    Unauthenticated = 407,
    PermissionDenied = 408,
    // Data
    InvalidFormat = 500,
    ChecksumMismatch = 501,
    VersionMismatch = 502,

    Unknown = 999,
};

template<typename T>
class Status
{
public:
    template<typename U>
    static Status Success(U&& val) noexcept
    {
        return Status(std::forward<U>(val));
    }

    static Status Error(ErrorCode err, const char* msg) noexcept
    {
        assert(err != ErrorCode::Success);
        return Status((int32)err, msg);
    }

    bool IsFailed() const noexcept { return error_code != 0; }
    int32 ErrorCode() const noexcept { return error_code; }
    const T& Value() const noexcept { assert(value_engaged); return storage.value; }
    const char* Message() const noexcept { assert(error_code != 0); return storage.message; }

    ~Status()
    {
        Release();
    }

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

private:
    // NOTE: Explicit ctor/dtor are required for unions containing non-trivial members to ensure member lifetimes are managed explicitly
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
    static Status Success() noexcept
    {
        return Status();
    }

    static Status Error(ErrorCode err, const char* msg) noexcept
    {
        assert(err != ErrorCode::Success);
        return Status((int32)err, msg);
    }

    bool IsFailed() const noexcept { return error_code != 0; }
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
template<typename T, typename... TArgs>
using Function = std::function<T(TArgs...)>;

template<typename T>
using UniquePtr = std::unique_ptr<T>;

template<typename T>
using SharedPtr = std::shared_ptr<T>;

template<typename T>
using WeakPtr = std::weak_ptr<T>;
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