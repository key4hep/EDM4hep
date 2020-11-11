
# podio
git clone --depth 1 https://github.com/AIDASoft/podio || true
cd podio
mkdir build install
cd build
cmake -DCMAKE_CXX_STANDARD=17 -DBUILD_TESTING=OFF -DCMAKE_INSTALL_PREFIX=$PWD/../install -DENABLE_SIO=ON -G Ninja .. \
      && ninja -k0 \
      && ninja install \
          || exit 1

cd ..
# Place this installation in front of any other which might be present in the
# setup
export ROOT_INCLUDE_PATH=$PWD/install/include:$ROOT_INCLUDE_PATH:$CPATH
unset CPATH
export CMAKE_PREFIX_PATH=$PWD/install:$CMAKE_PREFIX_PATH
export LD_LIBRARY_PATH=$PWD/install/lib:$PWD/install/lib64:$LD_LIBRARY_PATH
cd ..
