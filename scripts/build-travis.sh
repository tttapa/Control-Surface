#!/usr/bin/env bash

export PATH="$HOME/.local/bin:$PATH"
cd "$TRAVIS_BUILD_DIR/build"
cmake ..
make -j4
make test
make arduino

ls /tmp/arduino-example-builder/f5219009433eda58268e1e0872d96091/f5219009433eda58268e1e0872d96091.ino