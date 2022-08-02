cmake_minimum_required(VERSION 3.11)


project(DownstreamProjectUsingEDM4hep)

find_package(EDM4HEP)

# Use the EDM4HEP_VERSION in an if clause to trigger a cmake error in case it is
# unset.
if (${EDM4HEP_VERSION} VERSION_GREATER_EQUAL "0.4.1")
  message(STATUS "The current exported EDM4HEP version is " ${EDM4HEP_VERSION})
else()
  message(STATUS "The current exported EDM4HEP version is " ${EDM4HEP_VERSION})
endif()

# Use the EDM4HEP_DATA_DIR to derive extended data model
if (${EDM4HEP_DATA_DIR})
  message(STATUS "Current EDM4HEP data model is available at ${EDM4HEP_DATA_DIR}.")
  PODIO_GENERATE_DATAMODEL(EDM4DIS edm4dis.yaml headers sources
    UPSTREAM_EDM edm4hep:${EDM4HEP_DATA_DIR}/edm4hep.yaml
    OUTPUT_FOLDER ${CMAKE_CURRENT_BINARY_DIR}
  )
  add_library(EDM4DIS SHARED
    ${sources}
  )

  target_link_libraries(EDM4DIS
    PUBLIC EDM4HEP::edm4hep
    PUBLIC podio::podio
  )
  target_include_directories(EDM4DIS
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
  )

  PODIO_GENERATE_DICTIONARY(EDM4DIS ${headers} 
    SELECTION ${CMAKE_CURRENT_BINARY_DIR}/src/selection.xml
    OPTIONS --library ${CMAKE_SHARED_LIBRARY_PREFIX}EDM4DIS${CMAKE_SHARED_LIBRARY_SUFFIX}
  )
  set_target_properties(edm4dis-dictgen PROPERTIES EXCLUDE_FROM_ALL TRUE)
else()
  message(WARNING "Unable to extend data model since no installed yaml file found.")
endif()

add_executable(appUsingEDM4hep main.cxx)
target_link_libraries(appUsingEDM4hep EDM4HEP::edm4hep EDM4HEP::utils)
