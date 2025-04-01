#pragma once

#include "Platform.hpp"
#include <atomic>
#include <forward_list>
#include <stdexcept>
#include <sys/mman.h>

namespace Blanketmen {
namespace Hypnos {
namespace Cache {
namespace SPSC {

class MmapBufferPool
{
public:
    static constexpr size_t MIN_BUFFER_SIZE = 512;

    MmapBufferPool(size_t size, size_t flags, size_t cap = 8)
    {
        this->size = MemoryUtils::AlignUp(size >= MIN_BUFFER_SIZE ? size : MIN_BUFFER_SIZE, alignof(uint8*));
        capacity = MathUtils::RoundUpToPowerOfTwo(cap);
        mask = capacity - 1;

        buffers = static_cast<uint8**>(std::aligned_alloc(CACHE_LINE_SIZE, capacity * sizeof(uint8**)));
        mmap_flags |= flags;
        mmap_size = this->size * capacity;
        mmap_ptr = static_cast<uint8*>(mmap(nullptr, mmap_size, mmap_prot, mmap_flags, -1, 0));
        if (mmap_ptr == MAP_FAILED)
        {
            throw std::bad_alloc();
        }

        for (size_t i = 0; i < capacity; ++i)
        {
            buffers[i] = mmap_ptr + (i * this->size);
        }

        head.store(0, std::memory_order_relaxed);
        tail.store(capacity, std::memory_order_relaxed);
    }

    ~MmapBufferPool()
    {
        std::free(buffers);
        munmap(mmap_ptr, mmap_size);
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

    inline bool Acquire(uint8*& buf) noexcept
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

    inline bool Release(uint8* buf) noexcept
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
    uint8* mmap_ptr;
    size_t mmap_size;

    uint8** buffers;
    alignas(CACHE_LINE_SIZE) Atomic<size_t> head;
    alignas(CACHE_LINE_SIZE) Atomic<size_t> tail;
};

} // namespace SPSC
} // namespace Cache
} // namespace Hypnos
} // namespace Blanketmen