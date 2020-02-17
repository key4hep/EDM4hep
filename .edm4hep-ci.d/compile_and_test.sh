
source init.sh
mkdir build install
cd build
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_CXX_STANDARD=17 -G Ninja .. && \
    ninja -k0 && \
    ninja install && \
    ctest --output-on-failure
