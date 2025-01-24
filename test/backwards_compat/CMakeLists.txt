ExternalData_Add_Test(backward_compat_tests
  NAME backwards_compat_v00-99 COMMAND pytest -v --inputfile=DATA{${CMAKE_CURRENT_SOURCE_DIR}/input_files/edm4hep_example_v00-99-01_podio_v01-01.root})
set_test_env(backwards_compat_v00-99)

set_tests_properties(
  backwards_compat_v00-99

  PROPERTIES
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test
)


if (ROOT_VERSION VERSION_LESS 6.33)
  # This input file has been written with RNTuple RC2 which is only present up
  # to the 6.32 release series. Trying to read this with a newer version of ROOT
  # doesn't work
  ExternalData_Add_Test(backward_compat_tests
    NAME backwards_compat_rntuple_v00-99 COMMAND pytest -v --inputfile=DATA{${CMAKE_CURRENT_SOURCE_DIR}/input_files/edm4hep_example_rntuple_v00-99-01_podio_v01-02.root})
  set_test_env(backwards_compat_rntuple_v00-99)

  set_tests_properties(
    backwards_compat_rntuple_v00-99

    PROPERTIES
      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test
  )
endif()
