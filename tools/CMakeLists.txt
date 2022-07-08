add_executable(edm4hep2json src/edm4hep2json.cxx)

target_include_directories (edm4hep2json
                            PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(EDM4HEP_SET_RPATH TRUE)
target_link_libraries (edm4hep2json PUBLIC ROOT::Core
                                           ROOT::Tree
                                           podio::podio
                                           podio::podioRootIO
                                           edm4hep
                                           nlohmann_json::nlohmann_json)

install(TARGETS edm4hep2json DESTINATION bin)
