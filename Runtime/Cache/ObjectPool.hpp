#ifndef OBJECT_POOL_HPP_
#define OBJECT_POOL_HPP_

#include "../Type.hpp"

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
        ObjectNode* nextNode = nullptr;
    };

    struct MemoryChunk
    {
        int32 capacity;
        ObjectNode* objectNodes;
        MemoryChunk* nextNode = nullptr;

        MemoryChunk(int32 objectCount) : capacity(objectCount), objectNodes(new ObjectNode[objectCount])
        {
            --objectCount;
            for (int i = 0; i < objectCount; ++i)
            {
                ObjectNode* node = objectNodes + i;
                (node)->nextNode = (node + 1);
            }
            (objectNodes + objectCount)->nextNode = nullptr;
        }

        ~MemoryChunk()
        {
            delete[] objectNodes;
        }
    };

    int32 capacity = 0;
    MemoryChunk* currentChunk = nullptr;
    ObjectNode* freeObjects = nullptr;

public:
    inline int32 Capacity() const noexcept
    {
        return capacity;
    }

    ObjectPool(int32 capacity = 8)
    {
        Allocate(capacity > 8 ? capacity : 8);
    }

    ~ObjectPool()
    {
        MemoryChunk* chunk = currentChunk;
        while (chunk != nullptr)
        {
            MemoryChunk* nextChunk = chunk->nextNode;
            delete chunk;
            chunk = nextChunk;
        }
    }

    T* Pop()
    {
        if (freeObjects == nullptr)
        {
            Allocate(capacity);
        }

        T* ptr = reinterpret_cast<T*>(freeObjects);
        freeObjects = freeObjects->nextNode;
        return ptr;
    }

    void Push(T* obj) // TODO: Auto recycle and handle outside object push.
    {
        if (obj == nullptr)
        {
            return;
        }

        reinterpret_cast<ObjectNode*>(obj)->nextNode = freeObjects;
        freeObjects = reinterpret_cast<ObjectNode*>(obj);
    }

private:
    void Allocate(int32 objCount)
    {
        MemoryChunk* chunk = new MemoryChunk(objCount);
        capacity += objCount;
        freeObjects = chunk->objectNodes;
        chunk->nextNode = currentChunk;
        currentChunk = chunk;
    }
};

} // namespace Cache
} // namespace Hypnos
} // namespace Blanketmen

#endif // OBJECT_POOL_HPP_