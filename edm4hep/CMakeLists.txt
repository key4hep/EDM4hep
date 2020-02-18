
file(GLOB sources src/*.cc)
file(GLOB headers edm4hep/*.h podio/PythonEventStore.h)

add_library(edm4hep SHARED ${sources} ${headers})
target_link_libraries(edm4hep
  PUBLIC
  podio::podio
  )
target_include_directories(edm4hep
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/edm4hep>
  $<INSTALL_INTERFACE:edm4hep>
)

PODIO_GENERATE_DICTIONARY(edm4hep ${headers} SELECTION src/selection.xml USES edm4hep
  OPTIONS --library libedm4hep${CMAKE_SHARED_LIBRARY_SUFFIX})
set_target_properties(edm4hep-dictgen PROPERTIES EXCLUDE_FROM_ALL TRUE)

set_target_properties(edm4hep PROPERTIES PUBLIC_HEADER "${headers}")
target_sources(edm4hep PRIVATE edm4hep.cxx)

install(TARGETS edm4hep
  # IMPORTANT: Add the datamodel library to the "export-set"
  EXPORT EDM4HEPTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/edm4hep"
  COMPONENT dev)

install(FILES
  "${PROJECT_BINARY_DIR}/edm4hep/edm4hepDict.rootmap"
  DESTINATION "${INSTALL_LIB_DIR}" COMPONENT dev)

if (${ROOT_VERSION} GREATER 6)
  install(FILES
      "${PROJECT_BINARY_DIR}/edm4hep/libedm4hep_rdict.pcm"
      DESTINATION "${INSTALL_LIB_DIR}" COMPONENT dev)
endif()
