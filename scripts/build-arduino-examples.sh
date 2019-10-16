#!/usr/bin/env bash
cd "$(dirname "$0")"

if [ $# -ge 2 ]; then
    p=$1
    j=$2
elif [ $# -eq 1 ]; then
    p=$1
    j=2
else
    p=$(nproc)
    j=2
fi

../tools/arduino-example-builder \
    --default-board 'Teensy 3.x' \
    --directory ../examples \
    --parallel $p --jobs $j \
    --board-options-file ../test/examples-board-options.txt \
    # --verbose # --print-successful
