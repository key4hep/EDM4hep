include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/edm4hep
)

file(GLOB sources src/*.cc)
file(GLOB headers edm4hep/*.h podio/PythonEventStore.h)

add_library(edm4hep SHARED ${sources} ${headers})
target_link_libraries(edm4hep podio)

REFLEX_GENERATE_DICTIONARY(edm4hep ${headers} SELECTION src/selection.xml )
add_library(edm4hepDict SHARED edm4hep.cxx)
add_dependencies(edm4hepDict edm4hep-dictgen)
target_link_libraries(edm4hepDict edm4hep podio ROOT::RIO ROOT::Tree ROOT::Physics)

set_target_properties(edm4hep PROPERTIES
  PUBLIC_HEADER "${headers}")

install(TARGETS edm4hep edm4hepDict
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
      "${PROJECT_BINARY_DIR}/edm4hep/edm4hep_rdict.pcm"
      DESTINATION "${INSTALL_LIB_DIR}" COMPONENT dev)
endif()
