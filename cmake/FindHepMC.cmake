# - Locate HepMC library
# Defines:
#
#  HEPMC_FOUND
#  HEPMC_INCLUDE_DIR
#  HEPMC_INCLUDE_DIRS (not cached)
#  HEPMC_<component>_LIBRARY
#  HEPMC_LIBRARIES (not cached)
#  HEPMC_LIBRARY_DIRS (not cached)

find_path(HEPMC_INCLUDE_DIR HepMC/GenEvent.h)
set(HEPMC_INCLUDE_DIRS ${HEPMC_INCLUDE_DIR})

if(NOT HepMC_FIND_COMPONENTS)
  set(HepMC_FIND_COMPONENTS HepMC)
endif()
foreach(component ${HepMC_FIND_COMPONENTS})
  find_library(HEPMC_${component}_LIBRARY NAMES HepMC${component} ${component})
  mark_as_advanced(HEPMC_${component}_LIBRARY)
  list(APPEND HEPMC_LIBRARIES ${HEPMC_${component}_LIBRARY})
  get_filename_component(_comp_dir ${HEPMC_${component}_LIBRARY} PATH)
  list(APPEND HEPMC_LIBRARY_DIRS ${_comp_dir})
endforeach()
if(HEPMC_LIBRARY_DIRS)
  list(REMOVE_DUPLICATES HEPMC_LIBRARY_DIRS)
endif()

# handle the QUIETLY and REQUIRED arguments and set HEPMC_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(HepMC DEFAULT_MSG HEPMC_INCLUDE_DIR HEPMC_LIBRARIES)

mark_as_advanced(HEPMC_FOUND HEPMC_INCLUDE_DIR)

