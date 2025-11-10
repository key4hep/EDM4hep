ExternalData_Add_Test(backward_compat_tests
  NAME backwards_compat_v00-99 COMMAND pytest -v --inputfile=DATA{${CMAKE_CURRENT_SOURCE_DIR}/input_files/edm4hep_example_v00-99-01_podio_v01-01.root})
set_test_env(backwards_compat_v00-99)

set_tests_properties(
  backwards_compat_v00-99

  PROPERTIES
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test
)

if (${ROOT_VERSION} VERSION_GREATER_EQUAL 6.36 AND ${podio_VERSION} VERSION_GREATER_EQUAL 1.3)
  # This input file needs to be read with ROOT 6.36 as otherwise RNTuple does
  # not yet have the necessary support for reading old schemas
  # Trying to read this with an older version of ROOT doesn't work
  ExternalData_Add_Test(backward_compat_tests
    NAME backwards_compat_rntuple_v00-99 COMMAND pytest -v --inputfile=DATA{${CMAKE_CURRENT_SOURCE_DIR}/input_files/edm4hep_example_rntuple_v00-99-01_podio_v01-03.root})
  set_test_env(backwards_compat_rntuple_v00-99)

  set_tests_properties(
    backwards_compat_rntuple_v00-99

    PROPERTIES
      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test
  )
endif()
