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
message(STATUS "${EDM4HEP_DATA_DIR}")
if ("${EDM4HEP_DATA_DIR}" STREQUAL "")
  message(FATAL_ERROR "Unable to extend data model since no installed yaml file found.")
else()
  message(STATUS "Current EDM4HEP data model is available at ${EDM4HEP_DATA_DIR}.")
  PODIO_GENERATE_DATAMODEL(edm4dis edm4dis.yaml headers sources
    UPSTREAM_EDM edm4hep:${EDM4HEP_DATA_DIR}/edm4hep.yaml
    OUTPUT_FOLDER ${CMAKE_CURRENT_BINARY_DIR}
    IO_BACKEND_HANDLERS ${PODIO_IO_HANDLERS}
  )

  PODIO_ADD_DATAMODEL_CORE_LIB(edm4dis "${headers}" "${sources}"
    OUTPUT_FOLDER ${CMAKE_CURRENT_BINARY_DIR}
  )

  PODIO_ADD_ROOT_IO_DICT(edm4disDict edm4dis "${headers}" ${CMAKE_CURRENT_BINARY_DIR}/src/selection.xml
    OUTPUT_FOLDER ${CMAKE_CURRENT_BINARY_DIR}
  )
endif()

add_executable(appUsingEDM4hep main.cxx)
target_link_libraries(appUsingEDM4hep EDM4HEP::edm4hep EDM4HEP::utils)
