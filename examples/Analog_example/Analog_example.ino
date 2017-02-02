/*
This is an example of the "Analog" class of the MIDI_controller library.
Connect 4 faders or potentiometers to A0-A3. These will be MIDI channel volumes of channel 1-4.
Map these in your DAW or DJ software.

If you are using an Arduino Leonardo, make sure you have Teensyduino and TeeOnArdu installed, 
that you are using the Arduino IDE version 1.0.6, and board type is set to TeeOnArdu in MIDI mode.
If you are using an Arduino Uno, use the HIDUINO firmware for the ATmega16U2.

This library and these examples are part of the Instructable

Written by tttapa, 21/08/2015
*/

#include <MIDI_controller.h> // include the library

const static byte resolution = 128; // the maximum analog resolution that can be sent over MIDI(7-bit, 0-127)
const static byte Channel_Volume = 0x7; // controller number 7 is defined as Channel Volume in the MIDI implementation.

//________________________________________________________________________________________________________________________________

Analog fader1(A0, Channel_Volume, 1, resolution); // Create a new member of the class 'Analog, called 'fader1', on pin A0, that sends MIDI messages with controller 7 (channel volume) on channel 1. The resolution of the analog input is 128 (7-bit). 
Analog fader2(A1, Channel_Volume, 2, resolution);
Analog fader3(A2, Channel_Volume, 3, resolution);
Analog fader4(A3, Channel_Volume, 4, resolution);

//________________________________________________________________________________________________________________________________

void setup(){
  setupMidi(13, 10); // Setup the MIDI communication, with an LED on pin 13, and a delay of 10ms after every message.
  delay(1000); // Wait a second...
}

//________________________________________________________________________________________________________________________________

void loop(){
  fader1.refresh(); // refresh the fader (check whether the input has changed since last time, if so, send it over MIDI)
  fader2.refresh();
  fader3.refresh();
  fader4.refresh();
}
