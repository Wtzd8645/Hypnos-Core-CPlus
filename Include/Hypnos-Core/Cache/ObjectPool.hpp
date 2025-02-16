#pragma once

#include "Hypnos-Core/Type.hpp"

namespace Blanketmen {
namespace Hypnos {
namespace Cache {

template<typename T>
class ObjectPool
{
private:
    struct ObjectNode
    {
        T object;
        ObjectNode* next = nullptr;
    };

    struct MemoryChunk
    {
        int32 capacity;
        ObjectNode* objects;
        MemoryChunk* next = nullptr;

        MemoryChunk(int32 count) : capacity(count), objects(new ObjectNode[count])
        {
            --count;
            for (int i = 0; i < count; ++i)
            {
                ObjectNode* node = objects + i;
                (node)->next = (node + 1);
            }
            (objects + count)->next = nullptr;
        }

        ~MemoryChunk()
        {
            delete[] objects;
        }
    };

public:
    ObjectPool(int32 cap = 8)
    {
        Allocate(cap > 8 ? cap : 8);
    }

    ~ObjectPool()
    {
        MemoryChunk* chunk = currChunk;
        while (chunk != nullptr)
        {
            MemoryChunk* nextChunk = chunk->next;
            delete chunk;
            chunk = nextChunk;
        }
    }

    inline int32 Capacity() const noexcept
    {
        return capacity;
    }

    T* Pop()
    {
        if (freeObjects == nullptr)
        {
            Allocate(capacity);
        }

        T* ptr = reinterpret_cast<T*>(freeObjects);
        freeObjects = freeObjects->next;
        return ptr;
    }

    void Push(T* obj) // TODO: Auto recycle and handle outside object push.
    {
        if (obj == nullptr)
        {
            return;
        }

        reinterpret_cast<ObjectNode*>(obj)->next = freeObjects;
        freeObjects = reinterpret_cast<ObjectNode*>(obj);
    }

private:
    int32 capacity = 0;
    MemoryChunk* currChunk = nullptr;
    ObjectNode* freeObjects = nullptr;

    void Allocate(int32 count)
    {
        MemoryChunk* chunk = new MemoryChunk(count);
        capacity += count;
        freeObjects = chunk->objects;
        chunk->next = currChunk;
        currChunk = chunk;
    }
};

} // namespace Cache
} // namespace Hypnos
} // namespace Blanketmen