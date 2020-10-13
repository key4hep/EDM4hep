

find_package(Delphes)

add_library(DelphesEDM4HepConverter SHARED DelphesEDM4HepConverter.cc)
target_include_directories(DelphesEDM4HepConverter SYSTEM PUBLIC ${DELPHES_INCLUDE_DIRS})
target_link_libraries(DelphesEDM4HepConverter edm4hep ${DELPHES_LIBRARY} podio::podioRootIO ROOT::MathCore ROOT::EG ROOT::Physics)
set_target_properties(DelphesEDM4HepConverter PROPERTIES PUBLIC_HEADER "DelphesEDM4HepConverter.h;Delphes.h")
install(TARGETS DelphesEDM4HepConverter 
        EXPORT EDM4HEPTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION include/edm4hep
        COMPONENT dev)

# build all executables and put them into bin/
add_executable(DelphesROOT_EDM4HEP DelphesROOT_EDM4HEP.cpp Delphes.cc)
target_link_libraries(DelphesROOT_EDM4HEP DelphesEDM4HepConverter ROOT::Physics)
install(TARGETS DelphesROOT_EDM4HEP DESTINATION bin)

# build all executables and put them into bin/
add_executable(DelphesHepMC_EDM4HEP DelphesHepMC_EDM4HEP.cpp Delphes.cc)
target_link_libraries(DelphesHepMC_EDM4HEP DelphesEDM4HepConverter)
install(TARGETS DelphesHepMC_EDM4HEP DESTINATION bin)

# build all executables and put them into bin/
add_executable(DelphesSTDHEP_EDM4HEP DelphesSTDHEP_EDM4HEP.cpp Delphes.cc)
target_link_libraries(DelphesSTDHEP_EDM4HEP DelphesEDM4HepConverter)
install(TARGETS DelphesSTDHEP_EDM4HEP DESTINATION bin)

find_package(Pythia8)
if(Pythia8_FOUND)
  add_executable(DelphesPythia8_EDM4HEP DelphesPythia8_EDM4HEP.cpp Delphes.cc)
  target_include_directories(DelphesPythia8_EDM4HEP PRIVATE ${PYTHIA8_INCLUDE_DIRS})
  target_link_libraries(DelphesPythia8_EDM4HEP DelphesEDM4HepConverter ${PYTHIA8_LIBRARIES} ${DELPHES_LIBRARY})
  install(TARGETS DelphesPythia8_EDM4HEP DESTINATION bin)
endif()
