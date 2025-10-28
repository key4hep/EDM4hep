list( APPEND CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}" )

if (EDM4HEP_WITH_JSON)
  add_executable(edm4hep2json src/edm4hep2json.cxx)

  target_compile_definitions(edm4hep2json PUBLIC PODIO_JSON_OUTPUT)
  target_include_directories(edm4hep2json
                             PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

  target_link_libraries(edm4hep2json PUBLIC ROOT::Core
                                            ROOT::Tree
                                            podio::podio
                                            podio::podioIO
                                            edm4hep
                                            nlohmann_json::nlohmann_json)

  install(TARGETS edm4hep2json DESTINATION ${CMAKE_INSTALL_BINDIR})

endif()
