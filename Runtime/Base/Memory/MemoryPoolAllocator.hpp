#pragma once

#include "MemoryUtils.hpp"
#include <cstdlib>
#include <new>

namespace Blanketmen {
namespace Hypnos {
namespace Memory {

typedef unsigned char* chunk_ptr;

static constexpr size_t MIN_CHUNK_SIZE = sizeof(chunk_ptr);
static constexpr size_t MAX_CHUNK_SIZE = 128;
static constexpr size_t BLOCK_SIZE = 4096;

static chunk_ptr free_chunk_buckets[MAX_CHUNK_SIZE / MIN_CHUNK_SIZE];

template <class T>
class MemoryPoolAllocator
{
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    template <class U>
    struct rebind
    {
        typedef MemoryPoolAllocator<U> other;
    };

    static constexpr size_t ALIGNED_SIZE = AlignUp(sizeof(T), alignof(T));
    static constexpr size_t MAX_OBJECT_NUM = static_cast<size_t>(-1) / ALIGNED_SIZE;
    static constexpr size_t MAX_ALLOCATE_NUM = MAX_CHUNK_SIZE / ALIGNED_SIZE;

    MemoryPoolAllocator() noexcept { }
    MemoryPoolAllocator(const MemoryPoolAllocator& other) noexcept { }
    template<class U> MemoryPoolAllocator(const MemoryPoolAllocator<U>&) noexcept { }

    template<class U> bool operator==(const MemoryPoolAllocator<U>&) const noexcept { return true; }
    template<class U> bool operator!=(const MemoryPoolAllocator<U>&) const noexcept { return false; }

    size_type max_size() const noexcept { return size_type(-1) / sizeof(T); }

    pointer address(reference x) const noexcept { return std::addressof(x); }

    const_pointer address(const_reference x) const noexcept { return std::addressof(x); }

    pointer allocate(size_type n, const void* hint = 0) const
    {
        if (n == 0 || n > MAX_OBJECT_NUM)
        {
            throw std::bad_array_new_length();
        }

        size_t size = n * ALIGNED_SIZE;
        if (size > MAX_CHUNK_SIZE)
        {
            // printf("[MemoryPoolAllocator] allocate: %d\n", size);
            void* const chunk = malloc(size);
            return chunk != nullptr ? static_cast<T*>(chunk) : throw std::bad_alloc();
        }

        size_t bucket = (size - 1) / MIN_CHUNK_SIZE;
        if (free_chunk_buckets[bucket] == nullptr)
        {
            chunk_ptr block = static_cast<chunk_ptr>(malloc(BLOCK_SIZE));
            if (block == nullptr)
            {
                throw std::bad_alloc();
            }

            size_t chunk_size = (bucket + 1) * MIN_CHUNK_SIZE;
            size_t chunk_count = BLOCK_SIZE / chunk_size; // NOTE: It will sacrifice a little space.
            do // Divide block into chunk and concatenate them.
            {
                *reinterpret_cast<chunk_ptr*>(block) = free_chunk_buckets[bucket];
                free_chunk_buckets[bucket] = block;
                block += chunk_size;
            }
            while (--chunk_count > 0);
        }

        chunk_ptr chunk = free_chunk_buckets[bucket];
        free_chunk_buckets[bucket] = *reinterpret_cast<chunk_ptr*>(chunk);
        // printf("[MemoryPoolAllocator] allocate: n = %d, index = %d, ptr: %p\n", n, bucket, chunk);
        return reinterpret_cast<T*>(chunk);
    }

    void deallocate(pointer chunk, size_type n) const noexcept
    {
        // printf("[MemoryPoolAllocator] deallocate: n: %d, ptr: %p\n", n, chunk);
        if (n > MAX_ALLOCATE_NUM)
        {
            free(chunk);
            return;
        }

        size_t bucket = (n * ALIGNED_SIZE - 1) / MIN_CHUNK_SIZE;
        *reinterpret_cast<chunk_ptr*>(chunk) = free_chunk_buckets[bucket];
        free_chunk_buckets[bucket] = reinterpret_cast<chunk_ptr>(chunk);
    }

