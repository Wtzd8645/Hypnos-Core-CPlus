#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>

namespace Blanketmen {
namespace Hypnos {

using Thread = std::thread;

using Mutex = std::mutex;
using ConditionVariable = std::condition_variable;

template<typename T>
using Atomic = std::atomic<T>;

template<typename T>
using LockGuard = std::lock_guard<T>;

template<typename T>
using UniqueLock = std::unique_lock<T>;

} // namespace Hypnos
} // namespace Blanketmen