if(USE_EXTERNAL_CATCH2)
  find_package(Catch2 REQUIRED)
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
endif()

include(Catch)

add_executable(unittests_edm4hep
  test_kinematics.cpp test_vector_utils.cpp test_covmatrix_utils.cpp test_PIDHandler.cpp)

target_link_libraries(unittests_edm4hep edm4hep EDM4HEP::utils Catch2::Catch2 Catch2::Catch2WithMain)

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
  # it fails to succesfully launch the executable and execute any test. Here
  # we just include them in order to have them show up as failing
  add_test(NAME unittests COMMAND unittests_edm4hep)
  set_property(TEST unittests
    PROPERTY ENVIRONMENT
    LD_LIBRARY_PATH=$<TARGET_FILE_DIR:edm4hep>:$<TARGET_FILE_DIR:podio::podio>:$<TARGET_FILE_DIR:ROOT::Core>:$ENV{LD_LIBRARY_PATH}
    PYTHONPATH=${PROJECT_SOURCE_DIR}/python:$ENV{PYTHONPATH}
    ROOT_INCLUDE_PATH=${PROJECT_SOURCE_DIR}/edm4hep:${PROJECT_SOURCE_DIR}/utils/include:$ENV{ROOT_INCLUDE_PATH}
  )
else()
  catch_discover_tests(unittests_edm4hep
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DL_PATHS $<TARGET_FILE_DIR:edm4hep>:$<TARGET_FILE_DIR:podio::podio>:$<TARGET_FILE_DIR:ROOT::Core>:$ENV{LD_LIBRARY_PATH}
  )
endif()

add_test(NAME pyunittests COMMAND python -m unittest discover -s ${CMAKE_CURRENT_LIST_DIR})
set_property(TEST pyunittests
  PROPERTY ENVIRONMENT
  LD_LIBRARY_PATH=$<TARGET_FILE_DIR:edm4hep>:$<TARGET_FILE_DIR:podio::podio>:$<$<TARGET_EXISTS:edm4hepRDF>:$<TARGET_FILE_DIR:edm4hepRDF>:>$<TARGET_FILE_DIR:ROOT::Core>:$ENV{LD_LIBRARY_PATH}
  PYTHONPATH=${PROJECT_SOURCE_DIR}/python:$ENV{PYTHONPATH}
  ROOT_INCLUDE_PATH=${PROJECT_SOURCE_DIR}/edm4hep:${PROJECT_SOURCE_DIR}/utils/include:$ENV{ROOT_INCLUDE_PATH}
  )
