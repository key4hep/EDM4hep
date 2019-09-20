cmake_minimum_required(VERSION 3.3)

project(EDM4HEP VERSION 0.1.0)

#--- Run the podio class generator and link library -------

find_package(podio REQUIRED HINTS $ENV{PODIO})

execute_process(COMMAND python ${podio_CMAKE_DIR}/../python/podio_class_generator.py edm4hep.yaml edm4hep edm4hep
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

find_library(PODIOLIB podio PATHS ${podio_DIR}/../lib)
if (NOT PODIOLIB)
    message(FATAL_ERROR "libpodio.so(dylib) cannot be found dynamically. Make sure you have set up your environment to use PODIO")
endif()
link_directories(${podio_LIBRARY_DIR})

#--- Declare ROOT dependency ---------------------------------------------------
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(ROOT REQUIRED COMPONENTS RIO Tree Physics)
include_directories(${ROOT_INCLUDE_DIR})
include(${ROOT_USE_FILE})
link_directories(${ROOT_LIBRARY_DIR})
set(ROOT_genreflex_cmd ${ROOT_genreflex_CMD})

# set up include-directories
include_directories(
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_BINARY_DIR}
  ${ROOT_INCLUDE_DIR}
  ${podio_INCLUDE_DIRS}
  )

#--- Define basic build settings -----------------------------------------------
# - Use GNU-style hierarchy for installing build products
include(GNUInstallDirs)

# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")

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
option(EDM4HEP_documentation "Whether or not to create doxygen doc target.")

#--- enable unit testing capabilities ------------------------------------------
include(CTest)

#--- enable CPack --------------------------------------------------------------
include(cmake/EDM4HEPCPack.cmake)

#--- target for Doxygen documentation ------------------------------------------
if(EDM4HEP_documentation)
  include(cmake/EDM4HEPDoxygen.cmake)
endif()

#--- add version files ---------------------------------------------------------

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/EDM4HEPVersion.h
               ${CMAKE_CURRENT_BINARY_DIR}/EDM4HEPVersion.h )
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/EDM4HEPVersion.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/EDM4HEP )

#--- add CMake infrastructure --------------------------------------------------
include(cmake/EDM4HEPCreateConfig.cmake)

#--- add license files ---------------------------------------------------------
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE
              ${CMAKE_CURRENT_SOURCE_DIR}/NOTICE
              DESTINATION ${CMAKE_INSTALL_DOCDIR})

add_subdirectory(edm4hep)


#--- create uninstall target ---------------------------------------------------
include(cmake/EDM4HEPUninstall.cmake)
