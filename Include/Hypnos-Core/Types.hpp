#pragma once

#include <cstddef>
#include <string>
#include <memory>

namespace Blanketmen {
namespace Hypnos {

// DataModel: https://en.cppreference.com/w/cpp/language/types
// NOTE: Signedness char represents a character, and shouldn't care whether it is signed or unsigned.
typedef char                      char8;
typedef signed char               int8;
typedef unsigned char             uint8;
typedef std::byte                 byte;
typedef signed short int          int16;
typedef unsigned short int        uint16;
typedef signed int                int32;
typedef unsigned int              uint32;
typedef signed long long int      int64;
typedef unsigned long long int    uint64;
typedef float                     float32;
typedef double                    float64;
typedef std::string               string;

template<typename T>
using unique_ptr = std::unique_ptr<T>;

template<typename T>
using shared_ptr = std::shared_ptr<T>;

template<typename T>
using weak_ptr = std::weak_ptr<T>;

} // namespace Hypnos
} // namespace Blanketmen