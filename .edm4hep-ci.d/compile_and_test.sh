
# edm4hep
mkdir build install
cd build
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_CXX_STANDARD=${STANDARD:=17} -DBUILD_DDG4EDM4HEP=${BUILD_DDG4EDM4HEP:=ON} -DBUILD_DELPHESEDM4HEP=${BUILD_DELPHESEDM4HEP:=ON} -DCMAKE_CXX_FLAGS=" -fdiagnostics-color=always " -DEDM4HEP_DOCUMENTATION=ON -G Ninja ..  \
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
