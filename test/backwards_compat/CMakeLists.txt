macro(add_backwards_compat_test input_base)
  ExternalData_Add_Test(backward_compat_tests
    NAME backwards_compat_${input_base} COMMAND pytest -v --inputfile=DATA{${CMAKE_CURRENT_SOURCE_DIR}/input_files/edm4hep_example_${input_base}.root}
  )
  set_test_env(backwards_compat_${input_base})
endmacro()

add_backwards_compat_test(v00-99-01_podio_v01-01)
add_backwards_compat_test(v00-99-02_podio_v01-03)

if (${ROOT_VERSION} VERSION_GREATER_EQUAL 6.36 AND ${podio_VERSION} VERSION_GREATER_EQUAL 1.3)
  # This input file needs to be read with ROOT 6.36 as otherwise RNTuple does
  # not yet have the necessary support for reading old schemas
  # Trying to read this with an older version of ROOT doesn't work
  add_backwards_compat_test(rntuple_v00-99-01_podio_v01-03)
endif()

# Run all the tests in the correct directory
get_property(test_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY TESTS)
set_tests_properties(
  ${test_names}

  PROPERTIES
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test
)