    void construct(pointer p, const_reference val) { new(static_cast<void*>(p)) T(val); }

    void destroy(pointer p) { p->~T(); }
};

} // namespace Memory
} // namespace Hypnos
} // namespace Blanketmen

// void* operator new(std::size_t size)
// {
//     std::cout << "operator new: " << size << "byte" << std::endl;
//     void* ptr = MemoryAllocator::Allocate(size);
//     if (ptr == nullptr)
//     {
//         throw std::bad_alloc();
//     }
//     return ptr;
// }
// 
// void* operator new(std::size_t size, const std::nothrow_t&) noexcept
// {
//     std::cout << "operator new noexcept: " << size << "byte" << std::endl;
//     return MemoryAllocator::Allocate(size);
// }
// 
// void* operator new[](std::size_t size)
// {
//     std::cout << "operator new[]: " << size << "byte" << std::endl;
//     void* ptr = MemoryAllocator::Allocate(size);
//     if (ptr == nullptr)
//     {
//         throw std::bad_alloc();
//     }
//     return ptr;
// }
// 
// void* operator new[](std::size_t size, const std::nothrow_t&) noexcept
// {
//     std::cout << "operator new[] noexcept: " << size << "byte" << std::endl;
//     return MemoryAllocator::Allocate(size);
// }
// 
// void operator delete(void* ptr, size_t size) noexcept
// {
//     std::cout << "operator delete: " << ptr << ", size: " << size << std::endl;
//     MemoryAllocator::Deallocate(ptr, size);
// }
// 
// void operator delete(void* ptr, size_t size, const std::nothrow_t&) noexcept
// {
//     std::cout << "operator delete noexcept: " << ptr << ", size: " << size << std::endl;
//     MemoryAllocator::Deallocate(ptr, size);
// }
// 
// void operator delete[](void* ptr) noexcept
// {
//     std::cout << "operator delete[]: " << ptr << std::endl;
//     // MemoryAllocator::Deallocate(ptr, size);
// }
// 
// void operator delete[](void* ptr, size_t size) noexcept
// {
//     std::cout << "operator delete[]: " << ptr << ", size: " << size << std::endl;
//     MemoryAllocator::Deallocate(ptr, size);
// }
// 
// void operator delete[](void* ptr, size_t size, const std::nothrow_t&) noexcept
// {
//     std::cout << "operator delete[] noexcept: " << ptr << ", size: " << size << std::endl;
//     MemoryAllocator::Deallocate(ptr, size);
// }

// namespace Blanketmen {
// namespace Hypnos {
// 
// uint32 MemoryAllocator::sizeThreshold = 0;
// size_t MemoryAllocator::allocateCount = 0;
// MemoryPool* MemoryAllocator::pools = nullptr;
// 
// void MemoryAllocator::Initialize(uint32 sizeThreshold)
// {
//     MemoryAllocator::sizeThreshold = sizeThreshold;
//     MemoryAllocator::pools = (MemoryPool*)calloc(sizeThreshold, sizeof(MemoryPool));
// }
// 
// inline void* MemoryAllocator::Allocate(size_t size)
// {
//     ++MemoryAllocator::allocateCount;
//     if (size > MemoryAllocator::sizeThreshold)
//     {
//         return malloc(size);
//     }
// 
//     MemoryPool* pool = (MemoryAllocator::pools + size);
//     if (pool->Capacity() == 0)
//     {
//         new(pool) MemoryPool(size, 1024 / (size < sizeof(char_ptr) ? sizeof(char_ptr) : size));
//     }
//     return pool->Pop();
// }
// 
// inline void MemoryAllocator::Deallocate(void* ptr, size_t size)
// {
//     --MemoryAllocator::allocateCount;
//     if (size > MemoryAllocator::sizeThreshold)
//     {
//         free(ptr);
//         return;
//     }
// 
//     // NOTE: ���Ʀ^�����ܷ|�Y��
//     (MemoryAllocator::pools + size)->Push(ptr);
// }
// 
// } // namespace Hypnos
// } // namespace Blanketmen