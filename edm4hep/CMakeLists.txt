
PODIO_GENERATE_DATAMODEL(edm4hep ../edm4hep.yaml headers sources)

add_library(edm4hep SHARED ${sources} ${headers})
target_link_libraries(edm4hep
  PUBLIC
  podio::podio
  )
target_include_directories(edm4hep
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
set_target_properties(edm4hep PROPERTIES PUBLIC_HEADER "${headers}")

add_library(edm4hepDict SHARED)
add_library(edm4hep::edm4hepDict ALIAS edm4hepDict )
target_link_libraries(edm4hepDict
  PUBLIC
  edm4hep
  podio::podio
  ROOT::Core
  )
target_include_directories(edm4hepDict
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  ${ROOT_INCLUDE_DIRS}
)

PODIO_GENERATE_DICTIONARY(edm4hepDict ${headers} SELECTION src/selection.xml
  OPTIONS --library libedm4hep${CMAKE_SHARED_LIBRARY_SUFFIX})
set_target_properties(edm4hepDict-dictgen PROPERTIES EXCLUDE_FROM_ALL TRUE)
add_dependencies(edm4hepDict edm4hep)

install(TARGETS edm4hep edm4hepDict
  EXPORT EDM4HEPTargets
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT bin
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT shlib
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/edm4hep"
  COMPONENT dev)

install(FILES
  "${PROJECT_BINARY_DIR}/edm4hep/edm4hepDictDict.rootmap"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT dev)

if (${ROOT_VERSION} GREATER 6)
  install(FILES
      "${PROJECT_BINARY_DIR}/edm4hep/libedm4hep_rdict.pcm"
      DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT dev)
endif()
