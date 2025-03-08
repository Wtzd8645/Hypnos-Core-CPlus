#pragma once

#include <cstddef>
#include <string>

namespace Blanketmen {
namespace Hypnos {

// Pre-defined Compiler Macros: https://sourceforge.net/p/predef/wiki/OperatingSystems/
#if defined _WIN32      // Windows x86 & x64 (MSDN Predefined macros)

#elif defined __APPLE__ // Mac OS X (Defined by GNU C and Intel C++)

#elif defined __linux__ // GNU/Linux and Android

#elif defined __unix__  // All UNIX (Not all compilers defines these macros, e.g. the xlC or the DEC C/C++ compiler)

#endif

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

} // namespace Hypnos
} // namespace Blanketmen