#!/bin/bash
# Changes deb package version. (Run make package before this!)
# https://dittovto.atlassian.net/wiki/spaces/DIT/pages/238845953/Jenkins+Build+Pipeline
#
# Copyright: 2017 Ditto Technologies. All Rights Reserved.
# Author: Daran He

if [ "$#" -ne 1 ]; then
    prog=`basename "$0"`
    echo "Usage: ./$prog <REVISION>"
    exit 1
fi

REVISION=$1
BUILD_DIR=build
CPACK_CONFIG_FILE=$BUILD_DIR/CPackConfig.cmake
ARCH=`dpkg --print-architecture`

get_cmake_var() {
  echo `grep $1 $CPACK_CONFIG_FILE | tail -1 | cut -d '"' -f2`
}

VERSION_MAJOR=$(get_cmake_var CPACK_PACKAGE_VERSION_MAJOR)
VERSION_MINOR=$(get_cmake_var CPACK_PACKAGE_VERSION_MINOR)
VERSION_PATCH=$(get_cmake_var CPACK_PACKAGE_VERSION_PATCH)
VERSION="$VERSION_MAJOR.$VERSION_MINOR.$VERSION_PATCH"
VERSION_REVISION="$VERSION-$REVISION"

# Override existing variables in CPack config.
echo 'SET(CPACK_PACKAGE_VERSION "$VERSION_REVISION")' >> $CPACK_CONFIG_FILE

# Rebuild packages with new version info.
cd $BUILD_DIR
rm *.deb
cpack

# Rename all deb packages generated to ditto specifications.
for f in *.deb
do
  package_name=`dpkg-deb -I $f | grep Package: | awk '{print $NF}'`
  new_filename="${package_name}_${VERSION_REVISION}_${ARCH}.deb"
  echo "New Debian Filename: $new_filename";
  mv $f $new_filename
done
