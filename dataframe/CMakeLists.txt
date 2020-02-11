
file(GLOB sources *.cc)
file(GLOB headers *.h)

PODIO_GENERATE_DICTIONARY(libedm4hepRDF edm4hepRDF.h SELECTION selection.xml USES edm4hep)

add_library(edm4hepRDF SHARED ${sources} ${headers} libedm4hepRDF.cxx )
target_link_libraries(edm4hepRDF edm4hep podio::podio ROOT::Physics)

set_target_properties(edm4hepRDF PROPERTIES
  PUBLIC_HEADER "${headers}")

install(TARGETS edm4hepRDF
  # IMPORTANT: Add the library to the "export-set"
  EXPORT edm4hepTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/edm4hep"
    COMPONENT dev)

install(FILES
  "${PROJECT_BINARY_DIR}/dataframe/libedm4hepRDFDict.rootmap"
  DESTINATION "${INSTALL_LIB_DIR}" COMPONENT dev)

if (${ROOT_VERSION} GREATER 6)
  install(FILES
      "${PROJECT_BINARY_DIR}/dataframe/libedm4hepRDF_rdict.pcm"
      DESTINATION "${INSTALL_LIB_DIR}" COMPONENT dev)
endif()
