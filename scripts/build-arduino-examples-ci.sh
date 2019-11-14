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
    --default-board 'AVR USB' \
    --directory ../examples \
    --parallel $p --jobs $j \
    --board-options-file ../test/examples-board-options-ci.txt \
    # --verbose # --print-successful
