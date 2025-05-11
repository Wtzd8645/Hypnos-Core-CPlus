cmake_minimum_required (VERSION 3.12)
project(Hypnos-Kernel VERSION 0.0.1 LANGUAGES CXX)

if(NOT CMAKE_CXX_STANDARD OR CMAKE_CXX_STANDARD LESS 20)
    message(FATAL_ERROR "Hypnos-Kernel requires C++20. Please set CMAKE_CXX_STANDARD to 20 or higher.")
endif()

add_library(Hypnos-Kernel INTERFACE)

set_target_properties(Hypnos-Kernel PROPERTIES
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS OFF
    LINKER_LANGUAGE CXX
)

target_include_directories(Hypnos-Kernel INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_precompile_headers(Hypnos-Kernel INTERFACE
    "include/Hypnos-Kernel/Logging.hpp"
    "include/Hypnos-Kernel/Platform.hpp"
)

target_compile_options(Hypnos-Kernel INTERFACE
    -includeHypnos-Kernel/Logging.hpp
    -includeHypnos-Kernel/Platform.hpp)

# Set the target properties
install(TARGETS Hypnos-Kernel
    EXPORT HypnosKernelTargets
)

# Install the headers
install(DIRECTORY include/ DESTINATION include)

# Install the export set for the target
install(EXPORT HypnosKernelTargets
    FILE HypnosKernelTargets.cmake
    NAMESPACE Blanketmen::
    DESTINATION lib/cmake/Hypnos-Kernel
)

# Generate the config file
include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/HypnosKernelConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/HypnosKernelConfig.cmake"
    INSTALL_DESTINATION lib/cmake/Hypnos-Kernel
)

# Generate the version file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/HypnosKernelConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMinorVersion
)

# Install the config and version files
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/HypnosKernelConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/HypnosKernelConfigVersion.cmake"
    DESTINATION lib/cmake/Hypnos-Kernel
)