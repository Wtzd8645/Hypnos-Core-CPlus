#ifndef BINARY_WRITER_HPP_
#define BINARY_WRITER_HPP_

#include "../Type.hpp"

namespace Blanketmen {
namespace Hypnos {
namespace IO {

inline void WriteInt8(int8& value, char_ptr buffer, size_t offset)
{
    *reinterpret_cast<int8_ptr>(buffer + offset) = value;
    offset += sizeof(int8);
}

inline void WriteUInt8(uint8& value, char_ptr buffer, size_t offset)
{
    *reinterpret_cast<uint8_ptr>(buffer + offset) = value;
    offset += sizeof(uint8);
}

inline void WriteInt16(int16& value, char_ptr buffer, size_t offset)
{
    (short*)buffer;
    *reinterpret_cast<int16_ptr>(buffer + offset) = value;
    offset += sizeof(int16);
}

inline void WriteUInt16(uint16& value, char_ptr buffer, size_t offset)
{
    *reinterpret_cast<uint16_ptr>(buffer + offset) = value;
    offset += sizeof(uint16);
}

inline void WriteInt32(int32& value, char_ptr buffer, size_t offset)
{
    *reinterpret_cast<int32_ptr>(buffer + offset) = value;
    offset += sizeof(int32);
}

inline void WriteUInt32(uint32& value, char_ptr buffer, size_t offset)
{
    *reinterpret_cast<uint32_ptr>(buffer + offset) = value;
    offset += sizeof(uint32);
}

inline void WriteInt64(int64& value, char_ptr buffer, size_t offset)
{
    *reinterpret_cast<int64_ptr>(buffer + offset) = value;
    offset += sizeof(int64);
}

inline void WriteUInt64(uint64& value, char_ptr buffer, size_t offset)
{
    *reinterpret_cast<uint64_ptr>(buffer + offset) = value;
    offset += sizeof(uint64);
}

inline void WriteFloat32(float32& value, char_ptr buffer, size_t offset)
{
    *reinterpret_cast<float32_ptr>(buffer + offset) = value;
    offset += sizeof(float32);
}

inline void WriteFloat64(float64& value, char_ptr buffer, size_t offset)
{
    *reinterpret_cast<float64_ptr>(buffer + offset) = value;
    offset += sizeof(float64);
}

inline void WriteString(string& value, char_ptr buffer, size_t offset)
{
    uint16 length = value.length();
    *reinterpret_cast<uint16_ptr>(buffer + offset) = length;
    offset += sizeof(uint16);

    ::strcpy_s(reinterpret_cast<char_ptr>(buffer + offset), length, value.c_str());
    offset += length;
}

} // namespace IO
} // namespace Hypnos
} // namespace Blanketmen

#endif // BINARY_WRITER_HPP_