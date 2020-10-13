
# For now unconditionally generate all the code that is supported by the
# installed podio
PODIO_GENERATE_DATAMODEL(edm4hep ../edm4hep.yaml headers sources IO_BACKEND_HANDLERS ${PODIO_IO_HANDLERS})

PODIO_ADD_DATAMODEL_CORE_LIB(edm4hep "${headers}" "${sources}")

PODIO_ADD_ROOT_IO_DICT(edm4hepDict edm4hep "${headers}" src/selection.xml)
add_library(edm4hep::edm4hepDict ALIAS edm4hepDict )

PODIO_ADD_SIO_IO_BLOCKS(edm4hep "${headers}" "${sources}")
IF(TARGET edm4hepSioBlocks)
  message(STATUS "SioBlocks are here!")
ELSE()
  message(STATUS "SioBlocks are not here")
ENDIF()

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
      "${PROJECT_BINARY_DIR}/edm4hep/libedm4hepDict_rdict.pcm"
      DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT dev)
endif()
