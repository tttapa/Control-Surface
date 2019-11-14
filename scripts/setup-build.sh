#!/usr/bin/env bash

set -ex 

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <project-dir>"
    exit 0
fi

cd "$1"

rm -rf build/*
mkdir -p build && cd $_ 

cmake .. \
    -DCMAKE_BUILD_TYPE=Debug \
    -DFAST_COMPILE=OFF

mkdir coverage && cd $_

CC=clang CXX=clang++ \
cmake ../.. \
    -DCMAKE_BUILD_TYPE=Coverage \
    -DFAST_COMPILE=ON