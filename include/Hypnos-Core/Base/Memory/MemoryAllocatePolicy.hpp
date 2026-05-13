#pragma once

#include <Hypnos-Core/Base/Memory/MemoryUtils.hpp>
#include <sys/mman.h>

namespace Blanketmen {

struct HeapAllocatePolicy
{
    static byte* Allocate(size_t& size) noexcept
    {
        size = MemoryUtils::AlignUp(size, MemoryUtils::PAGE_SIZE);
        return static_cast<byte*>(MemoryUtils::Allocate(size, alignof(byte*)));
    }

    static void Deallocate(byte* ptr, size_t /*size*/) noexcept
    {
        MemoryUtils::Deallocate(ptr, alignof(byte*));
    }
};

template<int32 Flags>
struct MmapAllocatePolicy
{
    static byte* Allocate(size_t& size) noexcept
    {
        size_t page_unit = (Flags & MAP_HUGETLB) ? MemoryUtils::HUGE_PAGE_SIZE : MemoryUtils::PAGE_SIZE;
        size = MemoryUtils::AlignUp(size, page_unit);
        void* ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | Flags, -1, 0);
        return (ptr != MAP_FAILED) ? static_cast<byte*>(ptr) : nullptr;
    }

    static void Deallocate(byte* ptr, size_t size) noexcept
    {
        munmap(ptr, size);
    }
};

} // namespace Blanketmen