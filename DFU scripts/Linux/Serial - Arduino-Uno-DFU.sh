#!/bin/bash

cd "$(dirname "${BASH_SOURCE}")"

read -p "Are you sure you want to flash your ATmega16U2 with the Arduino Uno Serial firmware? [y/N]: " confirm
if [ "$confirm" = y ] || [ "$confirm" = Y ]; then
    if [ ! -e Arduino-usbserial-atmega16u2-Uno-Rev3.hex ]; then
        echo "Downloading firmware: Arduino-usbserial-atmega16u2-Uno-Rev3.hex ..."
        wget --quiet "https://raw.githubusercontent.com/arduino/ArduinoCore-avr/7a2e1cd815266fef3012a5c9b48f88d78551f838/firmwares/atmegaxxu2/arduino-usbserial/Arduino-usbserial-atmega16u2-Uno-Rev3.hex"
    fi
    echo "Erasing ..."
    sudo dfu-programmer atmega16u2 erase
    echo "Flashing Arduino-usbserial-atmega16u2-Uno-Rev3.hex ..."
    sudo dfu-programmer atmega16u2 flash Arduino-usbserial-atmega16u2-Uno-Rev3.hex
    echo "Resetting ..."
    sudo dfu-programmer atmega16u2 reset
    echo "Done."
fi
