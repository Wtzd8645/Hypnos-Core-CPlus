#pragma once

#include "Platform.hpp"
#include <atomic>
#include <forward_list>
#include <memory>

namespace Blanketmen {
namespace Hypnos {
namespace Cache {
namespace SPSC {

class BufferPool
{
public:
    static constexpr size_t MIN_CHUNK_SIZE = sizeof(char8*);

    BufferPool(size_t size, size_t cap = 8)
    {
        if (size < MIN_CHUNK_SIZE)
        {
            size = MIN_CHUNK_SIZE;
        }
        this->size = (size + alignof(char8*) - 1) & ~(alignof(char8*) - 1);
        Allocate(cap > 8 ? cap : 8);
    }

    ~BufferPool()
    {
        for (auto& ptr : blocks)
        {
            free(ptr);
        }
    }

    inline size_t Capacity() const noexcept { return capacity; }

    inline uint8* Pop()
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

    inline void Push(uint8* buf) noexcept
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
    size_t capacity = 0;
    size_t size = 0;

    std::forward_list<uint8*> blocks;
    std::atomic<uint8*> free_buffer = nullptr;

    void Allocate(size_t count)
    {
        uint8* ptr = static_cast<uint8*>(std::aligned_alloc(alignof(std::max_align_t), size * count));
        if (ptr == nullptr)
        {
            throw std::bad_alloc();
        }

        capacity += count;
        blocks.push_front(ptr);

        uint8* new_head = ptr;
        uint8* tail = new_head;
        for (size_t i = 1; i < count; ++i)
        {
            uint8* next = tail + size;
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