#!/bin/bash
# Changes deb package version. Requires Version.cmake (Run make package before this!)
# https://dittovto.atlassian.net/wiki/spaces/DIT/pages/238845953/Jenkins+Build+Pipeline
#
# Copyright: 2017 Ditto Technologies. All Rights Reserved.
# Author: Daran He

if [ "$#" -ne 2 ]; then
    prog=`basename "$0"`
    echo "Usage: ./$prog <VERSION> <REVISION>"
    exit 1
fi

VERSION=$1
REVISION=$2
BUILD_DIR=build
CPACK_CONFIG_FILE=$BUILD_DIR/CPackConfig.cmake
DITTO_VERSION_FILE=DittoVersion.cmake
ARCH=`dpkg --print-architecture`
VERSION_REVISION="$VERSION-$REVISION"

# Override version information in CPack configuration.
cat $DITTO_VERSION_FILE >> $CPACK_CONFIG_FILE
echo 'SET(CPACK_PACKAGE_VERSION "$VERSION_REVISION")' >> $CPACK_CONFIG_FILE

# Rebuild packages with new version info.
cd $BUILD_DIR
rm *.deb
make  -j$(nproc) package
cpack

# Rename all deb packages generated to ditto specifications.
for f in *.deb
do
  package_name=`dpkg-deb -I $f | grep Package: | awk '{print $NF}'`
  new_filename="${package_name}_${VERSION_REVISION}_${ARCH}.deb"
  echo "New Debian Filename: $new_filename";
  mv $f $new_filename
done
