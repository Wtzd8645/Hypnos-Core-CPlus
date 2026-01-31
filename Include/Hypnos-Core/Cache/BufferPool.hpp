#pragma once

#include <Hypnos-Core/Base/Math/MathUtils.hpp>
#include <stdexcept>
#include <sys/mman.h>

namespace Blanketmen {

class BufferPool
{
public:
    BufferPool(size_t size, size_t cap = 8, int32 flags = 0)
    {
        this->size = MemoryUtils::AlignUp(size, alignof(byte*));
        capacity = MathUtils::RoundUpToPowerOfTwo(cap);
        mask = capacity - 1;

        buffers = static_cast<byte**>(aligned_alloc(alignof(byte*), sizeof(byte*) * capacity));
        mmap_flags |= flags;
        mmap_size = this->size * capacity;
        mmap_ptr = static_cast<byte*>(mmap(nullptr, mmap_size, mmap_prot, mmap_flags, -1, 0));
        if (mmap_ptr == MAP_FAILED)
        {
            throw std::bad_alloc();
        }

        for (size_t i = 0; i < capacity; ++i)
        {
            buffers[i] = mmap_ptr + (i * this->size);
        }

        head = 0;
        tail = capacity;
    }

    ~BufferPool()
    {
        free(buffers);
        munmap(mmap_ptr, mmap_size);
    }

    BufferPool(const BufferPool&) = delete;

    BufferPool& operator=(const BufferPool&) = delete;

    BufferPool(BufferPool&&) = delete;

    BufferPool& operator=(BufferPool&&) = delete;

    inline size_t Capacity() const noexcept { return capacity; }

    inline bool IsEmpty() const noexcept { return head == tail; }

    inline bool IsFull() const noexcept { return (tail - head) == capacity; }

    inline bool Acquire(byte*& buf) noexcept
    {
        if (head == tail)
        {
            return false;
        }

        buf = buffers[head & mask];
        ++head;
        return true;
    }

    inline bool Release(byte* buf) noexcept
    {
        if (buf == nullptr)
        {
            return false;
        }

        if ((tail - head) == capacity)
        {
            return false;
        }

        buffers[tail & mask] = buf;
        ++tail;
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
    size_t head;
    size_t tail;
};

} // namespace Blanketmen