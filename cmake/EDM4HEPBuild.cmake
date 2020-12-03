#---------------------------------------------------------------------------------------------------
#---edm4hep_set_rpath
#
#  Set optional rpath on linux and mandatory rpath on mac
#
#---------------------------------------------------------------------------------------------------
macro(edm4hep_set_rpath)
  #  When building, don't use the install RPATH already (but later on when installing)
  set(CMAKE_SKIP_BUILD_RPATH FALSE)         # don't skip the full RPATH for the build tree
  set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) # use always the build RPATH for the build tree
  set(CMAKE_MACOSX_RPATH TRUE)              # use RPATH for MacOSX
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE) # point to directories outside the build tree to the install RPATH

  # Check whether to add RPATH to the installation (the build tree always has the RPATH enabled)
  if(APPLE)
    set(CMAKE_INSTALL_NAME_DIR "@rpath")
    set(CMAKE_INSTALL_RPATH "@loader_path/../lib")    # self relative LIBDIR
    # the RPATH to be used when installing, but only if it's not a system directory
    list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
    if("${isSystemDir}" STREQUAL "-1")
      set(CMAKE_INSTALL_RPATH "@loader_path/../lib")
    endif("${isSystemDir}" STREQUAL "-1")
  elseif(EDM4HEP_SET_RPATH)
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${LIBDIR}") # install LIBDIR
    # the RPATH to be used when installing, but only if it's not a system directory
    list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
    if("${isSystemDir}" STREQUAL "-1")
      set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${LIBDIR}")
    endif("${isSystemDir}" STREQUAL "-1")
  else()
    set(CMAKE_SKIP_INSTALL_RPATH TRUE)           # skip the full RPATH for the install tree
  endif()
endmacro(edm4hep_set_rpath)

#---------------------------------------------------------------------------------------------------
#---edm4hep_set_compiler_flags
#
#  Set compiler and linker flags
#
#---------------------------------------------------------------------------------------------------

macro(edm4hep_set_compiler_flags)
  include(CheckCXXCompilerFlag)

  SET(COMPILER_FLAGS -fPIC -Wall -Wextra -Wpedantic -Wno-unused-variable -Wno-unused-parameter)

  # AppleClang/Clang specific warning flags
  if(CMAKE_CXX_COMPILER_ID MATCHES "^(Apple)?Clang$")
    set ( COMPILER_FLAGS ${COMPILER_FLAGS} -Winconsistent-missing-override -Wheader-hygiene )
  endif()

  FOREACH( FLAG ${COMPILER_FLAGS} )
    ## need to replace the minus or plus signs from the variables, because it is passed
    ## as a macro to g++ which causes a warning about no whitespace after macro
    ## definition
    STRING(REPLACE "-" "_" FLAG_WORD ${FLAG} )
    STRING(REPLACE "+" "P" FLAG_WORD ${FLAG_WORD} )

    CHECK_CXX_COMPILER_FLAG( "${FLAG}" CXX_FLAG_WORKS_${FLAG_WORD} )
    IF( ${CXX_FLAG_WORKS_${FLAG_WORD}} )
      message( STATUS "Adding ${FLAG} to CXX_FLAGS" )
      SET ( CMAKE_CXX_FLAGS "${FLAG} ${CMAKE_CXX_FLAGS} ")
    ELSE()
      message( STATUS "NOT Adding ${FLAG} to CXX_FLAGS" )
    ENDIF()
  ENDFOREACH()
  
  # resolve which linker we use
  execute_process(COMMAND ${CMAKE_CXX_COMPILER} -Wl,--version OUTPUT_VARIABLE stdout ERROR_QUIET)
  if("${stdout}" MATCHES "GNU ")
    set(LINKER_TYPE "GNU")
    message( STATUS "Detected GNU compatible linker" )
  else()
    execute_process(COMMAND ${CMAKE_CXX_COMPILER} -Wl,-v ERROR_VARIABLE stderr )
    if(("${stderr}" MATCHES "PROGRAM:ld") AND ("${stderr}" MATCHES "PROJECT:ld64"))
      set(LINKER_TYPE "APPLE")
      message( STATUS "Detected Apple linker" )
    else()
      set(LINKER_TYPE "unknown")
      message( STATUS "Detected unknown linker" )
    endif()
  endif()

  if("${LINKER_TYPE}" STREQUAL "APPLE")
    SET ( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-undefined,dynamic_lookup")
  elseif("${LINKER_TYPE}" STREQUAL "GNU")
    SET ( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--allow-shlib-undefined")
  else()
    MESSAGE( WARNING "No known linker (GNU or Apple) has been detected, pass no flags to linker" )
  endif()

endmacro(edm4hep_set_compiler_flags)
