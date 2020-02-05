

add_executable(write_events write_events.cc)
target_include_directories(write_events PUBLIC ${CMAKE_SOURCE_DIR}/edm4hep )
target_link_libraries(write_events edm4hep podio edm4hepDict )
add_test(NAME write_events COMMAND write_events)

add_executable(read_events read_events.cc)
target_include_directories(read_events PUBLIC ${CMAKE_SOURCE_DIR}/edm4hep )
target_link_libraries(read_events edm4hep podio edm4hepDict )
add_test(NAME read_events COMMAND read_events)


find_package(tricktrack 1.0.9)
if(tricktrack_FOUND)
add_executable(edm4hep_tricktrack edm4hep_tricktrack.cc)
target_include_directories(edm4hep_tricktrack PUBLIC ${CMAKE_SOURCE_DIR}/edm4hep podio ${tricktrack_INCLUDE_DIR} )
target_link_libraries(edm4hep_tricktrack edm4hep podio edm4hepDict ${tricktrack_LIBRARIES})
add_test(NAME edm4hep_tricktrack COMMAND edm4hep_tricktrack)
endif()



find_package(HepMC)
find_package(HepPDT)

if(HepMC_FOUND AND HepPDT_FOUND )
  add_executable(edm4hep_testhepmc hepmc/edm4hep_testhepmc.cc)
  target_include_directories(edm4hep_testhepmc PUBLIC ${CMAKE_SOURCE_DIR}/edm4hep ${HEPMC_INCLUDE_DIR} ${HEPPDT_INCLUDE_DIR} )
  target_link_libraries(edm4hep_testhepmc edm4hep podio edm4hepDict ${HEPPDT_LIBRARIES} ${HEPMC_LIBRARIES})
  add_test(NAME edm4hep_testhepmc COMMAND edm4hep_testhepmc)
endif()
