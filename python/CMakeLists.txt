find_package(Python3 REQUIRED)

# Setup the python install dir. See the discussion in
# https://github.com/AIDASoft/podio/pull/599 for more details on why this is
# done the way it is
#
# For Python 3.13+ with free-threading (PEP 703), the site-packages directory
# includes a 't' suffix (e.g., python3.14t/site-packages). We use Python's
# own SITEARCH to get the correct path including any ABI suffixes.
#
set(edm4hep_python_lib_dir lib)
if("${Python3_SITEARCH}" MATCHES "/lib64/")
  set(edm4hep_python_lib_dir lib64)
endif()

# Extract the python-specific part of the path (e.g., python3.14t/site-packages)
# from Python3_SITEARCH
string(REGEX MATCH "python[0-9]+\\.[0-9]+[a-z]*/site-packages" _python_site_subdir "${Python3_SITEARCH}")
if(NOT _python_site_subdir)
  # Fallback to manual construction if regex fails
  set(_python_site_subdir "python${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR}/site-packages")
  message(WARNING "Could not extract site-packages path from Python3_SITEARCH (${Python3_SITEARCH}), using fallback: ${_python_site_subdir}")
endif()

set(EDM4HEP_PYTHON_INSTALLDIR
  "${CMAKE_INSTALL_PREFIX}/${edm4hep_python_lib_dir}/${_python_site_subdir}"
  CACHE STRING
  "The install prefix for the python bindings"
)

install(DIRECTORY edm4hep DESTINATION ${EDM4HEP_PYTHON_INSTALLDIR}
  REGEX .*\\.in EXCLUDE
  PATTERN __pycache__ EXCLUDE
)
