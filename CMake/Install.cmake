install(TARGETS Hypnos-Core
    EXPORT HypnosCoreTargets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY "${PROJECT_SOURCE_DIR}/Include/" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT HypnosCoreTargets
    FILE HypnosCoreTargets.cmake
    NAMESPACE Blanketmen::
    DESTINATION lib/cmake/Hypnos-Core
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/CMake/HypnosCoreConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/Hypnos-CoreConfig.cmake"
    INSTALL_DESTINATION lib/cmake/Hypnos-Core
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/Hypnos-CoreConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMinorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/Hypnos-CoreConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/Hypnos-CoreConfigVersion.cmake"
    DESTINATION lib/cmake/Hypnos-Core
)
