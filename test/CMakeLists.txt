IF(NOT BUILD_TESTING)
  RETURN()
ENDIF()

function(set_test_env _testname)
  set_property(TEST ${_testname} APPEND PROPERTY ENVIRONMENT
      ROOT_LIBRARY_PATH=$<TARGET_FILE_DIR:edm4hep>:$<TARGET_FILE_DIR:podio::podio>:$<TARGET_FILE_DIR:edm4hepRDF>:$<TARGET_FILE_DIR:ROOT::Core>
      LD_LIBRARY_PATH=$<TARGET_FILE_DIR:edm4hep>:$<TARGET_FILE_DIR:edm4hepRDF>:$ENV{LD_LIBRARY_PATH}
      ROOT_INCLUDE_PATH=${PROJECT_SOURCE_DIR}/edm4hep:${PROJECT_SOURCE_DIR}/utils/include:$ENV{ROOT_INCLUDE_PATH}
      PYTHONPATH=${PROJECT_SOURCE_DIR}/python:$ENV{PYTHONPATH}
      PODIO_SIOBLOCK_PATH=$<TARGET_FILE_DIR:edm4hep>
  )
  set_tests_properties(${_testname} PROPERTIES
    FAIL_REGULAR_EXPRESSION "[^a-z]Error;ERROR;error;Failed"
  )
endfunction()

macro(add_edm4hep_test _testname)
  add_test(NAME ${_testname} ${ARGN})
  set_test_env(${_testname})
endmacro()

add_edm4hep_test(create_complete_file COMMAND python ${PROJECT_SOURCE_DIR}/scripts/createEDM4hepFile.py --output-file edm4hep_example.root)
set_tests_properties(create_complete_file PROPERTIES
  FIXTURES_SETUP complete_file
  TIMEOUT 600
)

add_edm4hep_test(create_complete_file_rntuple COMMAND python ${PROJECT_SOURCE_DIR}/scripts/createEDM4hepFile.py --rntuple --output-file edm4hep_example_rntuple.root)
set_tests_properties(create_complete_file_rntuple PROPERTIES
  SKIP_REGULAR_EXPRESSION "The RNTuple writer from podio is not available but was requested"
  FIXTURES_SETUP complete_file_rntuple
  TIMEOUT 600
)

add_edm4hep_test(check_complete_file COMMAND pytest --inputfile=${PROJECT_BINARY_DIR}/test/edm4hep_example.root -v)
set_tests_properties(
  check_complete_file PROPERTIES
  FIXTURES_REQUIRED complete_file
  TIMEOUT 600
)
add_edm4hep_test(check_complete_file_rntuple COMMAND pytest --inputfile=${PROJECT_BINARY_DIR}/test/edm4hep_example_rntuple.root -v)
set_tests_properties(
  check_complete_file_rntuple PROPERTIES
  FIXTURES_REQUIRED complete_file_rntuple
  SKIP_REGULAR_EXPRESSION "collected 0 items / 1 skipped"
  TIMEOUT 600
)

set_tests_properties(
  check_complete_file
  check_complete_file_rntuple

  PROPERTIES
   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(write_events write_events.cc)
target_include_directories(write_events PUBLIC ${PROJECT_SOURCE_DIR}/edm4hep )
target_link_libraries(write_events edm4hep podio::podioRootIO)
add_edm4hep_test(write_events COMMAND write_events)
set_tests_properties(
  write_events PROPERTIES
  FIXTURES_SETUP write_events_fixture
)

add_executable(read_events read_events.cc)
target_include_directories(read_events PUBLIC ${PROJECT_SOURCE_DIR}/edm4hep )
target_link_libraries(read_events edm4hep podio::podioRootIO)
add_edm4hep_test(read_events COMMAND read_events)
set_tests_properties(
  read_events PROPERTIES
  FIXTURES_REQUIRED write_events_fixture
)

IF(TARGET edm4hepSioBlocks)
  add_executable(write_events_sio write_events_sio.cc)
  target_include_directories(write_events_sio PUBLIC ${PROJECT_SOURCE_DIR}/edm4hep)
  target_link_libraries(write_events_sio edm4hep podio::podioSioIO)
  add_edm4hep_test(write_events_sio COMMAND write_events_sio)
  set_tests_properties(
    write_events_sio PROPERTIES
    FIXTURES_SETUP write_events_sio_fixture
  )

  add_executable(read_events_sio read_events_sio.cc)
  target_include_directories(read_events_sio PUBLIC ${PROJECT_SOURCE_DIR}/edm4hep)
  target_link_libraries(read_events_sio edm4hep podio::podioSioIO)
  add_edm4hep_test(read_events_sio COMMAND read_events_sio)
  set_tests_properties(read_events_sio PROPERTIES
    FIXTURES_REQUIRED write_events_sio_fixture
  )
ENDIF()

IF(TARGET ROOT::ROOTDataFrame)
  add_executable(test_rdf test_rdf.cc)
  target_include_directories(test_rdf PUBLIC ${PROJECT_SOURCE_DIR}/edm4hep ${PROJECT_SOURCE_DIR}/dataframe )
  target_link_libraries(test_rdf edm4hepRDF ROOT::ROOTDataFrame podio::podioRootIO)
  add_edm4hep_test(test_rdf COMMAND test_rdf)
  set_property(TEST test_rdf PROPERTY
    FIXTURES_REQUIRED write_events_fixture
  )

  add_edm4hep_test(py_test_rdf COMMAND python ${CMAKE_CURRENT_LIST_DIR}/test_rdf.py)
  set_tests_properties(py_test_rdf PROPERTIES FIXTURES_REQUIRED write_events_fixture)
endif()


find_package(HepMC3)
find_package(HepPDT)

if(HepMC3_FOUND AND HepPDT_FOUND )
  add_executable(edm4hep_testhepmc hepmc/edm4hep_testhepmc.cc)
  target_include_directories(edm4hep_testhepmc PUBLIC ${HEPMC3_INCLUDE_DIR} ${HEPPDT_INCLUDE_DIR} )
  target_link_libraries(edm4hep_testhepmc edm4hep podio::podioRootIO ${HEPPDT_LIBRARIES} ${HEPMC3_LIBRARIES})
  add_edm4hep_test(edm4hep_testhepmc COMMAND edm4hep_testhepmc)
  set_property(TEST edm4hep_testhepmc APPEND PROPERTY ENVIRONMENT
      ROOT_INCLUDE_PATH=${HEPMC3_INCLUDE_DIR}:$ENV{ROOT_INCLUDE_PATH}
  )
endif()

add_edm4hep_test(py_test_module COMMAND python ${CMAKE_CURRENT_LIST_DIR}/test_module.py)

add_subdirectory(utils)
add_subdirectory(tools)


include(ExternalData)
list(APPEND ExternalData_URL_TEMPLATES
  "https://key4hep.web.cern.ch:443/testFiles/EDM4hep/%(hash)"
)

add_subdirectory(backwards_compat)

ExternalData_Add_Target(backward_compat_tests)
