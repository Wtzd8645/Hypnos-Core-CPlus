#pragma once

#include <atomic>
#include <vector>

namespace Blanketmen {
namespace Hypnos {
namespace Threads {
namespace SPSC {

template <typename T>
class RingBuffer
{
public:
    RingBuffer(size_t cap) : buffer(cap), cap(cap), head(0), tail(0) { }

    size_t Size() const
    {
        size_t curr_head = head.load(std::memory_order_acquire);
        size_t curr_tail = tail.load(std::memory_order_acquire);
        return (curr_head + cap - curr_tail) % cap;
    }

    bool IsEmpty() const
    {
        return head.load(std::memory_order_acquire) == tail.load(std::memory_order_acquire);
    }

    bool IsFull() const
    {
        return (head.load(std::memory_order_acquire) + 1) % cap == tail.load(std::memory_order_acquire);
    }

    bool Enqueue(const T& item)
    {
        size_t curr_head = head.load(std::memory_order_relaxed);
        size_t next_head = (curr_head + 1) % cap;
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
        tail.store((curr_tail + 1) % cap, std::memory_order_release);
        return true;
    }

private:
    std::vector<T> buffer;
    const size_t cap;
    std::atomic<size_t> head;
    std::atomic<size_t> tail;
};

} // namespace SPSC
} // namespace Threads
} // namespace Hypnos
} // namespace Blanketmen