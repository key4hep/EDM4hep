add_test(NAME check_complete_file COMMAND pytest --inputfile=${CMAKE_BINARY_DIR}/test/edm4hep_example.root -v)
set_test_env(check_complete_file)
set_tests_properties(
  check_complete_file

  PROPERTIES
   DEPENDS create_complete_file
   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_test(NAME check_complete_file_rntuple COMMAND pytest --inputfile=${CMAKE_BINARY_DIR}/test/edm4hep_example_rntuple.root -v)
set_test_env(check_complete_file_rntuple)
set_tests_properties(
  check_complete_file_rntuple

  PROPERTIES
   DEPENDS create_complete_file_rntuple
   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
