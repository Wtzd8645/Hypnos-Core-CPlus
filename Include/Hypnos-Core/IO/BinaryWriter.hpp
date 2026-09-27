#pragma once

#include <cstring>

namespace Blanketmen {

struct BinaryWriter
{
    inline static void WriteInt8(int8 value, uint8* buffer)
    {
        std::memcpy(buffer, &value, sizeof(int8));
    }

    inline static void WriteUInt8(uint8 value, uint8* buffer)
    {
        std::memcpy(buffer, &value, sizeof(uint8));
    }

    inline static void WriteInt16(int16 value, uint8* buffer)
    {
        std::memcpy(buffer, &value, sizeof(int16));
    }

    inline static void WriteUInt16(uint16 value, uint8* buffer)
    {
        std::memcpy(buffer, &value, sizeof(uint16));
    }

    inline static void WriteInt32(int32 value, uint8* buffer)
    {
        std::memcpy(buffer, &value, sizeof(int32));
    }

    inline static void WriteUInt32(uint32 value, uint8* buffer)
    {
        std::memcpy(buffer, &value, sizeof(uint32));
    }

    inline static void WriteInt64(int64 value, uint8* buffer)
    {
        std::memcpy(buffer, &value, sizeof(int64));
    }

    inline static void WriteUInt64(uint64 value, uint8* buffer)
    {
        std::memcpy(buffer, &value, sizeof(uint64));
    }

    inline static void WriteFloat32(float32 value, uint8* buffer)
    {
        std::memcpy(buffer, &value, sizeof(float32));
    }

    inline static void WriteFloat64(float64 value, uint8* buffer)
    {
        std::memcpy(buffer, &value, sizeof(float64));
    }

    inline static void WriteString(const string& value, uint8* buffer)
    {
        const size_t len = value.length();
        std::memcpy(buffer, &len, sizeof(size_t));
        std::memcpy(buffer + sizeof(size_t), value.data(), len);
    }
};

} // namespace Blanketmen