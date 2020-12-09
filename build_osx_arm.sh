#!/bin/bash

mkdir -p build/
cd build
/Applications/CMake.app/Contents/bin/cmake -DCMAKE_CXX_COMPILER="c++" -DCMAKE_CXX_FLAGS="-std=c++11 -stdlib=libc++" -DCMAKE_OSX_ARCHITECTURES="arm64" ../
make
cd ..

mkdir -p ../infistd/lib/osx/arm/
cp -v build/lib/libgtest.a ../infistd/lib/osx/arm/