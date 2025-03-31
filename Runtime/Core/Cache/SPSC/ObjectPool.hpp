#pragma once

#include "Platform.hpp"

namespace Blanketmen {
namespace Hypnos {
namespace Cache {
namespace SPSC {

template<typename T>
class ObjectPool
{
public:
    ObjectPool(size_t cap = 8)
    {
        Allocate(cap > 8 ? cap : 8);
    }

    ~ObjectPool()
    {
        MemoryChunk* chunk = chunks;
        while (chunk != nullptr)
        {
            MemoryChunk* next = chunk->next;
            delete chunk;
            chunk = next;
        }
    }

    inline size_t Capacity() const noexcept { return capacity; }

    inline T* Acquire()
    {
        ObjectNode* head = free_nodes.load(std::memory_order_acquire);
        if (head == nullptr)
        {
            Allocate(capacity);
            head = free_nodes.load(std::memory_order_acquire);
        }

        while (!free_nodes.compare_exchange_weak(head, head->next, std::memory_order_acquire, std::memory_order_relaxed)) { }
        return reinterpret_cast<T*>(head);
    }

    inline void Release(T* obj)
    {
        if (obj == nullptr)
        {
            return;
        }

        ObjectNode* node = reinterpret_cast<ObjectNode*>(obj);
        ObjectNode* head = free_nodes.load(std::memory_order_relaxed);
        do
        { 
            node->next = head;
        }
        while (!free_nodes.compare_exchange_weak(head, node, std::memory_order_release, std::memory_order_relaxed));
    }

private:
    struct alignas(alignof(T)) ObjectNode
    {
        T object;
        ObjectNode* next = nullptr;
    };

    struct MemoryChunk
    {
        ObjectNode* nodes;
        MemoryChunk* next;

        MemoryChunk(size_t count) :
            nodes(static_cast<ObjectNode*>(std::aligned_alloc(alignof(ObjectNode), count * sizeof(ObjectNode)))),
            next(nullptr)
        {
            count--;
            for (size_t i = 0; i < count - 1; ++i)
            {
                nodes[i].next = &nodes[i + 1];
            }
            (nodes + count)->next = nullptr;
        }

        ~MemoryChunk()
        {
            std::free(nodes);
        }
    };

    size_t capacity = 0;
    MemoryChunk* chunks = nullptr;
    std::atomic<ObjectNode*> free_nodes = nullptr;

    void Allocate(size_t count)
    {
        MemoryChunk* chunk = new MemoryChunk(count);
        capacity += count;

        chunk->next = chunks;
        chunks = chunk;

        ObjectNode* new_head = chunk->nodes;
        ObjectNode* tail = &chunk->nodes[count - 1];
        ObjectNode* head = free_nodes.load(std::memory_order_relaxed);
        do
        {
            tail->next = head;
        }
        while (!free_nodes.compare_exchange_weak(head, new_head, std::memory_order_release, std::memory_order_relaxed));
    }
};

} // namespace SPSC
} // namespace Cache
} // namespace Hypnos
} // namespace Blanketmen