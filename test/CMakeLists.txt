IF(NOT BUILD_TESTING)
  RETURN()
ENDIF()

function(set_test_env _testname)
  set_property(TEST ${_testname} APPEND PROPERTY ENVIRONMENT
      LD_LIBRARY_PATH=$<TARGET_FILE_DIR:edm4hep>:$<TARGET_FILE_DIR:podio::podio>:$<$<TARGET_EXISTS:edm4hepRDF>:$<TARGET_FILE_DIR:edm4hepRDF>:>$<TARGET_FILE_DIR:ROOT::Core>:$ENV{LD_LIBRARY_PATH}
      ROOT_INCLUDE_PATH=${PROJECT_SOURCE_DIR}/edm4hep:${PROJECT_SOURCE_DIR}/utils/include:$ENV{ROOT_INCLUDE_PATH}
  )
  set_tests_properties(${_testname} PROPERTIES
    FAIL_REGULAR_EXPRESSION "[^a-z]Error;ERROR;error;Failed"
    )
endfunction()

add_executable(write_events write_events.cc)
target_include_directories(write_events PUBLIC ${PROJECT_SOURCE_DIR}/edm4hep )
target_link_libraries(write_events edm4hep podio::podioRootIO)
add_test(NAME write_events COMMAND write_events)
set_test_env(write_events)

add_executable(read_events read_events.cc)
target_include_directories(read_events PUBLIC ${PROJECT_SOURCE_DIR}/edm4hep )
target_link_libraries(read_events edm4hep podio::podioRootIO)
add_test(NAME read_events COMMAND read_events)
  set_property(TEST read_events PROPERTY
    DEPENDS write_events
    )
set_test_env(read_events)

IF(TARGET edm4hepSioBlocks)
  add_executable(write_events_sio write_events_sio.cc)
  target_include_directories(write_events_sio PUBLIC ${PROJECT_SOURCE_DIR}/edm4hep)
  target_link_libraries(write_events_sio edm4hep podio::podioSioIO)
  add_test(NAME write_events_sio COMMAND write_events_sio)
  set_test_env(write_events_sio)

  add_executable(read_events_sio read_events_sio.cc)
  target_include_directories(read_events_sio PUBLIC ${PROJECT_SOURCE_DIR}/edm4hep)
  target_link_libraries(read_events_sio edm4hep podio::podioSioIO)
  add_test(NAME read_events_sio COMMAND read_events_sio)
  set_test_env(read_events_sio)
  set_tests_properties(read_events_sio PROPERTIES
    DEPENDS write_events_sio
    )
ENDIF()

IF(TARGET ROOT::ROOTDataFrame)
  add_executable(test_rdf test_rdf.cc)
  target_include_directories(test_rdf PUBLIC ${PROJECT_SOURCE_DIR}/edm4hep ${PROJECT_SOURCE_DIR}/dataframe )
  target_link_libraries(test_rdf edm4hepRDF ROOT::ROOTDataFrame podio::podioRootIO)
  add_test(NAME test_rdf COMMAND test_rdf)
  set_test_env(test_rdf)
  set_property(TEST test_rdf PROPERTY
    DEPENDS write_events
    )

  add_test(NAME py_test_rdf COMMAND python ${CMAKE_CURRENT_LIST_DIR}/test_rdf.py)
  set_test_env(py_test_rdf)
  set_property(TEST py_test_rdf APPEND PROPERTY ENVIRONMENT
      ${ENVIRONMENT}
      PYTHONPATH=${PROJECT_SOURCE_DIR}/python:$ENV{PYTHONPATH}
  )
  get_property(ENVIRONMENT TEST py_test_rdf PROPERTY ENVIRONMENT)
  # Remove fccanalyses to prevent ROOT from loading their dictionaries
  # for RDataFrame
  string(REGEX REPLACE "[^:]*\\/fccanalyses\\/[^:]*(:?|$)" "" ENVIRONMENT "${ENVIRONMENT}")
  set_property(TEST py_test_rdf PROPERTY ENVIRONMENT ${ENVIRONMENT})
  set_tests_properties(py_test_rdf PROPERTIES DEPENDS write_events)
endif()


find_package(HepMC3)
find_package(HepPDT)

if(HepMC3_FOUND AND HepPDT_FOUND )
  add_executable(edm4hep_testhepmc hepmc/edm4hep_testhepmc.cc)
  target_include_directories(edm4hep_testhepmc PUBLIC ${HEPMC3_INCLUDE_DIR} ${HEPPDT_INCLUDE_DIR} )
  target_link_libraries(edm4hep_testhepmc edm4hep podio::podioRootIO ${HEPPDT_LIBRARIES} ${HEPMC3_LIBRARIES})
  add_test(NAME edm4hep_testhepmc COMMAND edm4hep_testhepmc)
  set_test_env(edm4hep_testhepmc)
  set_property(TEST edm4hep_testhepmc APPEND PROPERTY ENVIRONMENT
      ROOT_INCLUDE_PATH=${HEPMC3_INCLUDE_DIR}:$ENV{ROOT_INCLUDE_PATH}
  )
endif()

add_subdirectory(utils)
add_subdirectory(tools)
