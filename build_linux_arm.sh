#!/bin/bash

mkdir -p build/
cd build/
CROSS_COMPILE_DIR_X11=/opt/fsl-imx-x11/4.1.15-2.1.0/sysroots/cortexa9hf-neon-poky-linux-gnueabi
CMAKE_FIND_ROOT_PATH=$CROSS_COMPILE_DIR_X11/usr/include
CMAKE_PREFIX_PATH=$CROSS_COMPILE_DIR_X11/usr/lib/cmake
CMAKE_LIBRARY_PATH=$CROSS_COMPILE_DIR_X11/usr/lib

cmake -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR -DCMAKE_FIND_ROOT_PATH=$CMAKE_FIND_ROOT_PATH -DCMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH  -DCMAKE_LIBRARY_PATH=$CMAKE_LIBRARY_PATH ../
make
cd ..

cp -v build/lib/libgtest.a ../infistd/lib/linux/bin/arm