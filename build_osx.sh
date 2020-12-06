#!/bin/bash

mkdir -p build/
cd build
/Applications/CMake.app/Contents/bin/cmake -DCMAKE_CXX_COMPILER="c++" -DCMAKE_CXX_FLAGS="-std=c++11 -stdlib=libc++" ../
make
cd ..

cp -v build/lib/libgtest.a ../infistd/lib/osx/