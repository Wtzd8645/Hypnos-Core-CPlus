#pragma once

#include "Hypnos-Core/Types.hpp"

namespace Blanketmen {
namespace Hypnos {
namespace Memory {

inline constexpr uint32 AlignUp(uint32 size, uint32 alignment) noexcept
{
    return (size + alignment - 1) & ~(alignment - 1);
}

inline constexpr uint32 AlignDown(uint32 size, uint32 alignment) noexcept
{
    return size & ~(alignment - 1);
}

} // namespace Memory
} // namespace Hypnos
} // namespace Blanketmen