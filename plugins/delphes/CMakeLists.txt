

find_package(Delphes)


# build all executables and put them into bin/
add_executable(DelphesROOT_EDM4HEP DelphesROOT_EDM4HEP.cpp Delphes.cc)
target_include_directories(DelphesROOT_EDM4HEP ${DELPHES_INCLUDE_DIR})
target_link_libraries(DelphesROOT_EDM4HEP ${DELPHES_LIBRARY} edm4hep edm4hepDict podio::podioRootIO ROOT::Physics)
install(TARGETS DelphesROOT_EDM4HEP DESTINATION bin)
