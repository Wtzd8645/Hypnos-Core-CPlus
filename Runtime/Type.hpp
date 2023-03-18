#ifndef TYPE_HPP_
#define TYPE_HPP_

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
typedef signed char                          int8;
typedef unsigned char                        uint8;
typedef signed short int                     int16;
typedef unsigned short int                   uint16;
typedef signed int                           int32;
typedef unsigned int                         uint32;
typedef signed long long int                 int64;
typedef unsigned long long int               uint64;
typedef float                                float32;
typedef double                               float64;
typedef std::string                          string;

typedef char*                                char_ptr;
typedef signed char*                         int8_ptr;
typedef unsigned char*                       uint8_ptr;
typedef signed short int*                    int16_ptr;
typedef unsigned short int*                  uint16_ptr;
typedef signed int*                          int32_ptr;
typedef unsigned int*                        uint32_ptr;
typedef signed long long int*                int64_ptr;
typedef unsigned long long int*              uint64_ptr;
typedef float*                               float32_ptr;
typedef double*                              float64_ptr;
typedef std::string*                         string_ptr;

typedef const char*                          const_char_ptr;
typedef const char*                          const_char_ptr;
typedef const signed char*                   const_int8_ptr;
typedef const unsigned char*                 const_uint8_ptr;
typedef const signed short int*              const_int16_ptr;
typedef const unsigned short int*            const_uint16_ptr;
typedef const signed int*                    const_int32_ptr;
typedef const unsigned int*                  const_uint32_ptr;
typedef const signed long long int*          const_int64_ptr;
typedef const unsigned long long int*        const_uint64_ptr;
typedef const float*                         const_float32_ptr;
typedef const double*                        const_float64_ptr;

typedef char* const                          char_ptr_const;
typedef char* const                          char_ptr_const;
typedef signed char* const                   int8_ptr_const;
typedef unsigned char* const                 uint8_ptr_const;
typedef signed short int* const              int16_ptr_const;
typedef unsigned short int* const            uint16_ptr_const;
typedef signed int* const                    int32_ptr_const;
typedef unsigned int* const                  uint32_ptr_const;
typedef signed long long int* const          int64_ptr_const;
typedef unsigned long long int* const        uint64_ptr_const;
typedef float* const                         float32_ptr_const;
typedef double* const                        float64_ptr_const;

typedef const char* const                    const_char_ptr_const;
typedef const char* const                    const_char_ptr_const;
typedef const signed char* const             const_int8_ptr_const;
typedef const unsigned char* const           const_uint8_ptr_const;
typedef const signed short int* const        const_int16_ptr_const;
typedef const unsigned short int* const      const_uint16_ptr_const;
typedef const signed int* const              const_int32_ptr_const;
typedef const unsigned int* const            const_uint32_ptr_const;
typedef const signed long long int* const    const_int64_ptr_const;
typedef const unsigned long long int* const  const_uint64_ptr_const;
typedef const float* const                   const_float32_ptr_const;
typedef const double* const                  const_float64_ptr_const;

} // namespace Hypnos
} // namespace Blanketmen

#endif // TYPE_HPP_