#pragma once

#include "Hypnos-Core/Types.hpp"
#include <cstdlib>

namespace Blanketmen {
namespace Hypnos {
namespace Cache {

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
        if (free_nodes == nullptr)
        {
            Allocate(capacity);
        }

        T* ptr = reinterpret_cast<T*>(free_nodes);
        free_nodes = free_nodes->next;
        return ptr;
    }

    inline void Push(T* obj)
    {
        if (obj == nullptr)
        {
            return;
        }

        reinterpret_cast<object_node*>(obj)->next = free_nodes;
        free_nodes = reinterpret_cast<object_node*>(obj);
    }

private:
    union object_node
    {
        object_node* next;
        T object;
    };

    struct memory_chunk
    {
        memory_chunk* next;
        object_node* nodes;

        memory_chunk(int32 count)
        {
            nodes = static_cast<object_node*>(malloc(count * sizeof(object_node)));
            count--;
            for (int i = 0; i < count; i++)
            {
                nodes[i].next = &nodes[i + 1];
            }
            (nodes + count)->next = nullptr;
        }

        ~memory_chunk()
        {
            free(nodes);
        }
    };

    int32 capacity = 0;
    memory_chunk* chunks = nullptr;
    object_node* free_nodes = nullptr;

    void Allocate(int32 count)
    {
        memory_chunk* chunk = new memory_chunk(count);
        capacity += count;
        
        chunk->next = chunks;
        chunks = chunk;

        free_nodes = chunk->nodes;
    }
};

} // namespace Cache
} // namespace Hypnos
} // namespace Blanketmen