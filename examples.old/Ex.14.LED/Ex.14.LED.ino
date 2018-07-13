/**
 * @example Ex.14.LED.ino
 * 
 * This is an example of the MIDI_LED class of the Control_Surface librar.
 * 
 * Connect two push buttons to pins 2 and 3. Pull-up resistors are not necessary, 
 * because the internal ones will be used.  
 * Connect two LEDs (+ current limiting resistors) to pins 12 and 13.
 * 
 * The buttons will mute and un-mute tracks 1 and 2 respectively. The LEDs will 
 * indicate whether the channel is muted or not.
 * 
 * Configure as a MCU-compatible Control Surface (Mackie Control Universal) in your DAW.
 * 
 * Written by Pieter P, 2018-04-11  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h>  // Include the library

const uint8_t velocity = 0b1111111;

Digital button1(2, MCU::MUTE_1, 1, velocity);
Digital button2(3, MCU::MUTE_2, 1, velocity);

MIDI_LED led1(12, MCU::MUTE_1, 1); // LED on pin 12, note MCU::MUTE_1, channel 1
MIDI_LED led2(13, MCU::MUTE_2, 1);

void setup() {}

void loop() {
  Control_Surface.refresh();
}
