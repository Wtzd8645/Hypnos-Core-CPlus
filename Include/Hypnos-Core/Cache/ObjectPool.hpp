#pragma once

#include <Hypnos-Core/Base/Memory/MemoryUtils.hpp>

namespace Blanketmen {

template<typename T>
class ObjectPool
{
public:
    ObjectPool(size_t cap = 8)
    {
        Allocate(cap);
    }

    ~ObjectPool()
    {
        Chunk* chunk = chunks;
        while (chunk != nullptr)
        {
            Chunk* next = chunk->next;
            MemoryUtils::Deallocate<Chunk>(chunk);
            chunk = next;
        }
    }

    ObjectPool(const ObjectPool&) = delete;

    ObjectPool& operator=(const ObjectPool&) = delete;

    ObjectPool(ObjectPool&&) = delete;

    ObjectPool& operator=(ObjectPool&&) = delete;

    size_t Capacity() const noexcept { return capacity; }

    T* Acquire()
    {
        if (free_storages == nullptr && Allocate(capacity).IsFailed())
        {
            return nullptr;
        }

        Storage* storage = free_storages;
        free_storages = free_storages->next;
        return new (storage) T();
    }

    void Release(T* obj)
    {
        if (obj == nullptr)
        {
            return;
        }

        obj->~T();
        Storage* storage = reinterpret_cast<Storage*>(obj);
        storage->next = free_storages;
        free_storages = storage;
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
        alignas(Storage) byte storage[];
    };

    size_t capacity = 0;
    Chunk* chunks = nullptr;
    Storage* free_storages = nullptr;

    Status<void> Allocate(size_t count)
    {
        void* bytes = MemoryUtils::Allocate<Chunk>(offsetof(Chunk, storage) + sizeof(Storage) * count);
        if (bytes == nullptr)
        {
            return Status<void>::Error(ErrorCode::OutOfMemory, "ObjectPool allocation failed.");
        }

        capacity += count;

        Chunk* chunk = static_cast<Chunk*>(bytes);
        chunk->next = chunks;
        chunks = chunk;

        Storage* storages = reinterpret_cast<Storage*>(chunk->storage);
        for (size_t i = 0; i < count - 1; ++i)
        {
            storages[i].next = &storages[i + 1];
        }
        storages[count - 1].next = free_storages;
        free_storages = storages;
        return Status<void>::Success();
    }
};

} // namespace Blanketmen