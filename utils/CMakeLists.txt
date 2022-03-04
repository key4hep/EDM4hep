add_library(kinematics INTERFACE)
add_library(EDM4HEP::kinematics ALIAS kinematics)

target_include_directories(kinematics
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(kinematics PUBLIC INTERFACE ROOT::Core)
target_compile_features(kinematics INTERFACE cxx_std_17)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  PATTERN "CMakeLists.txt" EXCLUDE)


add_library(utils INTERFACE)
add_library(EDM4HEP::utils ALIAS utils)
target_link_libraries(utils INTERFACE kinematics)

install(TARGETS utils kinematics
  EXPORT EDM4HEPTargets
  )
