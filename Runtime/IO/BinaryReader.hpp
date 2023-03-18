#ifndef BINARY_READER_HPP_
#define BINARY_READER_HPP_

#include "../Type.hpp"

namespace Blanketmen {
namespace Hypnos {
namespace IO {

inline void ReadInt8(int8& result, char_ptr buffer, size_t offset)
{
    result = *reinterpret_cast<int8_ptr>(buffer + offset);
    offset += sizeof(int8);
}

inline void ReadUInt8(uint8& result, char_ptr buffer, size_t offset)
{
    result = *reinterpret_cast<uint8_ptr>(buffer + offset);
    offset += sizeof(uint8);
}

inline void ReadInt16(int16& result, char_ptr buffer, size_t offset)
{
    result = *reinterpret_cast<int16_ptr>(buffer + offset);
    offset += sizeof(int16);
}

inline void ReadUInt16(uint16& result, char_ptr buffer, size_t offset)
{
    result = *reinterpret_cast<uint16_ptr>(buffer + offset);
    offset += sizeof(uint16);
}

inline void ReadInt32(int32& result, char_ptr buffer, size_t offset)
{
    result = *reinterpret_cast<int32_ptr>(buffer + offset);
    offset += sizeof(int32);
}

inline void ReadUInt32(uint32& result, char_ptr buffer, size_t offset)
{
    result = *reinterpret_cast<uint32_ptr>(buffer + offset);
    offset += sizeof(uint32);
}

inline void ReadInt64(int64& result, char_ptr buffer, size_t offset)
{
    result = *reinterpret_cast<int64_ptr>(buffer + offset);
    offset += sizeof(int64);
}

inline void ReadUInt64(uint64& result, char_ptr buffer, size_t offset)
{
    result = *reinterpret_cast<uint64_ptr>(buffer + offset);
    offset += sizeof(uint64);
}

inline void ReadFloat32(float32& result, char_ptr buffer, size_t offset)
{
    result = *reinterpret_cast<float32_ptr>(buffer + offset);
    offset += sizeof(float32);
}

inline void ReadFloat64(float64& result, char_ptr buffer, size_t offset)
{
    result = *reinterpret_cast<float64_ptr>(buffer + offset);
    offset += sizeof(float64);
}

inline void ReadString(string& result, char_ptr buffer, size_t offset)
{
    int len = *reinterpret_cast<uint16_ptr>(buffer + offset);
    offset += sizeof(uint16);
    result.assign(reinterpret_cast<char_ptr>(buffer + offset), len);
    offset += len;
}

} // namespace IO
} // namespace Hypnos
} // namespace Blanketmen

#endif // BINARY_READER_HPP_