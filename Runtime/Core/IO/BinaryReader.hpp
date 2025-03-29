#pragma once

#include "Platform.hpp"

namespace Blanketmen {
namespace Hypnos {
namespace IO {

inline void ReadInt8(int8& result, uint8* buffer, size_t offset)
{
    result = *reinterpret_cast<int8*>(buffer + offset);
    offset += sizeof(int8);
}

inline void ReadUInt8(uint8& result, uint8* buffer, size_t offset)
{
    result = *reinterpret_cast<uint8*>(buffer + offset);
    offset += sizeof(uint8);
}

inline void ReadInt16(int16& result, uint8* buffer, size_t offset)
{
    result = *reinterpret_cast<int16*>(buffer + offset);
    offset += sizeof(int16);
}

inline void ReadUInt16(uint16& result, uint8* buffer, size_t offset)
{
    result = *reinterpret_cast<uint16*>(buffer + offset);
    offset += sizeof(uint16);
}

inline void ReadInt32(int32& result, uint8* buffer, size_t offset)
{
    result = *reinterpret_cast<int32*>(buffer + offset);
    offset += sizeof(int32);
}

inline void ReadUInt32(uint32& result, uint8* buffer, size_t offset)
{
    result = *reinterpret_cast<uint32*>(buffer + offset);
    offset += sizeof(uint32);
}

inline void ReadInt64(int64& result, uint8* buffer, size_t offset)
{
    result = *reinterpret_cast<int64*>(buffer + offset);
    offset += sizeof(int64);
}

inline void ReadUInt64(uint64& result, uint8* buffer, size_t offset)
{
    result = *reinterpret_cast<uint64*>(buffer + offset);
    offset += sizeof(uint64);
}

inline void ReadFloat32(float32& result, uint8* buffer, size_t offset)
{
    result = *reinterpret_cast<float32*>(buffer + offset);
    offset += sizeof(float32);
}

inline void ReadFloat64(float64& result, uint8* buffer, size_t offset)
{
    result = *reinterpret_cast<float64*>(buffer + offset);
    offset += sizeof(float64);
}

inline void ReadString(string& result, uint8* buffer, size_t offset)
{
    int len = *reinterpret_cast<uint16*>(buffer + offset);
    offset += sizeof(uint16);
    result.assign(reinterpret_cast<char*>(buffer + offset), len);
    offset += len;
}

} // namespace IO
} // namespace Hypnos
} // namespace Blanketmen