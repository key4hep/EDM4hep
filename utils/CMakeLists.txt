add_library(kinematics INTERFACE)
add_library(EDM4HEP::kinematics ALIAS kinematics)

target_include_directories(kinematics
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(kinematics PUBLIC INTERFACE ROOT::Core)
target_compile_features(kinematics INTERFACE cxx_std_17)

add_library(utils INTERFACE)
add_library(EDM4HEP::utils ALIAS utils)
target_link_libraries(utils INTERFACE kinematics)


set(sources src/dataframe.cc)
set(headers include/edm4hep/utils/dataframe.h)

add_library(edm4hepRDF SHARED ${sources})
target_include_directories(edm4hepRDF PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )
target_link_libraries(edm4hepRDF edm4hep ROOT::Physics ROOT::ROOTVecOps)
PODIO_GENERATE_DICTIONARY(edm4hepRDF "${headers}" SELECTION src/selection.xml
  OPTIONS --library ${CMAKE_SHARED_LIBRARY_PREFIX}edm4hepRDF${CMAKE_SHARED_LIBRARY_SUFFIX}
  )
set_target_properties(edm4hepRDF-dictgen PROPERTIES EXCLUDE_FROM_ALL TRUE)


install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  PATTERN "CMakeLists.txt" EXCLUDE)

install(FILES
  "${PROJECT_BINARY_DIR}/utils/edm4hepRDFDict.rootmap"
  "${PROJECT_BINARY_DIR}/utils/libedm4hepRDF_rdict.pcm"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT dev)

install(TARGETS utils kinematics edm4hepRDF
  EXPORT EDM4HEPTargets
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT bin
  RUNTIME DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT shlib
  COMPONENT dev
  )
