#!/usr/bin/env bash
cd "$( dirname "${BASH_SOURCE[0]}" )"/..

build_type="${1:-RelWithDebInfo}"
prefix="${2:-/usr/local}"

set -ex

pushd /tmp

# GoogleTest framework
rm -rf googletest
git clone --single-branch --depth=1 --branch main \
    https://github.com/google/googletest.git
pushd googletest
cmake -Bbuild -S. \
    -D CMAKE_INSTALL_PREFIX="${prefix}" \
    -D CMAKE_BUILD_TYPE="${build_type}"
cmake --build build -j
cmake --install build
popd

popd
