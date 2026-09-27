#pragma once

#include <Hypnos-Core/Base/Memory/MemoryAllocatePolicy.hpp>
#include <Hypnos-Core/Base/Memory/MemoryUtils.hpp>

namespace Blanketmen {

template<typename T, typename TAllocPolicy>
class ObjectPool
{
public:
    ObjectPool() = default;

    ~ObjectPool()
    {
        Chunk* chunk = chunks;
        while (chunk != nullptr)
        {
            Chunk* next = chunk->next;
            TAllocPolicy::Deallocate(chunk->ptr, chunk->size);
            MemoryUtils::Deallocate<Chunk>(chunk);
            chunk = next;
        }
    }

    ObjectPool(const ObjectPool&) = delete;
    ObjectPool& operator=(const ObjectPool&) = delete;

    ObjectPool(ObjectPool&&) = delete;
    ObjectPool& operator=(ObjectPool&&) = delete;

    size_t Capacity() const noexcept { return capacity; }

    Status<void> Allocate(size_t count)
    {
        size_t alloc_size = sizeof(Storage) * count;
        byte* ptr = TAllocPolicy::Allocate(alloc_size);
        if (ptr == nullptr)
        {
            return Status<void>::Error(ErrorCode::OutOfMemory, "ObjectPool allocation failed.");
        }

        void* bytes = MemoryUtils::Allocate<Chunk>(sizeof(Chunk));
        if (bytes == nullptr)
        {
            TAllocPolicy::Deallocate(ptr, alloc_size);
            return Status<void>::Error(ErrorCode::OutOfMemory, "ObjectPool allocation failed.");
        }

        count = alloc_size / sizeof(Storage);
        capacity += count;

        Chunk* chunk = static_cast<Chunk*>(bytes);
        chunk->ptr = ptr;
        chunk->size = alloc_size;
        chunk->next = chunks;
        chunks = chunk;

        Storage* storages = reinterpret_cast<Storage*>(ptr);
        for (size_t i = 0; i < count - 1; ++i)
        {
            storages[i].next = &storages[i + 1];
        }
        storages[count - 1].next = free_nodes;
        free_nodes = storages;
        return Status<void>::Success();
    }

    T* Acquire()
    {
        assert(capacity > 0 && "ObjectPool is not allocated.");
        if (free_nodes == nullptr && Allocate(capacity).IsFailed())
        {
            return nullptr;
        }

        Storage* storage = free_nodes;
        free_nodes = free_nodes->next;
        return new (storage) T();
    }

    void Release(T* obj)
    {
        assert(obj != nullptr && "Cannot release a null object.");
        obj->~T();
        Storage* storage = reinterpret_cast<Storage*>(obj);
        storage->next = free_nodes;
        free_nodes = storage;
    }

private:
    union Storage
    {
        Storage* next;
        T value;
    };

    struct Chunk
    {
        Chunk* next;
        byte* ptr;
        size_t size;
    };

    size_t capacity = 0;
    Chunk* chunks = nullptr;
    Storage* free_nodes = nullptr;
};

template<typename T>
using HeapObjectPool = ObjectPool<T, HeapAllocatePolicy>;
template<typename T>
using MmapObjectPool = ObjectPool<T, MmapAllocatePolicy<0>>;

} // namespace Blanketmen