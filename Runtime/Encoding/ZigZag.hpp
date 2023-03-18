#ifndef ZIG_ZAG_HPP_
#define ZIG_ZAG_HPP_

#include "../Type.hpp"

namespace Blanketmen {
namespace Hypnos {
namespace Encoding {

struct ZigZag
{
    inline static uint32 Encode(int32 n)
    {
        return (uint32)((n << 1) ^ (n >> 31));
    }

    inline static uint64 Encode(int64 n)
    {
        return (uint64)((n << 1) ^ (n >> 63));
    }

    inline static int32 Decode(uint32 n)
    {
        return (int32)(n >> 1) ^ -(int32)(n & 1u);
    }

    inline static int64 Decode(uint64 n)
    {
        return (int64)(n >> 1) ^ -(int64)(n & 1u);
    }
};

} // namespace Encoding
} // namespace Hypnos
} // namespace Blanketmen

#endif // ZIG_ZAG_HPP_