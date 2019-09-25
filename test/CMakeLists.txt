
find_package(HepMC)
find_package(HepPDT)

add_executable(edm4hep_testhepmc hepmc/edm4hep_testhepmc.cc)
target_include_directories(edm4hep_testhepmc PUBLIC ${CMAKE_SOURCE_DIR}/edm4hep )
target_link_libraries(edm4hep_testhepmc edm4hep podio edm4hepDict HepMC HepPDT)


add_test(NAME edm4hep_testhepmc COMMAND edm4hep_testhepmc)


