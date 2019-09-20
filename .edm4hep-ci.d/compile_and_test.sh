
source init.sh
mkdir build install
cd build
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_CXX_STANDARD=17 .. && \
make -j4 && \
make install && \
ctest --output-on-failure
