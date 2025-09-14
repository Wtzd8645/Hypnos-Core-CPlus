cmake_minimum_required (VERSION 3.12)

project(Hypnos-Core VERSION 0.0.1 LANGUAGES CXX)

include(GNUInstallDirs)

add_library(Hypnos-Core INTERFACE)

target_compile_features(Hypnos-Core INTERFACE cxx_std_20)

# Configure include directories for both build and install scenarios
# BUILD_INTERFACE: Points to the source tree location during build time
# INSTALL_INTERFACE: Points to the installed location after package installation
target_include_directories(Hypnos-Core INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_precompile_headers(Hypnos-Core INTERFACE
    "<Hypnos-Core/Logging.hpp>"
    "<Hypnos-Core/Platform.hpp>"
)

target_compile_options(Hypnos-Core INTERFACE
    -includeHypnos-Core/Logging.hpp
    -includeHypnos-Core/Platform.hpp)

if (CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    # Install the package
    install(TARGETS Hypnos-Core
        EXPORT HypnosCoreTargets
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    # Install the headers
    install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

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
endif()