

find_package(Delphes)

# build all executables and put them into bin/
add_executable(DelphesROOT_EDM4HEP DelphesROOT_EDM4HEP.cpp Delphes.cc)
target_include_directories(DelphesROOT_EDM4HEP PRIVATE ${DELPHES_INCLUDE_DIRS})
target_link_libraries(DelphesROOT_EDM4HEP ${DELPHES_LIBRARY} edm4hep edm4hepDict podio::podioRootIO ROOT::Physics  ROOT::GenVector ROOT::MathCore)
install(TARGETS DelphesROOT_EDM4HEP DESTINATION bin)
