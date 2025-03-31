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
    static constexpr size_t MIN_BUFFER_SIZE = 1024;

    MmapBufferPool(size_t buf_size, size_t flags, size_t cap = 8)
    {
        if (buf_size < MIN_BUFFER_SIZE)
        {
            buf_size = MIN_BUFFER_SIZE;
        }

        this->buf_size = (buf_size + alignof(uint8*) - 1) & ~(alignof(uint8*) - 1);
        mmap_flags |= flags;
        Allocate(cap > 8 ? cap : 8);
    }

    ~MmapBufferPool()
    {
        for (auto& block : blocks)
        {
            munmap(block.ptr, block.size);
        }
    }

    inline size_t Capacity() const noexcept { return capacity; }

    inline uint8* Acquire()
    {
        uint8* head = free_buffer.load(std::memory_order_acquire);
        if (head == nullptr)
        {
            Allocate(capacity);
            head = free_buffer.load(std::memory_order_acquire);
        }

        uint8* next = *reinterpret_cast<uint8**>(head);
        while (!free_buffer.compare_exchange_weak(head, next, std::memory_order_release, std::memory_order_relaxed))
        {
            next = *reinterpret_cast<uint8**>(head);
        }
        return head;
    }

    inline void Release(uint8* buf) noexcept
    {
        if (buf == nullptr)
        {
            return;
        }

        uint8* head = free_buffer.load(std::memory_order_relaxed);
        do
        {
            *reinterpret_cast<uint8**>(buf) = head;
        }
        while (!free_buffer.compare_exchange_weak(head, buf, std::memory_order_release, std::memory_order_relaxed));
    }

private:
    struct MmapBlock
    {
        uint8* ptr;
        size_t size;
    };

    size_t buf_size = 0;
    int32 mmap_prot = PROT_READ | PROT_WRITE;
    int32 mmap_flags = MAP_PRIVATE | MAP_ANONYMOUS;
    size_t capacity = 0;

    std::forward_list<MmapBlock> blocks;
    std::atomic<uint8*> free_buffer = nullptr;

    void Allocate(size_t count)
    {
        size_t mmap_size = buf_size * count;
        uint8* ptr = static_cast<uint8*>(mmap(nullptr, mmap_size, mmap_prot, mmap_flags, -1, 0));
        if (ptr == MAP_FAILED)
        {
            throw std::bad_alloc();
        }

        capacity += count;
        blocks.push_front({ ptr, mmap_size });

        uint8* new_head = ptr;
        uint8* tail = new_head;
        for (size_t i = 1; i < count; ++i)
        {
            uint8* next = tail + buf_size;
            *reinterpret_cast<uint8**>(tail) = next;
            tail = next;
        }

        uint8* head = free_buffer.load(std::memory_order_relaxed);
        do
        {
            *reinterpret_cast<uint8**>(tail) = head;
        }
        while (!free_buffer.compare_exchange_weak(head, new_head, std::memory_order_release, std::memory_order_relaxed));
    }
};

} // namespace SPSC
} // namespace Cache
} // namespace Hypnos
} // namespace Blanketmen