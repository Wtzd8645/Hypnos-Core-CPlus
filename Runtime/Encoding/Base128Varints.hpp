#ifndef BASE_128_VARINTS_HPP_
#define BASE_128_VARINTS_HPP_

#include "../Type.hpp"

namespace Blanketmen {
namespace Hypnos {
namespace Encoding {

struct Base128Varints
{
    inline static void WriteUInt32(uint32 value, uint8_ptr buffer, int32& offset)
    {
        while (value > 127u)
        {
            *(buffer + offset++) = (uint8)((value & 0x7F) | 0x80);
            value >>= 7;
        }
        *(buffer + offset++) = (uint8)(value & 0x7F);
    }

    inline static void WriteUInt64(uint64 value, uint8_ptr buffer, int32& offset)
    {
        while (value > 127ul)
        {
            *(buffer + offset++) = (uint8)((value & 0x7F) | 0x80);
            value >>= 7;
        }
        *(buffer + offset++) = (uint8)(value & 0x7F);
    }

    inline static uint32 ReadUInt32(uint8_ptr buffer, int32& offset)
    {
        int32 tmp = *(buffer + offset++);
        if (tmp < 128)
        {
            return (uint32)tmp;
        }

        int32 result = tmp & 0x7f;
        if ((tmp = *(buffer + offset++)) < 128)
        {
            result |= tmp << 7;
            return (uint32)result;
        }

        result |= (tmp & 0x7f) << 7;
        if ((tmp = *(buffer + offset++)) < 128)
        {
            result |= tmp << 14;
            return (uint32)result;
        }

        result |= (tmp & 0x7f) << 14;
        if ((tmp = *(buffer + offset++)) < 128)
        {
            result |= tmp << 21;
            return (uint32)result;
        }

        result |= (tmp & 0x7f) << 21;
        result |= (tmp = *(buffer + offset++)) << 28;
        if (tmp < 128)
        {
            return (uint32)result;
        }

        // If larger than 32 bits, discard the upper 32 bits.
        for (int32 i = 0; i < 5; ++i)
        {
            if (*(buffer + offset++) < 128)
            {
                return (uint32)result;
            }
        }

        //throw new IOException("Encountered a malformed varint.");
        return 0ul;
    }

    inline static uint64 ReadUInt64(uint8_ptr buffer, int32& offset)
    {
        uint64 result = *(buffer + offset++);
        if (result < 128)
        {
            return result;
        }

        result &= 0x7f;
        int32 shift = 7;
        do
        {
            int32 tmp = *(buffer + offset++);
            result |= (uint64)(tmp & 0x7F) << shift;
            if (tmp < 128)
            {
                return result;
            }
            shift += 7;
        } while (shift < 64);

        //throw new IOException("Encountered a malformed varint.");
        return 0ul;
    }
};

} // namespace Encoding
} // namespace Hypnos
} // namespace Blanketmen

#endif // BASE_128_VARINTS_HPP_