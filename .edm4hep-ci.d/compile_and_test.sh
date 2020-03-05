source init.sh

export CMAKE_PREFIX_PATH=/cvmfs/sw-nightlies.hsf.org/key4hep/packageviews/podio/latest/x86_64-centos7-gcc8-opt:$CMAKE_PREFIX_PATH
export CMAKE_PREFIX_PATH=/cvmfs/sw-nightlies.hsf.org/key4hep/packageviews/tricktrack/latest/x86_64-centos7-gcc8-opt:$CMAKE_PREFIX_PATH
export CMAKE_PREFIX_PATH=/cvmfs/sw-nightlies.hsf.org/key4hep/packageviews/DD4hep/latest/x86_64-centos7-gcc8-opt:$CMAKE_PREFIX_PATH


# edm4hep
mkdir build install
cd build
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_CXX_STANDARD=17 -DCMAKE_CXX_FLAGS=" -fdiagnostics-color=always " -G Ninja ..  \
      && ninja -k0 \
      && ninja install \
      && ctest --output-on-failure || exit 1
# test if installation can be used by external projects
cd ..
export CMAKE_PREFIX_PATH=$PWD/install:$CMAKE_PREFIX_PATH
cd test/downstream-project-cmake-test
mkdir build install
cd build
cmake -DCMAKE_CXX_STANDARD=17 -DCMAKE_INSTALL_PREFIX=../install -G Ninja .. \
      && ninja -k0
