
export K4VIEW=/cvmfs/sw-nightlies.hsf.org/key4hep/views/latest
export BINARY_TAG=x86_64-centos7-gcc8-opt
source $K4VIEW/$BINARY_TAG/setup.sh



# edm4hep
mkdir build install
cd build
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_CXX_STANDARD=${STANDARD:=17} -DCMAKE_CXX_FLAGS=" -fdiagnostics-color=always " -G Ninja ..  \
      && ninja -k0 \
      && ninja install \
      && ctest --output-on-failure || exit 1
# test if installation can be used by external projects
cd ..
export CMAKE_PREFIX_PATH=$PWD/install:$CMAKE_PREFIX_PATH
cd test/downstream-project-cmake-test
mkdir build install
cd build
cmake -DCMAKE_CXX_STANDARD=${STANDARD} -DCMAKE_INSTALL_PREFIX=../install -G Ninja .. \
      && ninja -k0
