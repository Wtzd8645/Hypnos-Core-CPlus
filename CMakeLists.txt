cmake_minimum_required (VERSION 3.12)

project(Hypnos-Core VERSION 0.0.1 LANGUAGES CXX)

include(GNUInstallDirs)

add_library(Hypnos-Core INTERFACE)

target_compile_features(Hypnos-Core INTERFACE cxx_std_20)

target_compile_options(Hypnos-Core INTERFACE
    -includeHypnos-Core/Base/Logging.hpp
    -includeHypnos-Core/Base/Platform.hpp)

target_include_directories(Hypnos-Core INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_precompile_headers(Hypnos-Core INTERFACE
    "<Hypnos-Core/Base/Logging.hpp>"
    "<Hypnos-Core/Base/Platform.hpp>"
)

if (CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    include("${CMAKE_CURRENT_SOURCE_DIR}/CMake/Install.cmake")
endif()
