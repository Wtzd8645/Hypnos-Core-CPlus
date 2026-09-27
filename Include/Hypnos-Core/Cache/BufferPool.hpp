#pragma once

#include <Hypnos-Core/Base/Memory/MemoryAllocatePolicy.hpp>
#include <Hypnos-Core/Base/Memory/MemoryUtils.hpp>

namespace Blanketmen {

template<typename TAllocPolicy>
class BufferPool
{
public:
    BufferPool(size_t buf_size)
    {
        size = MemoryUtils::AlignUp(buf_size, alignof(byte*));
    }

    ~BufferPool()
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

    BufferPool(const BufferPool&) = delete;
    BufferPool& operator=(const BufferPool&) = delete;

    BufferPool(BufferPool&&) = delete;
    BufferPool& operator=(BufferPool&&) = delete;

    size_t Capacity() const noexcept { return capacity; }

    Status<void> Allocate(size_t count)
    {
        size_t alloc_size = size * count;
        byte* ptr = TAllocPolicy::Allocate(alloc_size);
        if (ptr == nullptr)
        {
            return Status<void>::Error(ErrorCode::OutOfMemory, "BufferPool allocation failed.");
        }

        void* bytes = MemoryUtils::Allocate<Chunk>(sizeof(Chunk));
        if (bytes == nullptr)
        {
            TAllocPolicy::Deallocate(ptr, alloc_size);
            return Status<void>::Error(ErrorCode::OutOfMemory, "BufferPool allocation failed.");
        }

        count = alloc_size / size;
        capacity += count;

        Chunk* chunk = static_cast<Chunk*>(bytes);
        chunk->ptr = ptr;
        chunk->size = alloc_size;
        chunk->next = chunks;
        chunks = chunk;

        for (size_t i = 0; i < count - 1; ++i)
        {
            NextOf(ptr + i * size) = ptr + (i + 1) * size;
        }
        NextOf(ptr + (count - 1) * size) = free_nodes;
        free_nodes = ptr;
        return Status<void>::Success();
    }

    byte* Acquire()
    {
        assert(capacity > 0 && "BufferPool is not allocated.");
        if (free_nodes == nullptr && Allocate(capacity).IsFailed())
        {
            return nullptr;
        }

        byte* buf = free_nodes;
        free_nodes = NextOf(free_nodes);
        return buf;
    }

    void Release(byte* buf)
    {
        assert(buf != nullptr && "Cannot release a null buffer.");
        NextOf(buf) = free_nodes;
        free_nodes = buf;
    }

private:
    struct Chunk
    {
        Chunk* next;
        byte* ptr;
        size_t size;
    };

    size_t size = 0;

    size_t capacity = 0;
    Chunk* chunks = nullptr;
    byte* free_nodes = nullptr;

    static byte*& NextOf(byte* buf) noexcept
    {
        return *reinterpret_cast<byte**>(buf);
    }
};

using HeapBufferPool = BufferPool<HeapAllocatePolicy>;
using MmapBufferPool = BufferPool<MmapAllocatePolicy<0>>;

} // namespace Blanketmen