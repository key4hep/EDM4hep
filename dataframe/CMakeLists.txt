
file(GLOB sources *.cc)
file(GLOB headers *.h)


add_library(edm4hepRDF SHARED ${sources} ${headers})
target_link_libraries(edm4hepRDF edm4hep ROOT::Physics)
PODIO_GENERATE_DICTIONARY(edm4hepRDF ${headers} SELECTION selection.xml USES edm4hepRDF
  OPTIONS --library libedm4hepRDF${CMAKE_SHARED_LIBRARY_SUFFIX}
  )
set_target_properties(edm4hepRDF-dictgen PROPERTIES EXCLUDE_FROM_ALL TRUE)
target_sources(edm4hepRDF PRIVATE edm4hepRDF.cxx)

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
  "${PROJECT_BINARY_DIR}/dataframe/edm4hepRDFDict.rootmap"
  DESTINATION "${INSTALL_LIB_DIR}" COMPONENT dev)

if (${ROOT_VERSION} GREATER 6)
  install(FILES
      "${PROJECT_BINARY_DIR}/dataframe/libedm4hepRDF_rdict.pcm"
      DESTINATION "${INSTALL_LIB_DIR}" COMPONENT dev)
endif()
