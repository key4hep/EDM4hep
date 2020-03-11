
file(GLOB sources *.cc)
file(GLOB headers *.h)


add_library(edm4hepRDF SHARED ${sources} ${headers})
target_link_libraries(edm4hepRDF edm4hep ROOT::Physics)
PODIO_GENERATE_DICTIONARY(edm4hepRDF ${headers} SELECTION selection.xml
  OPTIONS --library libedm4hepRDF${CMAKE_SHARED_LIBRARY_SUFFIX}
  )
set_target_properties(edm4hepRDF-dictgen PROPERTIES EXCLUDE_FROM_ALL TRUE)

set_target_properties(edm4hepRDF PROPERTIES
  PUBLIC_HEADER "${headers}")

install(TARGETS edm4hepRDF
  # IMPORTANT: Add the library to the "export-set"
  EXPORT edm4hepTargets
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT bin
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT shlib
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/edm4hep"
    COMPONENT dev)

install(FILES
  "${PROJECT_BINARY_DIR}/dataframe/edm4hepRDFDict.rootmap"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT dev)

if (${ROOT_VERSION} GREATER 6)
  install(FILES
      "${PROJECT_BINARY_DIR}/dataframe/libedm4hepRDF_rdict.pcm"
      DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT dev)
endif()
