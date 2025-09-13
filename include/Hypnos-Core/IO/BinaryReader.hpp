#pragma once

#include <cstring>

namespace Blanketmen {

struct BinaryReader
{
    inline static void ReadInt8(int8& result, uint8* buffer)
    {
        std::memcpy(&result, buffer, sizeof(int8));
    }

    inline static void ReadUInt8(uint8& result, uint8* buffer)
    {
        std::memcpy(&result, buffer, sizeof(uint8));
    }

    inline static void ReadInt16(int16& result, uint8* buffer)
    {
        std::memcpy(&result, buffer, sizeof(int16));
    }

    inline static void ReadUInt16(uint16& result, uint8* buffer)
    {
        std::memcpy(&result, buffer, sizeof(uint16));
    }

    inline static void ReadInt32(int32& result, uint8* buffer)
    {
        std::memcpy(&result, buffer, sizeof(int32));
    }

    inline static void ReadUInt32(uint32& result, uint8* buffer)
    {
        std::memcpy(&result, buffer, sizeof(uint32));
    }

    inline static void ReadInt64(int64& result, uint8* buffer)
    {
        std::memcpy(&result, buffer, sizeof(int64));
    }

    inline static void ReadUInt64(uint64& result, uint8* buffer)
    {
        std::memcpy(&result, buffer, sizeof(uint64));
    }

    inline static void ReadFloat32(float32& result, uint8* buffer)
    {
        std::memcpy(&result, buffer, sizeof(float32));
    }

    inline static void ReadFloat64(float64& result, uint8* buffer)
    {
        std::memcpy(&result, buffer, sizeof(float64));
    }

    inline static void ReadString(string& result, uint8* buffer)
    {
        size_t len;
        std::memcpy(&len, buffer, sizeof(size_t));
        result.assign(reinterpret_cast<char*>(buffer + sizeof(size_t)), len);
    }
};

} // namespace Blanketmen