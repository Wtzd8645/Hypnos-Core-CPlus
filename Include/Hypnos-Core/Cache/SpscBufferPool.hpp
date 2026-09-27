#pragma once

#include <Hypnos-Core/Base/Math/MathUtils.hpp>
#include <atomic>
#include <stdexcept>
#include <sys/mman.h>

namespace Blanketmen {

class SpscBufferPool
{
public:
    SpscBufferPool(size_t size, size_t cap = 8, int32 flags = 0)
    {
        this->size = MemoryUtils::AlignUp(size, alignof(byte*));
        capacity = MathUtils::RoundUpToPowerOfTwo(cap);
        mask = capacity - 1;

        buffers = static_cast<byte**>(aligned_alloc(CACHE_LINE_SIZE, sizeof(byte**) * capacity));
        mmap_flags |= flags;
        mmap_size = this->size * capacity;
        mmap_ptr = static_cast<byte*>(mmap(nullptr, mmap_size, mmap_prot, mmap_flags, -1, 0));
        if (mmap_ptr == MAP_FAILED)
        {
            // NOTE: Maybe re-try without huge page flag?
            throw std::bad_alloc();
        }

        for (size_t i = 0; i < capacity; ++i)
        {
            buffers[i] = mmap_ptr + (i * this->size);
        }

        head.store(0, std::memory_order_relaxed);
        tail.store(capacity, std::memory_order_relaxed);
    }

    ~SpscBufferPool()
    {
        free(buffers);
        munmap(mmap_ptr, mmap_size);
    }

    inline size_t Capacity() const noexcept { return capacity; }

    inline bool IsEmpty() const noexcept { return head.load(std::memory_order_relaxed) == tail.load(std::memory_order_acquire); }

    inline bool IsFull() const noexcept { return (tail.load(std::memory_order_relaxed) - head.load(std::memory_order_acquire)) == capacity; }

    inline bool Acquire(byte*& buf) noexcept
    {
        size_t curr = head.load(std::memory_order_relaxed);
        if (curr == tail.load(std::memory_order_acquire))
        {
            return false;
        }

        buf = buffers[curr & mask];
        head.store(curr + 1, std::memory_order_release);
        return true;
    }

    inline bool Release(byte* buf) noexcept
    {
        if (buf == nullptr)
        {
            return false;
        }

        size_t curr = tail.load(std::memory_order_relaxed);
        if ((curr - head.load(std::memory_order_acquire)) == capacity)
        {
            return false;
        }

        buffers[curr & mask] = buf;
        tail.store(curr + 1, std::memory_order_release);
        return true;
    }

private:
    size_t size;
    size_t capacity;
    size_t mask;

    int32 mmap_prot = PROT_READ | PROT_WRITE;
    int32 mmap_flags = MAP_PRIVATE | MAP_ANONYMOUS;
    byte* mmap_ptr;
    size_t mmap_size;

    byte** buffers;
    alignas(CACHE_LINE_SIZE) Atomic<size_t> head;
    alignas(CACHE_LINE_SIZE) Atomic<size_t> tail;
};

} // namespace Blanketmen