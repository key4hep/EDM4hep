
# podio
git clone --depth 1 https://github.com/AIDASoft/podio || true
cd podio
mkdir build install
cd build
cmake -DCMAKE_CXX_STANDARD=17 -DBUILD_TESTING=OFF -DCMAKE_INSTALL_PREFIX=../install -G Ninja .. \
      && ninja -k0 \
      && ninja install \
          || exit 1

cd ..
export CMAKE_PREFIX_PATH=$PWD/install:$CMAKE_PREFIX_PATH
export PODIO=$PWD/install:$CMAKE_PREFIX_PATH
export LD_LIBRARY_PATH=$PWD/install/lib:$LD_LIBRARY_PATH
cd ..

