source init.sh
git clone --depth 1 https://github.com/AIDASoft/podio || true
cd podio
mkdir build install
cd build
 cmake -DCMAKE_CXX_STANDARD=17 -DCMAKE_INSTALL_PREFIX=../install ..
make -j 4 install
cd ..
export CMAKE_PREFIX_PATH=$PWD/install:$CMAKE_PREFIX_PATH
export PODIO=$PWD/install:$CMAKE_PREFIX_PATH
cd ..
./.edm4hep-ci.d/compile_and_test.sh
