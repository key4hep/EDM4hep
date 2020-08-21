cmake_minimum_required(VERSION 3.3)

project(EDM4HEP)

# project version
SET( ${PROJECT_NAME}_VERSION_MAJOR 0 )
SET( ${PROJECT_NAME}_VERSION_MINOR 2 )
SET( ${PROJECT_NAME}_VERSION_PATCH 0 )

SET( ${PROJECT_NAME}_VERSION  "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH}" )


#--- Run the podio class generator and link library -------

find_package(podio REQUIRED HINTS $ENV{PODIO})

#--- Declare ROOT dependency ---------------------------------------------------
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(ROOT REQUIRED COMPONENTS RIO Tree Physics)
set(ROOT_genreflex_cmd ${ROOT_genreflex_CMD})

#--- Define basic build settings -----------------------------------------------
# - Use GNU-style hierarchy for installing build products
include(GNUInstallDirs)

# - Define a default build type when using a single-mode tool like make/ninja
# If you're using a build tool that supports multiple modes (Visual Studio,
# Xcode), this setting has no effect.
# HSF recommend RelWithDebInfo (optimized with debugging symbols) as this is
# generally the mode used by system packaging (rpm, deb, spack, macports).
# However, it can be overriden by passing ``-DCMAKE_BUILD_TYPE=<type>`` when
# invoking CMake
if(NOT CMAKE_CONFIGURATION_TYPES)
  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo
      CACHE STRING "Choose the type of build, options are: None Release MinSizeRel Debug RelWithDebInfo"
      FORCE
      )
  else()
    set(CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}"
      CACHE STRING "Choose the type of build, options are: None Release MinSizeRel Debug RelWithDebInfo"
      FORCE
      )
  endif()
endif()

# - Define the C++ Standard to use (Simplest Possible)
set(CMAKE_CXX_STANDARD 17 CACHE STRING "")

if(NOT CMAKE_CXX_STANDARD MATCHES "14|17")
  message(FATAL_ERROR "Unsupported C++ standard: ${CMAKE_CXX_STANDARD}")
endif()

message (STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")

if (${APPLE})
    set(CPP_STANDARD_FLAGS "-std=c++${CMAKE_CXX_STANDARD}\ -stdlib=libc++")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -DDROP_CGAL ${CPP_STANDARD_FLAGS} -Wall -Wextra -Wpedantic -Wno-unused-variable -Wno-unused-parameter")


#--- Declare options -----------------------------------------------------------

option(BUILD_DDG4EDM4HEP "Build the EDM4hep plugin for DD4hep" ON)
option(BUILD_DELPHESEDM4HEP "Build the DELPHES plugin for DD4hep" ON)
option(EDM4HEP_DOCUMENTATION "Whether or not to create doxygen doc target.")

#--- enable unit testing capabilities ------------------------------------------
include(CTest)

#--- enable CPack --------------------------------------------------------------
option(ENABLE_CPACK "Whether or not to use cpack config" OFF)
if(ENABLE_CPACK)
  include(cmake/EDM4HEPCPack.cmake)
endif()

#--- target for Doxygen documentation ------------------------------------------
if(EDM4HEP_DOCUMENTATION)
  include(cmake/EDM4HEPDoxygen.cmake)
endif()

#--- add version files ---------------------------------------------------------

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/EDM4HEPVersion.h
               ${CMAKE_CURRENT_BINARY_DIR}/EDM4HEPVersion.h )
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/EDM4HEPVersion.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/edm4hep )

#--- add CMake infrastructure --------------------------------------------------
include(cmake/EDM4HEPCreateConfig.cmake)

#--- add license files ---------------------------------------------------------
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE
              ${CMAKE_CURRENT_SOURCE_DIR}/NOTICE
              DESTINATION ${CMAKE_INSTALL_DOCDIR})

add_subdirectory(edm4hep)
add_subdirectory(test)
add_subdirectory(dataframe)
if(BUILD_DDG4EDM4HEP)
  add_subdirectory(plugins/DDG4EDM4hep)
endif()
if(BUILD_DELPHESEDM4HEP)
  add_subdirectory(plugins/delphes)
endif()


#--- create uninstall target ---------------------------------------------------
include(cmake/EDM4HEPUninstall.cmake)
