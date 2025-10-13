if (nlohmann_json_FOUND)
  add_test(NAME convert_events
    COMMAND edm4hep2json ${CMAKE_CURRENT_BINARY_DIR}/../edm4hep_events.root)
  set_property(TEST convert_events PROPERTY DEPENDS write_events)
  set_test_env(convert_events)
endif()
