#!/usr/bin/env bash

# Script to download and install Doxygen

set -ex

version="Release_1_9_4" # Release tag on GitHub
prefix="${1:-$HOME/.local}"

[ -e "$prefix/bin/doxygen" ] \
 && exit 0

mkdir /tmp/doxygen-install
pushd /tmp/doxygen-install

# Download
[ -d "doxygen" ] \
 || git clone --single-branch --depth=1 --branch "$version" \
        https://github.com/doxygen/doxygen.git
pushd doxygen
# Configure
cmake -Bbuild -S. \
    -DCMAKE_INSTALL_PREFIX="$prefix" \
    -DCMAKE_BUILD_TYPE=Release
# Build
cmake --build build -j1
# Install
cmake --install build

popd
popd