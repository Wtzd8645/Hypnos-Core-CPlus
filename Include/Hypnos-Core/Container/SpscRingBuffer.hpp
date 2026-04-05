#pragma once

#include "Hypnos-Core/Base/Math/MathUtils.hpp"
#include "Hypnos-Core/Base/Memory/MemoryUtils.hpp"
#include <type_traits>

namespace Blanketmen {

template <typename T>
class SpscRingBuffer
{
public:
    static_assert(std::is_trivially_copyable_v<T>, "SpscRingBuffer requires trivially copyable payloads.");

    SpscRingBuffer(size_t cap)
    {
        capacity = MathUtils::RoundUpToPowerOfTwo(cap);
        mask = capacity - 1;

        size_t size = MemoryUtils::AlignUp(capacity * sizeof(T), CACHE_LINE_SIZE);
        buffer = static_cast<T*>(MemoryUtils::Allocate(size, CACHE_LINE_SIZE));

        head.store(0, std::memory_order_relaxed);
        tail.store(0, std::memory_order_relaxed);
    }

    ~SpscRingBuffer()
    {
        MemoryUtils::Deallocate(buffer, CACHE_LINE_SIZE);
    }

    size_t Capacity() const noexcept { return capacity; }
    size_t Size() const noexcept { return tail.load(std::memory_order_acquire) - head.load(std::memory_order_acquire); }
    size_t RemainingCapacity() const noexcept { return capacity - Size(); }

    bool IsEmpty() const noexcept { return head.load(std::memory_order_relaxed) == tail.load(std::memory_order_acquire); }

    bool IsFull() const noexcept { return (tail.load(std::memory_order_relaxed) - head.load(std::memory_order_acquire)) == capacity; }

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

} // namespace Blanketmen
