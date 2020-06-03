IF(NOT BUILD_TESTING)
  RETURN()
ENDIF()

add_executable(write_events write_events.cc)
target_include_directories(write_events PUBLIC ${CMAKE_SOURCE_DIR}/edm4hep )
target_link_libraries_force_dict(write_events edm4hepDict edm4hep podio::podioRootIO )
add_test(NAME write_events COMMAND write_events)

add_executable(read_events read_events.cc)
target_include_directories(read_events PUBLIC ${CMAKE_SOURCE_DIR}/edm4hep )
target_link_libraries_force_dict(read_events edm4hepDict edm4hep podio::podioRootIO )
add_test(NAME read_events COMMAND read_events)
set_tests_properties(read_events PROPERTIES DEPENDS write_events)

IF(TARGET ROOT::ROOTDataFrame)
  add_executable(test_rdf test_rdf.cc)
  target_include_directories(test_rdf PUBLIC ${CMAKE_SOURCE_DIR}/edm4hep ${CMAKE_SOURCE_DIR}/dataframe )

  target_link_libraries_force_dict(test_rdf edm4hepDict edm4hepRDF ROOT::ROOTDataFrame )
  add_test(NAME test_rdf COMMAND test_rdf)
  set_tests_properties(test_rdf PROPERTIES DEPENDS write_events)
endif()


find_package(tricktrack 1.0.9)
if(tricktrack_FOUND)
  add_executable(edm4hep_tricktrack edm4hep_tricktrack.cc)
  target_link_libraries(edm4hep_tricktrack edm4hep ${tricktrack_LIBRARIES})
  target_include_directories(edm4hep_tricktrack SYSTEM PUBLIC ${tricktrack_INCLUDE_DIR})
  add_test(NAME edm4hep_tricktrack COMMAND edm4hep_tricktrack)
endif()



find_package(HepMC)
find_package(HepPDT)

if(HepMC_FOUND AND HepPDT_FOUND )
  add_executable(edm4hep_testhepmc hepmc/edm4hep_testhepmc.cc)
  target_include_directories(edm4hep_testhepmc PUBLIC ${HEPMC_INCLUDE_DIR} ${HEPPDT_INCLUDE_DIR} )
  target_link_libraries(edm4hep_testhepmc edm4hep podio::podioRootIO ${HEPPDT_LIBRARIES} ${HEPMC_LIBRARIES})
  add_test(NAME edm4hep_testhepmc COMMAND edm4hep_testhepmc)
endif()
