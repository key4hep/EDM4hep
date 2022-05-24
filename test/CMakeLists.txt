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

IF (podio_VERSION VERSION_GREATER "0.13")
  add_executable(reco_particle_ref reco_particle_ref.cc)
  target_include_directories(reco_particle_ref PUBLIC ${CMAKE_SOURCE_DIR}/edm4hep)
  target_link_libraries(reco_particle_ref edm4hep podio::podioRootIO)
  add_test(NAME reco_particle_ref COMMAND reco_particle_ref)
  set_tests_properties(reco_particle_ref PROPERTIES
    ENVIRONMENT
    LD_LIBRARY_PATH=$<TARGET_FILE_DIR:edm4hep>:$<TARGET_FILE_DIR:podio::podio>:$ENV{LD_LIBRARY_PATH}
  )
ENDIF()

IF(TARGET edm4hepSioBlocks)
  add_executable(write_events_sio write_events_sio.cc)
  target_include_directories(write_events_sio PUBLIC ${CMAKE_SOURCE_DIR}/edm4hep)
  target_link_libraries(write_events_sio edm4hep podio::podioSioIO)
  add_test(NAME write_events_sio COMMAND write_events_sio)
  set_tests_properties(write_events_sio PROPERTIES
    ENVIRONMENT LD_LIBRARY_PATH=$<TARGET_FILE_DIR:edm4hep>:$<TARGET_FILE_DIR:podio::podio>:$ENV{LD_LIBRARY_PATH}
    )

  add_executable(read_events_sio read_events_sio.cc)
  target_include_directories(read_events_sio PUBLIC ${CMAKE_SOURCE_DIR}/edm4hep)
  target_link_libraries(read_events_sio edm4hep podio::podioSioIO)
  add_test(NAME read_events_sio COMMAND read_events_sio)
  set_tests_properties(read_events_sio PROPERTIES
    ENVIRONMENT LD_LIBRARY_PATH=$<TARGET_FILE_DIR:edm4hep>:$<TARGET_FILE_DIR:podio::podio>:$ENV{LD_LIBRARY_PATH}
    DEPENDS write_events_sio
    )
ENDIF()

IF(TARGET ROOT::ROOTDataFrame)
  add_executable(test_rdf test_rdf.cc)
  target_include_directories(test_rdf PUBLIC ${CMAKE_SOURCE_DIR}/edm4hep ${CMAKE_SOURCE_DIR}/dataframe )
  target_link_libraries(test_rdf edm4hepRDF ROOT::ROOTDataFrame)
  add_test(NAME test_rdf COMMAND test_rdf)
  set_tests_properties(test_rdf PROPERTIES
    ENVIRONMENT LD_LIBRARY_PATH=$<TARGET_FILE_DIR:edm4hep>:$<TARGET_FILE_DIR:podio::podio>:$ENV{LD_LIBRARY_PATH}
    DEPENDS write_events
    )
endif()


find_package(HepMC3)
find_package(HepPDT)

if(HepMC3_FOUND AND HepPDT_FOUND )
  add_executable(edm4hep_testhepmc hepmc/edm4hep_testhepmc.cc)
  target_include_directories(edm4hep_testhepmc PUBLIC ${HEPMC3_INCLUDE_DIR} ${HEPPDT_INCLUDE_DIR} )
  target_link_libraries(edm4hep_testhepmc edm4hep podio::podioRootIO ${HEPPDT_LIBRARIES} ${HEPMC3_LIBRARIES})
  add_test(NAME edm4hep_testhepmc COMMAND edm4hep_testhepmc)
  set_tests_properties(edm4hep_testhepmc PROPERTIES
    ENVIRONMENT LD_LIBRARY_PATH=$<TARGET_FILE_DIR:edm4hep>:$<TARGET_FILE_DIR:podio::podio>:$ENV{LD_LIBRARY_PATH}
    )
endif()

add_subdirectory(utils)
