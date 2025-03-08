#pragma once

// Pre-defined Compiler Macros: https://sourceforge.net/p/predef/wiki/OperatingSystems/
#if defined _WIN32      // Windows x86 & x64 (MSDN Predefined macros)

#elif defined __APPLE__ // Mac OS X (Defined by GNU C and Intel C++)

#elif defined __linux__ // GNU/Linux and Android

#elif defined __unix__  // All UNIX (Not all compilers defines these macros, e.g. the xlC or the DEC C/C++ compiler)

#endif

#if defined(__APPLE__)
#define GOOGLE_PROTOBUF_OS_APPLE
#include <Availability.h>
#include <TargetConditionals.h>
#if TARGET_OS_IPHONE
#define GOOGLE_PROTOBUF_OS_IPHONE
#endif
#elif defined(__EMSCRIPTEN__)
#define GOOGLE_PROTOBUF_OS_EMSCRIPTEN
#elif defined(__native_client__)
#define GOOGLE_PROTOBUF_OS_NACL
#elif defined(sun)
#define GOOGLE_PROTOBUF_OS_SOLARIS
#elif defined(_AIX)
#define GOOGLE_PROTOBUF_OS_AIX
#elif defined(__ANDROID__)
#define GOOGLE_PROTOBUF_OS_ANDROID
#endif

#if defined(GOOGLE_PROTOBUF_OS_ANDROID) || defined(GOOGLE_PROTOBUF_OS_IPHONE) || defined(__OpenBSD__)
// Android ndk does not support the __thread keyword very well yet. Here
// we use pthread_key_create()/pthread_getspecific()/... methods for
// TLS support on android.
// iOS and OpenBSD also do not support the __thread keyword.
#define GOOGLE_PROTOBUF_NO_THREADLOCAL
#endif

#if defined(__MAC_OS_X_VERSION_MIN_REQUIRED) && __MAC_OS_X_VERSION_MIN_REQUIRED < 1070
// __thread keyword requires at least 10.7
#define GOOGLE_PROTOBUF_NO_THREADLOCAL
#endif

#if defined(_MSC_VER)
#define THREAD_LOCAL __declspec(thread)
#else
#define THREAD_LOCAL __thread
#endif