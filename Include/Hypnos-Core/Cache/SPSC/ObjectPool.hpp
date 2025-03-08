#pragma once

#include "Hypnos-Core/Type.hpp"

namespace Blanketmen {
namespace Hypnos {
namespace Cache {
namespace SPSC {

template<typename T>
class ObjectPool
{
public:
    ObjectPool(int32 cap = 8)
    {
        Allocate(cap > 8 ? cap : 8);
    }

    ~ObjectPool()
    {
        memory_chunk* chunk = chunks;
        while (chunk != nullptr)
        {
            memory_chunk* next = chunk->next;
            delete chunk;
            chunk = next;
        }
    }

    inline int32 Capacity() const noexcept { return capacity; }

    inline T* Pop()
    {
        object_node* head = free_nodes.load(std::memory_order_acquire);
        if (head == nullptr)
        {
            Allocate(capacity);
            head = free_nodes.load(std::memory_order_acquire);
        }

        while (!free_nodes.compare_exchange_weak(head, head->next, std::memory_order_acquire, std::memory_order_relaxed)) { }
        return reinterpret_cast<T*>(head);
    }

    inline void Push(T* obj) // TODO: Auto recycle and handle outside object push.
    {
        if (obj == nullptr)
        {
            return;
        }

        object_node* node = reinterpret_cast<object_node*>(obj);
        object_node* head = free_nodes.load(std::memory_order_relaxed);
        do
        { 
            node->next = head;
        }
        while (!free_nodes.compare_exchange_weak(head, node, std::memory_order_release, std::memory_order_relaxed));
    }

private:
    struct alignas(alignof(T)) object_node
    {
        T object;
        object_node* next = nullptr;
    };

    struct memory_chunk
    {
        object_node* nodes;
        memory_chunk* next = nullptr;

        memory_chunk(int32 count) : nodes(new object_node[count])
        {
            --count;
            for (int i = 0; i < count - 1; ++i)
            {
                nodes[i].next = &nodes[i + 1];
            }
            (nodes + count)->next = nullptr;
        }

        ~memory_chunk()
        {
            delete[] nodes;
        }
    };

    int32 capacity = 0;
    memory_chunk* chunks = nullptr;
    std::atomic<object_node*> free_nodes = nullptr;

    void Allocate(int32 count)
    {
        memory_chunk* chunk = new memory_chunk(count);
        capacity += count;

        chunk->next = chunks;
        chunks = chunk;

        object_node* new_head = chunk->nodes;
        object_node* tail = &chunk->nodes[count - 1];
        object_node* head = free_nodes.load(std::memory_order_relaxed);
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