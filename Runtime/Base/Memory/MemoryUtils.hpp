#pragma once

#include <cstddef>

namespace Blanketmen {
namespace Hypnos {

struct MemoryUtils
{
    inline static constexpr size_t AlignUp(size_t size, size_t alignment) noexcept
    {
        return (size + alignment - 1) & ~(alignment - 1);
    }

    inline static constexpr size_t AlignDown(size_t size, size_t alignment) noexcept
    {
        return size & ~(alignment - 1);
    }
};

} // namespace Hypnos
} // namespace Blanketmen