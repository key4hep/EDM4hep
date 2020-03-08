

find_package(Python 2 COMPONENTS Development)
find_package(Geant4)
find_package(DD4hep COMPONENTS DDCore DDG4 REQUIRED)

include( DD4hep )

dd4hep_configure_output()





#---------------------------  EDM4hep Plugins for new simulation framework  -----------
# target available with DD4hep > v00-11
IF(TARGET DD4hep::DDG4)
  dd4hep_add_plugin(DDG4EDM4hep SOURCES edm4hep/*.cpp)
  target_link_libraries(DDG4EDM4hep edm4hep DD4hep::DDCore DD4hep::DDG4)
else() # compatibility with older DD4hep versions
  dd4hep_package(DDG4EDM4hep)
  link_directories(${DD4hep_LIBRARY_DIRS})
  dd4hep_add_plugin(DDG4EDM4hep SOURCES edm4hep/*.cpp)
  target_link_libraries(DDG4EDM4hep ${DD4hep_LIBRARIES} ${Geant4_LIBRARIES} edm4hep)
  target_include_directories(DDG4EDM4hep PUBLIC ${DD4hep_INCLUDE_DIRS} ${Geant4_INCLUDE_DIRS})
  dd4hep_generate_rootmap( DDG4EDM4hep)
endif()

install(TARGETS DDG4EDM4hep
  # IMPORTANT: Add the datamodel library to the "export-set"
  EXPORT DDG4EDM4HEPTargets
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT bin
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT shlib
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/edm4hep"
  COMPONENT dev)

add_test(NAME ddsim_edm4hep COMMAND ddsim --compactFile /cvmfs/sw-nightlies.hsf.org/key4hep/views/latest/x86_64-centos7-gcc8-opt/DDDetectors/compact/SiD.xml -N 2 -G --gun.particle pi+ --outputFile my_edm4hep.root --part.userParticleHandler= )
set_property(TEST ddsim_edm4hep APPEND PROPERTY ENVIRONMENT "LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/edm4hep:$ENV{LD_LIBRARY_PATH}")
set_property(TEST ddsim_edm4hep APPEND PROPERTY ENVIRONMENT "DD4HEP_LIBRARY_PATH=${CMAKE_BINARY_DIR}/plugins/DDG4EDM4hep/lib/:$ENV{DD4HEP_LIBRARY_PATH}")
set_property(TEST ddsim_edm4hep APPEND PROPERTY ENVIRONMENT "LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/plugins/DDG4EDM4hep/lib/:$ENV{LD_LIBRARY_PATH}")


