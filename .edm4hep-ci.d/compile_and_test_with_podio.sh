source init.sh

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
cd ..

# tricktrack
git clone --depth 1 https://github.com/HSF/TrickTrack || true
cd TrickTrack
mkdir build install
cd build
cmake -DCMAKE_CXX_STANDARD=17 -DBUILD_TESTING=OFF -DCMAKE_INSTALL_PREFIX=../install -G Ninja .. \
      && ninja -k0 \
      && ninja install
cd ..
export CMAKE_PREFIX_PATH=$PWD/install:$CMAKE_PREFIX_PATH
cd ..
# edm4hep
mkdir build install
cd build
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_CXX_STANDARD=17 -DCMAKE_CXX_FLAGS=" -fdiagnostics-color=always " -G Ninja ..  \
      && ninja -k0 \
      && ninja doc \
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
