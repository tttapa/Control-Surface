#!/usr/bin/env bash

cd "$( dirname "${BASH_SOURCE[0]}" )"/../..
set -ex

longshort=short
port="/dev/ttyACM0"

if [ "$1" = 'upload' ]; then
    make -C test/tools
    cmake --build --preset dev --config Release -t syx2usb
    ./build/dev/test/tools/Release/syx2usb \
        test/tools/data/MIDI-$longshort-challenge.syx \
        test/tools/sketches/midi-receive/MIDI-challenge.h
    arduino-cli compile --fqbn arduino:mbed_rp2040:pico test/tools/sketches/midi-echo
    arduino-cli upload -p $port --fqbn arduino:mbed_rp2040:pico test/tools/sketches/midi-echo
    sleep 2
fi

true
while [ $? = '0' ]; do
    time amidi -p hw:1,0,0 -r test/tools/data/MIDI-$longshort-response.syx -ca -t 0.5 \
                           -s test/tools/data/MIDI-$longshort-challenge.syx
    ./build/dev/test/tools/Release/syx2usb \
        test/tools/data/MIDI-$longshort-response.syx \
        test/tools/sketches/midi-echo/MIDI-response.h
    cmp test/tools/sketches/midi-echo/MIDI-challenge.h \
        test/tools/sketches/midi-echo/MIDI-response.h
done
