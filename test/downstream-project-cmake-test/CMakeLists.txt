cmake_minimum_required(VERSION 3.11)


project(DownstreamProjectUsingEDM4hep)

find_package(EDM4HEP)

# Use the EDM4HEP_VERSION in an if clause to trigger a cmake error in case it is
# unset.
if (${EDM4HEP_VERSION} VERSION_GREATER_EQUAL "0.4.1")
  message(STATUS "The current exported EDM4HEP version is " ${EDM4HEP_VERSION})
else()
  message(STATUS "The current exported EDM4HEP version is " ${EDM4HEP_VERSION})
endif()

add_executable(appUsingEDM4hep main.cxx)
target_link_libraries(appUsingEDM4hep EDM4HEP::edm4hep EDM4HEP::utils)
