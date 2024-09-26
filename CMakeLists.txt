cmake_minimum_required(VERSION 3.23)

project(EDM4HEP LANGUAGES CXX)

# project version
SET( ${PROJECT_NAME}_VERSION_MAJOR 0 )
SET( ${PROJECT_NAME}_VERSION_MINOR 99 )
SET( ${PROJECT_NAME}_VERSION_PATCH 1 )

SET( ${PROJECT_NAME}_VERSION  "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH}" )

find_package(podio 1.1 REQUIRED HINTS $ENV{PODIO})
find_package(ROOT REQUIRED COMPONENTS RIO Tree Physics)

include(cmake/Key4hepConfig.cmake)

#--- Define basic build settings -----------------------------------------------
# - Use GNU-style hierarchy for installing build products
include(GNUInstallDirs)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(cmake/EDM4HEPBuild.cmake)
edm4hep_set_linker_flags()

#--- Declare options -----------------------------------------------------------

option(BUILD_DOCS "Whether or not to create doxygen doc target.")

option(USE_EXTERNAL_CATCH2 "Link against an external Catch2 v3 static
library, otherwise build it locally" ON)

#--- enable unit testing capabilities ------------------------------------------
include(CTest)

#--- enable CPack --------------------------------------------------------------
option(ENABLE_CPACK "Whether or not to use cpack config" OFF)
if(ENABLE_CPACK)
  include(cmake/EDM4HEPCPack.cmake)
endif()

#--- target for Doxygen documentation ------------------------------------------
if(BUILD_DOCS)
  include(cmake/EDM4HEPDoxygen.cmake)
endif()

#--- add version files ---------------------------------------------------------

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/EDM4hepVersion.h.in
               ${CMAKE_CURRENT_SOURCE_DIR}/edm4hep/edm4hep/EDM4hepVersion.h )
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/edm4hep/edm4hep/EDM4hepVersion.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/edm4hep )
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/python/edm4hep/__version__.py.in
  ${CMAKE_CURRENT_SOURCE_DIR}/python/edm4hep/__version__.py)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/edm4hep
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

#--- add CMake infrastructure --------------------------------------------------
include(cmake/EDM4HEPCreateConfig.cmake)

#--- add license files ---------------------------------------------------------
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE
              ${CMAKE_CURRENT_SOURCE_DIR}/NOTICE
              DESTINATION ${CMAKE_INSTALL_DOCDIR})

find_package(nlohmann_json 3.10.5)

add_subdirectory(edm4hep)
add_subdirectory(utils)
add_subdirectory(tools)
add_subdirectory(python)
add_subdirectory(test)

#--- create uninstall target ---------------------------------------------------
include(cmake/EDM4HEPUninstall.cmake)
