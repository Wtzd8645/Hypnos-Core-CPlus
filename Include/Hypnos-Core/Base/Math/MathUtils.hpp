#pragma once

namespace Blanketmen {

struct MathUtils
{
    inline static constexpr size_t RoundUpToPowerOfTwo(size_t val) noexcept
    {
        if (val == 0)
        {
            return 1;
        }

        --val;
        val |= val >> 1;
        val |= val >> 2;
        val |= val >> 4;
        val |= val >> 8;
        val |= val >> 16;
        if constexpr (sizeof(size_t) == 8)
        {
            val |= val >> 32;
        }
        return ++val;
    }
};

} // namespace Blanketmen