find_package(Doxygen)
if(DOXYGEN_FOUND)
  # podio wraps its internal classes in a PODIO_INTERNAL conditional section
  set(DOXYGEN_ENABLED_SECTIONS "")
  if(DOCS_SHOW_PODIO_INTERNALS)
    set(DOXYGEN_ENABLED_SECTIONS PODIO_INTERNAL)
  endif()

  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in
                 ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doc/DoxygenLayout.xml ${CMAKE_CURRENT_BINARY_DIR}/DoxygenLayout.xml COPYONLY)
  # Doxygen does not remove outputs from previous runs, so start from scratch
  # to not keep pages from a previous configuration
  add_custom_target(doc
                    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_CURRENT_BINARY_DIR}/doxygen
                    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                    COMMENT "Generating API documentation with Doxygen" VERBATIM)

  install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doxygen
    DESTINATION ${CMAKE_INSTALL_DOCDIR}/doxygen
    OPTIONAL)
endif()
