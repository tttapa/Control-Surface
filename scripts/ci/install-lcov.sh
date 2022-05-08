#!/usr/bin/env bash

# Script to download and install LCOV

set -ex

version="v1.15" # Release tag on GitHub
prefix="${1:-$HOME/.local}"

[ -e "$prefix/bin/lcov" ] \
 && exit 0

mkdir /tmp/lcov-install 
pushd /tmp/lcov-install 

# Download
[ -d "lcov" ] \
 || git clone --single-branch --depth=1 --branch "$version" \
        https://github.com/linux-test-project/lcov.git
pushd lcov
make install PREFIX="$prefix"

popd
popd
