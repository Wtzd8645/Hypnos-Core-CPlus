cmake_minimum_required (VERSION 3.12)

if (NOT DEFINED CMAKE_CXX_COMPILER)
    set(CMAKE_CXX_COMPILER clang++)
endif ()
project(Hypnos-Kernel CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG           "/MTd /Zi /Od /Ob0 /RTC1 /DDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "/MT /Zi /O2 /Ob1 /DDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE         "/MT /O2 /Ob2 /DNDEBUG")
    set(CMAKE_CXX_FLAGS_MINSIZEREL      "/MT /O1 /Ob2 /DNDEBUG")
else ()
    set(CMAKE_CXX_FLAGS_DEBUG           "-g -O0 -DDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "-g -O2 -DDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE         "-O3 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_MINSIZEREL      "-Os -DNDEBUG")
endif ()

add_library(Hypnos-Kernel STATIC "Include/Hypnos-Kernel/Base/Logging_Unix.cpp")

set_target_properties(Hypnos-Kernel PROPERTIES LINKER_LANGUAGE CXX)

target_compile_options(Hypnos-Kernel PRIVATE -stdlib=libc++)

target_link_options(Hypnos-Kernel PRIVATE -stdlib=libc++)

target_include_directories(Hypnos-Kernel INTERFACE "Include" "Runtime")

target_precompile_headers(Hypnos-Kernel INTERFACE "Runtime/Platform.hpp")