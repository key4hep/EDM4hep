
# For now unconditionally generate all the code that is supported by the
# installed podio
PODIO_GENERATE_DATAMODEL(edm4hep ../edm4hep.yaml headers sources IO_BACKEND_HANDLERS ${PODIO_IO_HANDLERS})

PODIO_ADD_DATAMODEL_CORE_LIB(edm4hep "${headers}" "${sources}")
target_include_directories(edm4hep PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/utils/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
add_library(EDM4HEP::edm4hep ALIAS edm4hep)

file(GLOB_RECURSE top_headers ${PROJECT_SOURCE_DIR}/include/*.h)
target_sources(edm4hep PUBLIC FILE_SET headers TYPE HEADERS BASE_DIRS ${PROJECT_SOURCE_DIR}/include FILES ${top_headers})

if (nlohmann_json_FOUND)
  target_compile_definitions(edm4hep PUBLIC PODIO_JSON_OUTPUT)
  target_link_libraries(edm4hep PUBLIC nlohmann_json::nlohmann_json)
endif()

PODIO_ADD_ROOT_IO_DICT(edm4hepDict edm4hep "${headers}" src/selection.xml)
add_library(EDM4HEP::edm4hepDict ALIAS edm4hepDict )

list(APPEND EDM4HEP_INSTALL_LIBS edm4hep edm4hepDict)

PODIO_ADD_SIO_IO_BLOCKS(edm4hep "${headers}" "${sources}")
IF(TARGET edm4hepSioBlocks)
  message(STATUS "Building and installing the SioBlocks since podio supports it")
  list(APPEND EDM4HEP_INSTALL_LIBS edm4hepSioBlocks)
ENDIF()

install(TARGETS ${EDM4HEP_INSTALL_LIBS}
  EXPORT EDM4HEPTargets
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT bin
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT shlib
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/edm4hep"
  FILE_SET headers DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
  COMPONENT dev)

install(FILES
  "${PROJECT_BINARY_DIR}/edm4hep/edm4hepDictDict.rootmap"
  "${PROJECT_BINARY_DIR}/edm4hep/libedm4hepDict_rdict.pcm"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT dev)

install(FILES
  ../edm4hep.yaml
  DESTINATION "${CMAKE_INSTALL_DATADIR}/edm4hep" COMPONENT dev)
