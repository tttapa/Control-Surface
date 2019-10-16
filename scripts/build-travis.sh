#!/usr/bin/env bash

export PATH="$HOME/.local/bin:$PATH"
cd "$TRAVIS_BUILD_DIR/build"
cmake ..
make -j4
make test