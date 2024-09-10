#---------------------------------------------------------------------------------------------------
#---edm4hep_set_linker_flags
#
#  Set linker flags
#
#---------------------------------------------------------------------------------------------------

macro(edm4hep_set_linker_flags)
  include(CheckCXXCompilerFlag)

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

endmacro()
