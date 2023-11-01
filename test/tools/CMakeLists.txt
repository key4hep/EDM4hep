if (nlohmann_json_FOUND)
  add_test(NAME convert_events
    COMMAND edm4hep2json ${CMAKE_CURRENT_BINARY_DIR}/../edm4hep_events.root)
  set_property(TEST convert_events PROPERTY DEPENDS write_events)
  set_test_env(convert_events)

  add_test(NAME test_convert_all_collections
           COMMAND python ${CMAKE_CURRENT_LIST_DIR}/test_all_collections.py
                   ${PROJECT_SOURCE_DIR}/edm4hep.yaml
                   ${PROJECT_SOURCE_DIR}/tools/include/edm4hep2json.hxx)
  set_test_env(test_convert_all_collections)
endif()
