ExternalData_Add_Test(backward_compat_tests
  NAME backwards_compat_v00-99 COMMAND pytest -v --inputfile=DATA{${CMAKE_CURRENT_SOURCE_DIR}/input_files/edm4hep_example_v00-99-01_podio_v01-01.root})
set_test_env(backwards_compat_v00-99)

ExternalData_Add_Test(backward_compat_tests
  NAME backwards_compat_rntuple_v00-99 COMMAND pytest -v --inputfile=DATA{${CMAKE_CURRENT_SOURCE_DIR}/input_files/edm4hep_example_rntuple_v00-99-01_podio_v01-01.root})
set_test_env(backwards_compat_rntuple_v00-99)

set_tests_properties(
  backwards_compat_v00-99
  backwards_compat_rntuple_v00-99

  PROPERTIES
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test
)
