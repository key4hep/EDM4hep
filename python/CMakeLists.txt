find_package(Python3 REQUIRED)

# Setup the python install dir. See the discussion in
# https://github.com/AIDASoft/podio/pull/599 for more details on why this is
# done the way it is
set(edm4hep_python_lib_dir lib)
if("${Python3_SITEARCH}" MATCHES "/lib64/")
  set(edm4hep_python_lib_dir lib64)
endif()

set(EDM4HEP_PYTHON_INSTALLDIR
  "${CMAKE_INSTALL_PREFIX}/${edm4hep_python_lib_dir}/python${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR}/site-packages"
  CACHE STRING
  "The install prefix for the python bindings"
)

install(DIRECTORY edm4hep DESTINATION ${EDM4HEP_PYTHON_INSTALLDIR}
  REGEX .*\\.in EXCLUDE
  PATTERN __pycache__ EXCLUDE
)
