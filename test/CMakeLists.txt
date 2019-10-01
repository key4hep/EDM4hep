
find_package(HepMC)
find_package(HepPDT)

if(HepMC_FOUND AND HepPDT_FOUND )
  add_executable(edm4hep_testhepmc hepmc/edm4hep_testhepmc.cc)
  target_include_directories(edm4hep_testhepmc PUBLIC ${CMAKE_SOURCE_DIR}/edm4hep ${HEPMC_INCLUDE_DIR} ${HEPPDT_INCLUDE_DIR} )
  target_link_libraries(edm4hep_testhepmc edm4hep podio edm4hepDict ${HEPPDT_LIBRARIES} ${HEPMC_LIBRARIES})
  add_test(NAME edm4hep_testhepmc COMMAND edm4hep_testhepmc)
endif()

