#pragma once

#include <cstddef>
#include <new>

namespace Blanketmen {

struct MemoryUtils
{
    static constexpr size_t PAGE_SIZE = 4096;                 // 4KB standard page
    static constexpr size_t HUGE_PAGE_SIZE = 2 * 1024 * 1024; // 2MB huge page

    [[nodiscard]] inline static void* Allocate(size_t size, size_t alignment) noexcept
    {
        return ::operator new(size, std::align_val_t { alignment }, std::nothrow);
    }

    inline static void Deallocate(void* ptr, size_t alignment)
    {
        ::operator delete(ptr, std::align_val_t { alignment });
    }

    template<class T>
    [[nodiscard]] static void* Allocate(size_t size) noexcept
    {
        return ::operator new(size, std::align_val_t { alignof(T) }, std::nothrow);
    }
    
    template<class T>
    static void Deallocate(void* ptr)
    {
        ::operator delete(ptr, std::align_val_t { alignof(T) });
    }

    /// <summary>
    /// Aligns the given size value upwards to the nearest multiple of the specified alignment.
    /// </summary>
    /// <param name="size">The size value to align.</param>
    /// <param name="alignment">The alignment boundary.</param>
    /// <returns>The aligned size value.</returns>
    inline static constexpr size_t AlignUp(size_t size, size_t alignment) noexcept
    {
        return (size + alignment - 1) & ~(alignment - 1);
    }

    /// <summary>
    /// Aligns the given size value downwards to the nearest multiple of the specified alignment.
    /// </summary>
    /// <param name="size">The size value to align.</param>
    /// <param name="alignment">The alignment boundary.</param>
    /// <returns>The aligned size value.</returns>
    inline static constexpr size_t AlignDown(size_t size, size_t alignment) noexcept
    {
        return size & ~(alignment - 1);
    }
};

} // namespace Blanketmen