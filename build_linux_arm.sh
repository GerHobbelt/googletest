#!/bin/bash

mkdir -p build/
cd build/
cmake ..
make
cd ..

cp build/lib/libgtest.a ../infistd/lib/linux/arm