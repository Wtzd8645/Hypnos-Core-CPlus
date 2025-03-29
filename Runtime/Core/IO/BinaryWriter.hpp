#pragma once

#include "Platform.hpp"
#include <cstring>

namespace Blanketmen {
namespace Hypnos {
namespace IO {

inline void WriteInt8(int8& value, uint8* buffer, size_t offset)
{
    *reinterpret_cast<int8*>(buffer + offset) = value;
    offset += sizeof(int8);
}

inline void WriteUInt8(uint8& value, uint8* buffer, size_t offset)
{
    *reinterpret_cast<uint8*>(buffer + offset) = value;
    offset += sizeof(uint8);
}

inline void WriteInt16(int16& value, uint8* buffer, size_t offset)
{
    *reinterpret_cast<int16*>(buffer + offset) = value;
    offset += sizeof(int16);
}

inline void WriteUInt16(uint16& value, uint8* buffer, size_t offset)
{
    *reinterpret_cast<uint16*>(buffer + offset) = value;
    offset += sizeof(uint16);
}

inline void WriteInt32(int32& value, uint8* buffer, size_t offset)
{
    *reinterpret_cast<int32*>(buffer + offset) = value;
    offset += sizeof(int32);
}

inline void WriteUInt32(uint32& value, uint8* buffer, size_t offset)
{
    *reinterpret_cast<uint32*>(buffer + offset) = value;
    offset += sizeof(uint32);
}

inline void WriteInt64(int64& value, uint8* buffer, size_t offset)
{
    *reinterpret_cast<int64*>(buffer + offset) = value;
    offset += sizeof(int64);
}

inline void WriteUInt64(uint64& value, uint8* buffer, size_t offset)
{
    *reinterpret_cast<uint64*>(buffer + offset) = value;
    offset += sizeof(uint64);
}

inline void WriteFloat32(float32& value, uint8* buffer, size_t offset)
{
    *reinterpret_cast<float32*>(buffer + offset) = value;
    offset += sizeof(float32);
}

inline void WriteFloat64(float64& value, uint8* buffer, size_t offset)
{
    *reinterpret_cast<float64*>(buffer + offset) = value;
    offset += sizeof(float64);
}

inline void WriteString(string& value, uint8* buffer, size_t offset)
{
    uint16 len = value.length();
    *reinterpret_cast<uint16*>(buffer + offset) = len;
    offset += sizeof(uint16);

    std::strncpy(reinterpret_cast<char*>(buffer + offset), value.c_str(), len);
    offset += len;
}

} // namespace IO
} // namespace Hypnos
} // namespace Blanketmen