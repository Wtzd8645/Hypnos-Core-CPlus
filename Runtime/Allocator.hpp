#ifndef ALLOCATOR_HPP_
#define ALLOCATOR_HPP_

#include <cstdlib>
#include <new>

namespace Blanketmen {
namespace Hypnos {

constexpr size_t MIN_CHUNK_SIZE = sizeof(void*);
constexpr size_t MAX_CHUNK_SIZE = 128;
constexpr size_t BLOCK_SIZE = 1024;

static char* FreeChunks[MAX_CHUNK_SIZE / MIN_CHUNK_SIZE];

// TODO: Check allocate algorithm.
template <class T>
class MemoryPoolAllocator
{
public:
    typedef T value_type;

    const size_t MAX_OBJECT_SIZE = static_cast<size_t>(-1) / sizeof(T);

    MemoryPoolAllocator() noexcept { }

    template<class U> MemoryPoolAllocator(const MemoryPoolAllocator<U>&) noexcept { }
    template<class U> bool operator==(const MemoryPoolAllocator<U>&) const noexcept { return true; }
    template<class U> bool operator!=(const MemoryPoolAllocator<U>&) const noexcept { return false; }

    T* allocate(const size_t n) const
    {
        if (n == 0 || n > MAX_OBJECT_SIZE)
        {
            throw std::bad_array_new_length();
        }

        size_t size = n * sizeof(T);
        if (size > MAX_CHUNK_SIZE)
        {
            // printf("[MemoryPoolAllocator] allocate: %d\n", size);
            void* const ptr = malloc(size);
            if (ptr == nullptr)
            {
                throw std::bad_alloc();
            }
            return reinterpret_cast<T*>(ptr);
        }

        size_t chunkIndex = size / MIN_CHUNK_SIZE;
        if (FreeChunks[chunkIndex] == nullptr)
        {
            char* block = reinterpret_cast<char*>(malloc(BLOCK_SIZE));
            if (block == nullptr)
            {
                throw std::bad_alloc();
            }

            size_t chunkSize = chunkIndex * MIN_CHUNK_SIZE + MIN_CHUNK_SIZE;
            size_t chunkNum = BLOCK_SIZE / chunkSize; // NOTE: It will sacrifice a little space.
            do // Divide block into chunk and concatenate them.
            {
                *reinterpret_cast<char**>(block) = FreeChunks[chunkIndex];
                FreeChunks[chunkIndex] = block;
                block += chunkSize;
            } while (--chunkNum > 0);
        }

        char* ptr = FreeChunks[chunkIndex];
        FreeChunks[chunkIndex] = *reinterpret_cast<char**>(ptr);
        // printf("[MemoryPoolAllocator] allocate: n = %d, index = %d, ptr: %p\n", n, chunkIndex, ptr);
        return reinterpret_cast<T*>(ptr);
    }

    void deallocate(T* const ptr, size_t n) const noexcept
    {
        // printf("[MemoryPoolAllocator] deallocate: n: %d, ptr: %p\n", n, ptr);
        size_t size = n * sizeof(T);
        if (size > MAX_CHUNK_SIZE)
        {
            free(ptr);
            return;
        }

        size_t chunkIndex = size / MIN_CHUNK_SIZE;
        *reinterpret_cast<char**>(ptr) = FreeChunks[chunkIndex];
        FreeChunks[chunkIndex] = reinterpret_cast<char*>(ptr);
    }
};

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

#endif // ALLOCATOR_HPP_