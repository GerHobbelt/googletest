#!/bin/bash

mkdir -p build_release/
cd build_release
cmake -DCMAKE_BUILD_TYPE=release ../
make
cd ..

mkdir -p build_debug/
cd build_debug
cmake -DCMAKE_BUILD_TYPE=debug ../
make
cd ..

mkdir -p ../infistd/lib/linux/x86/release/
mkdir -p ../infistd/lib/linux/x86/debug/
cp build_release/lib/libgtest.a ../infistd/lib/linux/x86/bin/
cp build_debug/lib/libgtestd.a ../infistd/lib/linux/x86/debug/