#!/usr/bin/env bash

# Script to download and install GoogleTest

set -ex

version="${1:-master}" # Release tag on GitHub
prefix="${2:-$HOME/.local}"
buildtype="${3:-Debug}"

# Create temporary directory
tmp_dir=`mktemp -d`
if [[ ! "$tmp_dir" || ! -d "$tmp_dir" ]]; then
  echo "Could not create temporary directory"
  exit 1
fi
function cleanup {
  rm -rf "$tmp_dir"
}
trap cleanup EXIT

pushd "$tmp_dir"
# Download
git clone --single-branch --depth=1 --branch "$version" \
        https://github.com/google/googletest.git
# Create build folder
mkdir -p googletest/build/$buildtype && pushd "$_"
# Configure
cmake ../.. -DCMAKE_INSTALL_PREFIX="$prefix" -DCMAKE_BUILD_TYPE=$buildtype
# Build
make -j$(nproc)
# Install
make install
popd
popd
