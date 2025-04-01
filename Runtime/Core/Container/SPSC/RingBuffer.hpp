#pragma once

#include <Hypnos-Kernel/Base/Math.hpp>
#include <Hypnos-Kernel/Base/Memory.hpp>
#include <atomic>

namespace Blanketmen {
namespace Hypnos {
namespace Container {
namespace SPSC {

template <typename T>
class RingBuffer
{
public:
    RingBuffer(size_t cap)
    {
        capacity = MathUtils::RoundUpToPowerOfTwo(cap);
        mask = capacity - 1;

        size_t size = MemoryUtils::AlignUp(capacity * sizeof(T), CACHE_LINE_SIZE);
        buffer = static_cast<T*>(std::aligned_alloc(CACHE_LINE_SIZE, size));

        head.store(0, std::memory_order_relaxed);
        tail.store(capacity, std::memory_order_relaxed);
    }

    ~RingBuffer()
    {
        std::free(buffer);
    }

    inline size_t Capacity() const noexcept
    {
        return capacity;
    }

    inline size_t Count() const noexcept
    {
        return tail.load(std::memory_order_acquire) - head.load(std::memory_order_acquire);
    }

    inline bool IsEmpty() const noexcept
    {
        return head.load(std::memory_order_acquire) == tail.load(std::memory_order_acquire);
    }

    inline bool IsFull() const noexcept
    {
        return (tail.load(std::memory_order_acquire) - head.load(std::memory_order_acquire)) == capacity;
    }

    bool Enqueue(const T& item)
    {
        size_t curr = tail.load(std::memory_order_relaxed);
        if ((curr - head.load(std::memory_order_acquire)) == capacity)
        {
            return false;
        }

        buffer[curr & mask] = item;
        tail.store(curr + 1, std::memory_order_release);
        return true;
    }

    bool Dequeue(T& item)
    {
        size_t curr = head.load(std::memory_order_relaxed);
        if (curr == tail.load(std::memory_order_acquire))
        {
            return false;
        }

        item = buffer[curr & mask];
        head.store(curr + 1, std::memory_order_release);
        return true;
    }

private:
    size_t capacity;
    size_t mask;

    T* buffer;
    alignas(CACHE_LINE_SIZE) Atomic<size_t> head;
    alignas(CACHE_LINE_SIZE) Atomic<size_t> tail;
};

} // namespace SPSC
} // namespace Container
} // namespace Hypnos
} // namespace Blanketmen