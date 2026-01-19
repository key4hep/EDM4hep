# files used in ExtraCode directives
set(extra_code extra_code/CovMatrixCommon.ipp)

# For now unconditionally generate all the code that is supported by the
# installed podio
PODIO_GENERATE_DATAMODEL(edm4hep ../edm4hep.yaml headers sources
  IO_BACKEND_HANDLERS ${PODIO_IO_HANDLERS}
  DEPENDS ${extra_code}
  VERSION ${${PROJECT_NAME}_VERSION}
  OLD_DESCRIPTIONS ../old_schemas/edm4hep_v5.yaml ../old_schemas/edm4hep_v4.yaml ../old_schemas/edm4hep_v3.yaml ../old_schemas/edm4hep_v2.yaml
  SCHEMA_EVOLUTION ../edm4hep_evolution.yaml
)

PODIO_ADD_DATAMODEL_CORE_LIB(edm4hep "${headers}" "${sources}")
target_include_directories(edm4hep PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/utils/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
add_library(EDM4HEP::edm4hep ALIAS edm4hep)

file(GLOB_RECURSE top_headers ${PROJECT_SOURCE_DIR}/include/*.h)
target_sources(edm4hep PUBLIC FILE_SET headers TYPE HEADERS BASE_DIRS ${PROJECT_SOURCE_DIR}/include FILES ${top_headers})

if (EDM4HEP_WITH_JSON)
  target_compile_definitions(edm4hep PUBLIC PODIO_JSON_OUTPUT)
  target_link_libraries(edm4hep PUBLIC nlohmann_json::nlohmann_json)
endif()

# Add the old schema bits to the core library
set(schema_evolution_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/schema_evolution/src/OldLinkEvolution.cc
)
target_sources(edm4hep PRIVATE ${schema_evolution_sources})
target_include_directories(edm4hep PRIVATE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/edm4hep/schema_evolution/include>
)

list(APPEND headers
  ${PROJECT_SOURCE_DIR}/edm4hep/schema_evolution/include/edm4hep/schema_evolution/OldLinkData.h
)

# Merge the schema evolution selection xml and the podio generated one
set(MERGED_SELECTION_XML ${CMAKE_CURRENT_BINARY_DIR}/merged_selection.xml)
execute_process(
  COMMAND ${Python3_EXECUTABLE} ${PROJECT_SOURCE_DIR}/edm4hep/schema_evolution/merge_selection_xml.py
    ${CMAKE_CURRENT_SOURCE_DIR}/src/selection.xml
    ${CMAKE_CURRENT_SOURCE_DIR}/schema_evolution/src/selection.xml
    ${MERGED_SELECTION_XML}
  RESULT_VARIABLE MERGE_RESULT
)

if(NOT MERGE_RESULT EQUAL 0)
  message(FATAL_ERROR "Failed to merge selection.xml files")
endif()

PODIO_ADD_ROOT_IO_DICT(edm4hepDict edm4hep "${headers}" ${MERGED_SELECTION_XML})
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

install(DIRECTORY
  extra_code
  DESTINATION "${CMAKE_INSTALL_DATADIR}/edm4hep/edm4hep" COMPONENT dev)
