cmake_minimum_required (VERSION 3.12)
project(Hypnos-Core VERSION 0.0.1 LANGUAGES CXX)

if(NOT CMAKE_CXX_STANDARD OR CMAKE_CXX_STANDARD LESS 20)
    message(FATAL_ERROR "Hypnos-Core requires C++20. Please set CMAKE_CXX_STANDARD to 20 or higher.")
endif()

add_library(Hypnos-Core INTERFACE)

set_target_properties(Hypnos-Core PROPERTIES
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS OFF
    LINKER_LANGUAGE CXX
)

target_include_directories(Hypnos-Core INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_precompile_headers(Hypnos-Core INTERFACE
    "include/Hypnos-Core/Logging.hpp"
    "include/Hypnos-Core/Platform.hpp"
)

target_compile_options(Hypnos-Core INTERFACE
    -includeHypnos-Core/Logging.hpp
    -includeHypnos-Core/Platform.hpp)

# Set the target properties
install(TARGETS Hypnos-Core
    EXPORT HypnosCoreTargets
)

# Install the headers
install(DIRECTORY include/ DESTINATION include)

# Install the export set for the target
install(EXPORT HypnosCoreTargets
    FILE HypnosCoreTargets.cmake
    NAMESPACE Blanketmen::
    DESTINATION lib/cmake/Hypnos-Core
)

# Generate the config file
include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/HypnosCoreConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/HypnosCoreConfig.cmake"
    INSTALL_DESTINATION lib/cmake/Hypnos-Core
)

# Generate the version file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/HypnosCoreConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMinorVersion
)

# Install the config and version files
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/HypnosCoreConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/HypnosCoreConfigVersion.cmake"
    DESTINATION lib/cmake/Hypnos-Core
)