#pragma once

#include <atomic>
#include <vector>

namespace Blanketmen {
namespace Hypnos {
namespace Threads {
namespace MPSC {

template <typename T>
class RingBuffer
{

public:
    explicit RingBuffer(size_t cap) : capacity(cap), head(0), tail(0), buffer(cap)
    {
    }

    bool IsEmpty() const
    {
        return head.load(std::memory_order_acquire) == tail.load(std::memory_order_acquire);
    }

    bool Enqueue(const T& item)
    {
        size_t currTail = tail.load(std::memory_order_relaxed);
        size_t nextTail = (currTail + 1) % capacity;
        if (nextTail == head.load(std::memory_order_acquire))
        {
            return false;
        }

        buffer[currTail] = item;
        tail.store(nextTail, std::memory_order_release);
        return true;
    }

    bool Dequeue(T& item)
    {
        size_t currHead = head.load(std::memory_order_relaxed);
        if (currHead == tail.load(std::memory_order_acquire))
        {
            return false;
        }

        item = buffer[currHead];
        head.store((currHead + 1) % capacity, std::memory_order_release);
        return true;
    }

private:
    const size_t capacity;

    std::vector<T> buffer;
    alignas(64) std::atomic<size_t> head;
    alignas(64) std::atomic<size_t> tail;
};

} // namespace MPSC
} // namespace Threads
} // namespace Hypnos
} // namespace Blanketmen