#ifndef THREAD_HPP_
#define THREAD_HPP_

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>

namespace Blanketmen {
namespace Hypnos {

using ConditionVariable = std::condition_variable;
using Mutex = std::mutex;
using Thread = std::thread;

template<class T>
using UniqueLock = std::unique_lock<T>;

} // namespace Hypnos
} // namespace Blanketmen

#endif // THREAD_HPP_