#pragma once

#include <atomic>
#include <vector>

namespace Blanketmen {
namespace Hypnos {
namespace Container {
namespace SPSC {

template <typename T>
class RingBuffer
{
public:
    static constexpr size_t MIN_CAPACITY = 2;

    RingBuffer(size_t cap) : buffer(cap), capacity(cap), head(0), tail(0) { } // TODO: Limit capacity.

    inline size_t Size() const
    {
        size_t curr_head = head.load(std::memory_order_acquire);
        size_t curr_tail = tail.load(std::memory_order_acquire);
        return (curr_head + capacity - curr_tail) % capacity;
    }

    inline bool IsEmpty() const noexcept
    {
        return head.load(std::memory_order_acquire) == tail.load(std::memory_order_acquire);
    }

    inline bool IsFull() const noexcept
    {
        return (head.load(std::memory_order_acquire) + 1) % capacity == tail.load(std::memory_order_acquire);
    }

    bool Enqueue(const T& item)
    {
        size_t curr_head = head.load(std::memory_order_relaxed);
        size_t next_head = (curr_head + 1) % capacity;
        if (next_head == tail.load(std::memory_order_acquire))
        {
            return false;
        }

        buffer[curr_head] = item;
        head.store(next_head, std::memory_order_release);
        return true;
    }

    bool Dequeue(T& item)
    {
        size_t curr_tail = tail.load(std::memory_order_relaxed);
        if (curr_tail == head.load(std::memory_order_acquire))
        {
            return false;
        }

        item = buffer[curr_tail];
        tail.store((curr_tail + 1) % capacity, std::memory_order_release);
        return true;
    }

private:
    const size_t capacity;

    std::vector<T> buffer;
    alignas(64) std::atomic<size_t> head;
    alignas(64) std::atomic<size_t> tail;
};

} // namespace SPSC
} // namespace Threads
} // namespace Hypnos
} // namespace Blanketmen