# - Use CMake's module to help generating relocatable config files
include(CMakePackageConfigHelpers)

# - Versioning
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/EDM4HEPConfigVersion.cmake
  VERSION ${EDM4HEP_VERSION}
  COMPATIBILITY SameMajorVersion)

# - Install time config and target files
configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/EDM4HEPConfig.cmake.in
  "${PROJECT_BINARY_DIR}/EDM4HEPConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/../share/cmake/EDM4HEP"
  PATH_VARS
    CMAKE_INSTALL_BINDIR
    CMAKE_INSTALL_INCLUDEDIR
    CMAKE_INSTALL_LIBDIR
  )

# - install and export
install(FILES
  "${PROJECT_BINARY_DIR}/EDM4HEPConfigVersion.cmake"
  "${PROJECT_BINARY_DIR}/EDM4HEPConfig.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/../share/cmake/EDM4HEP"
  )
install(EXPORT EDM4HEPTargets
  NAMESPACE EDM4HEP::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/../share/cmake/EDM4HEP"
  )

