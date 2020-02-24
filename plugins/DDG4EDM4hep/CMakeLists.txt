

find_package(Geant4)
find_package(DD4hep COMPONENTS DDCore DDG4 REQUIRED)


dd4hep_configure_output()

dd4hep_package(DDG4EDM4hep)


# TODO: remove when DD4hep version > v00-10 available
link_directories(${DD4hep_LIBRARY_DIRS})

set(DDG4EDM4hep_INSTALL  ${CMAKE_INSTALL_PREFIX}/examples/DDG4EDM4hep_INSTALL)

#---------------------------  EDM4hep Plugins for new simulation framework  -----------
dd4hep_add_plugin(DDG4EDM4hep SOURCES edm4hep/*.cpp)
target_link_libraries(DDG4EDM4hep ${DD4hep_LIBRARIES} ${Geant4_LIBRARIES} edm4hep DDG4)
target_include_directories(DDG4EDM4hep PUBLIC ${DD4hep_INCLUDE_DIRS} ${Geant4_INCLUDE_DIRS})

dd4hep_generate_rootmap( DDG4EDM4hep)
