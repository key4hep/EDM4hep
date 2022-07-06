# find_package(nlohmann_json)
# find_package(ROOT COMPONENTS RIO Tree)
# find_package(EDM4HEP)

add_executable(edm4hep2json src/edm4hep2json.cxx)

target_include_directories (edm4hep2json
                            PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries (edm4hep2json LINK_PUBLIC ROOT::Core
                                                ROOT::Tree
                                                podio::podio
                                                podio::podioRootIO
                                                edm4hep)

install(TARGETS edm4hep2json DESTINATION bin)
