#pragma once

namespace Blanketmen {

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

    size_t Capacity() const noexcept { return capacity; }

    T* Acquire()
    {
        if (free_nodes == nullptr)
        {
            Allocate(capacity);
        }

        ObjectNode* node = free_nodes;
        free_nodes = free_nodes->next;
        return reinterpret_cast<T*>(&node->storage);
    }

    void Release(T* obj)
    {
        if (obj == nullptr)
        {
            return;
        }

        reinterpret_cast<ObjectNode*>(obj)->next = free_nodes;
        free_nodes = reinterpret_cast<ObjectNode*>(obj);
    }

    template<typename... Args>
    T* Emplace(Args&&... args)
    {
        if (free_nodes == nullptr)
        {
            Allocate(capacity);
        }

        ObjectNode* node = free_nodes;
        free_nodes = free_nodes->next;
        return new (&node->storage) T(std::forward<Args>(args)...);
    }

    void Destroy(T* obj)
    {
        if (obj == nullptr)
        {
            return;
        }

        obj->~T();
        reinterpret_cast<ObjectNode*>(obj)->next = free_nodes;
        free_nodes = reinterpret_cast<ObjectNode*>(obj);
    }

private:
    union ObjectNode
    {
        AlignedStorage<T> storage;
        ObjectNode* next;
    };

    struct MemoryChunk
    {
        ObjectNode* nodes;
        MemoryChunk* next;

        MemoryChunk(size_t count) :
            nodes(static_cast<ObjectNode*>(std::aligned_alloc(alignof(ObjectNode), sizeof(ObjectNode) * count))),
            next(nullptr)
        {
            count--;
            for (size_t i = 0; i < count; ++i)
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
    ObjectNode* free_nodes = nullptr;

    void Allocate(size_t count)
    {
        capacity += count;
        MemoryChunk* chunk = new MemoryChunk(count);
        chunk->next = chunks;
        chunks = chunk;
        free_nodes = chunk->nodes;
    }
};

} // namespace Blanketmen