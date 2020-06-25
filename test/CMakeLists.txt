IF(NOT BUILD_TESTING)
  RETURN()
ENDIF()

add_executable(write_events write_events.cc)
target_include_directories(write_events PUBLIC ${CMAKE_SOURCE_DIR}/edm4hep )
target_link_libraries(write_events edm4hep podio::podioRootIO)
add_test(NAME write_events COMMAND write_events)
set_tests_properties(write_events PROPERTIES
  ENVIRONMENT LD_LIBRARY_PATH=$<TARGET_FILE_DIR:edm4hep>:$<TARGET_FILE_DIR:podio::podio>:$ENV{LD_LIBRARY_PATH}
  )

add_executable(read_events read_events.cc)
target_include_directories(read_events PUBLIC ${CMAKE_SOURCE_DIR}/edm4hep )
target_link_libraries(read_events edm4hep podio::podioRootIO)
add_test(NAME read_events COMMAND read_events)
set_tests_properties(read_events PROPERTIES
  DEPENDS write_events
  ENVIRONMENT LD_LIBRARY_PATH=$<TARGET_FILE_DIR:edm4hep>:$<TARGET_FILE_DIR:podio::podio>:$ENV{LD_LIBRARY_PATH}
  )

add_executable(reco_particle_ref reco_particle_ref.cc)
target_include_directories(reco_particle_ref PUBLIC ${CMAKE_SOURCE_DIR}/edm4hep)
target_link_libraries(reco_particle_ref edm4hep podio::podioRootIO)
add_test(NAME reco_particle_ref COMMAND reco_particle_ref)
set_tests_properties(reco_particle_ref PROPERTIES
  ENVIRONMENT
  LD_LIBRARY_PATH=$<TARGET_FILE_DIR:edm4hep>:$<TARGET_FILE_DIR:podio::podio>:$ENV{LD_LIBRARY_PATH}
  )

IF(TARGET ROOT::ROOTDataFrame)
  add_executable(test_rdf test_rdf.cc)
  target_include_directories(test_rdf PUBLIC ${CMAKE_SOURCE_DIR}/edm4hep ${CMAKE_SOURCE_DIR}/dataframe )
  target_link_libraries(test_rdf edm4hepRDF ROOT::ROOTDataFrame)
  add_test(NAME test_rdf COMMAND test_rdf)
  set_tests_properties(test_rdf PROPERTIES
    ENVIRONMENT LD_LIBRARY_PATH=$<TARGET_FILE_DIR:edm4hep>:$<TARGET_FILE_DIR:podio::podio>:$ENV{LD_LIBRARY_PATH}
    )
endif()


find_package(tricktrack 1.0.9)
if(tricktrack_FOUND)
  add_executable(edm4hep_tricktrack edm4hep_tricktrack.cc)
  target_link_libraries(edm4hep_tricktrack edm4hep ${tricktrack_LIBRARIES})
  target_include_directories(edm4hep_tricktrack SYSTEM PUBLIC ${tricktrack_INCLUDE_DIR})
  add_test(NAME edm4hep_tricktrack COMMAND edm4hep_tricktrack)
  set_tests_properties(edm4hep_tricktrack PROPERTIES
    ENVIRONMENT LD_LIBRARY_PATH=$<TARGET_FILE_DIR:edm4hep>:$<TARGET_FILE_DIR:podio::podio>:$ENV{LD_LIBRARY_PATH}
    )
endif()



find_package(HepMC)
find_package(HepPDT)

if(HepMC_FOUND AND HepPDT_FOUND )
  add_executable(edm4hep_testhepmc hepmc/edm4hep_testhepmc.cc)
  target_include_directories(edm4hep_testhepmc PUBLIC ${HEPMC_INCLUDE_DIR} ${HEPPDT_INCLUDE_DIR} )
  target_link_libraries(edm4hep_testhepmc edm4hep podio::podioRootIO ${HEPPDT_LIBRARIES} ${HEPMC_LIBRARIES})
  add_test(NAME edm4hep_testhepmc COMMAND edm4hep_testhepmc)
  set_tests_properties(edm4hep_testhepmc PROPERTIES
    ENVIRONMENT LD_LIBRARY_PATH=$<TARGET_FILE_DIR:edm4hep>:$<TARGET_FILE_DIR:podio::podio>:$ENV{LD_LIBRARY_PATH}
    )
endif()
