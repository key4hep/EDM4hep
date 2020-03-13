

# we need to boostrap the data model, so this has to be executed in the cmake run
execute_process(
    COMMAND ${CMAKE_COMMAND} -E echo "Creating EDM4hep DataModel"
    COMMAND python ${podio_DIR}/../../../python/podio_class_generator.py ../edm4hep.yaml ./ edm4hep
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/edm4hep)


# for this to work the command has to be in the same CMakeLists.txt as the PODIO_GENERATE_DICTIONARY
# This ensures that we re-run the class generator if the yaml file is touched
# the command is run of datalayout.yaml is newer then selection.xml
add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/edm4hep/src/selection.xml  # we specify one output file so this doesn't always run
  COMMENT "Re-Creating EDM4hep DataModel"
  MAIN_DEPENDENCY ../edm4hep.yaml
  # This removal ensures selection.xml is newer than datalayout.yaml, other wise this command always runs
  COMMAND ${CMAKE_COMMAND} -E remove src/selection.xml
  COMMAND python ${podio_DIR}/../../../python/podio_class_generator.py ../edm4hep.yaml ./ edm4hep
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/edm4hep
  )

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
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
set_target_properties(edm4hep PROPERTIES PUBLIC_HEADER "${headers}")

add_library(edm4hepDict SHARED)
add_library(edm4hep::edm4hepDict ALIAS edm4hepDict )
target_link_libraries(edm4hepDict
  PUBLIC
  podio::podio
  )
target_include_directories(edm4hepDict
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
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
