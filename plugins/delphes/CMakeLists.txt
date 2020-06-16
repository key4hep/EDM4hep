

find_package(Delphes)

add_library(DelphesEDM4HepConverter SHARED DelphesEDM4HepConverter.cc DelphesEDM4HepConverter.h)
target_include_directories(DelphesEDM4HepConverter PRIVATE ${DELPHES_INCLUDE_DIRS}
  PRIVATE ${podio_INCLUDE_DIR})
target_link_libraries(DelphesEDM4HepConverter edm4hepDict edm4hep ${DELPHES_LIBRARY} podio::podioRootIO ROOT::MathCore ROOT::EG)
install(TARGETS DelphesEDM4HepConverter DESTINATION lib)

# build all executables and put them into bin/
add_executable(DelphesROOT_EDM4HEP DelphesROOT_EDM4HEP.cpp Delphes.cc)
target_include_directories(DelphesROOT_EDM4HEP PRIVATE ${DELPHES_INCLUDE_DIRS})
target_link_libraries(DelphesROOT_EDM4HEP ${DELPHES_LIBRARY} edm4hep edm4hepDict podio::podioRootIO ROOT::Physics  ROOT::GenVector ROOT::MathCore)
install(TARGETS DelphesROOT_EDM4HEP DESTINATION bin)

# build all executables and put them into bin/
add_executable(DelphesHepMC_EDM4HEP DelphesHepMC_EDM4HEP.cpp Delphes.cc)
target_include_directories(DelphesHepMC_EDM4HEP PRIVATE ${DELPHES_INCLUDE_DIRS})
target_link_libraries(DelphesHepMC_EDM4HEP ${DELPHES_LIBRARY} edm4hep edm4hepDict podio::podioRootIO ROOT::Physics  ROOT::GenVector ROOT::MathCore)
install(TARGETS DelphesHepMC_EDM4HEP DESTINATION bin)

# build all executables and put them into bin/
add_executable(DelphesSTDHEP_EDM4HEP DelphesSTDHEP_EDM4HEP.cpp Delphes.cc)
target_include_directories(DelphesSTDHEP_EDM4HEP PRIVATE ${DELPHES_INCLUDE_DIRS})
target_link_libraries(DelphesSTDHEP_EDM4HEP ${DELPHES_LIBRARY} edm4hep edm4hepDict podio::podioRootIO ROOT::Physics  ROOT::GenVector ROOT::MathCore)
install(TARGETS DelphesSTDHEP_EDM4HEP DESTINATION bin)

find_package(Pythia8)
find_package(ROOT COMPONENTS EG Eve Geom Gui GuiHtml GenVector Hist Physics Matrix Graf RIO Tree Gpad RGL MathCore)

# build all executables and put them into bin/
add_executable(DelphesPythia8_EDM4HEP DelphesPythia8_EDM4HEP.cpp Delphes.cc)
target_include_directories(DelphesPythia8_EDM4HEP PRIVATE ${DELPHES_INCLUDE_DIRS} ${PYTHIA8_INCLUDE_DIRS})
target_link_libraries(DelphesPythia8_EDM4HEP DelphesEDM4HepConverter ${PYTHIA8_LIBRARIES})
install(TARGETS DelphesPythia8_EDM4HEP DESTINATION bin)
