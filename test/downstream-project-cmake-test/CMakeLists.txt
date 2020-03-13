cmake_minimum_required(VERSION 3.11)


project(DownstreamProjectUsingEDM4hep)

find_package(EDM4HEP)

add_executable(appUsingEDM4hep main.cxx)
target_link_libraries(appUsingEDM4hep EDM4HEP::edm4hep)
