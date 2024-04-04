add_library(kinematics INTERFACE)
add_library(EDM4HEP::kinematics ALIAS kinematics)

target_include_directories(kinematics
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(kinematics PUBLIC INTERFACE ROOT::Core)
target_compile_features(kinematics INTERFACE cxx_std_17)

set(utils_sources
  src/ParticleIDUtils.cc
)

add_library(utils SHARED ${utils_sources})
set_target_properties(utils PROPERTIES OUTPUT_NAME "edm4hepUtils")
add_library(EDM4HEP::utils ALIAS utils)
target_include_directories(utils
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(utils PUBLIC EDM4HEP::edm4hep kinematics)


set(sources src/dataframe.cc)
set(headers include/edm4hep/utils/dataframe.h)

add_library(edm4hepRDF SHARED ${sources})
target_include_directories(edm4hepRDF PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )
target_link_libraries(edm4hepRDF PUBLIC edm4hep ROOT::Physics ROOT::ROOTVecOps)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  PATTERN "CMakeLists.txt" EXCLUDE)

install(TARGETS utils kinematics edm4hepRDF
  EXPORT EDM4HEPTargets
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT bin
  RUNTIME DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT shlib
  COMPONENT dev
  )
