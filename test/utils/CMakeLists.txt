if(USE_EXTERNAL_CATCH2)
  find_package(Catch2 REQUIRED)
  # Mark Catch2 include directories as SYSTEM to suppress warnings from its headers
  get_target_property(CATCH2_IF_INC_DIRS Catch2::Catch2 INTERFACE_INCLUDE_DIRECTORIES)
  set_target_properties(Catch2::Catch2 PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${CATCH2_IF_INC_DIRS}")
else()
  message(STATUS "Fetching local copy of Catch2 library for unit-tests...")

  include(FetchContent)
  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.4.0
  )
  FetchContent_MakeAvailable(Catch2)
  set(CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras ${CMAKE_MODULE_PATH})
  # Same hack as for the external case: mark include dirs as SYSTEM
  get_target_property(CATCH2_IF_INC_DIRS Catch2 INTERFACE_INCLUDE_DIRECTORIES)
  set_target_properties(Catch2 PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${CATCH2_IF_INC_DIRS}")
endif()

include(Catch)

add_executable(unittests_edm4hep
  test_kinematics.cpp
  test_vector_utils.cpp
  test_covmatrix_utils.cpp
  test_PIDHandler.cpp
  test_bit_utils.cpp
  test_ExtraCode.cpp
)

target_link_libraries(unittests_edm4hep edm4hep EDM4HEP::utils Catch2::Catch2 Catch2::Catch2WithMain)
# Catch2 TEST_CASE/SECTION macros expand __COUNTER__ at the call site in user
# code, so marking Catch2 headers as system includes is not sufficient to
# suppress this warning, present with Clang 22
target_compile_options(unittests_edm4hep PRIVATE -Wno-c2y-extensions)

option(SKIP_CATCH_DISCOVERY "Skip the Catch2 test discovery" OFF)

# To work around https://github.com/catchorg/Catch2/issues/2424 we need the
# DL_PATH argument for catch_discoer_tests which requires CMake 3.22 at least
# The whole issue can be avoied if we skip the catch test discovery and set the
# environment on our own
if (CMAKE_VERSION VERSION_LESS 3.22)
  set(SKIP_CATCH_DISCOVERY ON)
endif()

if (SKIP_CATCH_DISCOVERY)
  # Unfortunately Memory sanitizer seems to be really unhappy with Catch2 and
  # it fails to successfully launch the executable and execute any test. Here
  # we just include them in order to have them show up as failing
  add_edm4hep_test(unittests COMMAND unittests_edm4hep)
else()
  catch_discover_tests(unittests_edm4hep
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DL_PATHS $<TARGET_FILE_DIR:edm4hep>:$<TARGET_FILE_DIR:podio::podio>:$<TARGET_FILE_DIR:ROOT::Core>:$ENV{LD_LIBRARY_PATH}
  )
endif()

add_edm4hep_test(pyunittests COMMAND python -m unittest discover -s ${CMAKE_CURRENT_LIST_DIR})
