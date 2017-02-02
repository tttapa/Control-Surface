#!/bin/bash

cd "$(dirname "${BASH_SOURCE}")"

read -p "Are you sure you want to flash your ATmega16U2 with the HIDuino MIDI firmware? [Y/n]: " confirm
if [ "$confirm" = y ] || [ "$confirm" = Y ]
then 
if [ ! -e arduino_midi.hex ]
then
echo "Downloading firmware: arduino_midi.hex ..."
wget --quiet "https://raw.githubusercontent.com/ddiakopoulos/hiduino/master/compiled_firmwares/arduino_midi.hex"
fi
echo "Erasing ..."
sudo dfu-programmer atmega16u2 erase
echo "Flashing arduino_midi.hex ..."
sudo dfu-programmer atmega16u2 flash arduino_midi.hex
echo "Resetting ..."
sudo dfu-programmer atmega16u2 reset
echo "Done."
fi
