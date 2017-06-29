/*
This is an example of the "AnalogHiRes" class of the MIDI_controller library.
Connect a fader or potentiometer to analog pin A0. This will be the MIDI Pitch Bend on channel 1.
Map it in your DAW or DJ software.

A normal "Analog" input has 7 bits of accuracy, an "AnalogHiRes" input has up to 14 bits of accuracy. 
(14 bits on Teensy 3, 12 bits on Arduino Due, 10 bits on a normal AVR board) 

If you are using a Teensy, make sure you have the USB type set to MIDI.
If you are using an Arduino Uno or Mega, use the HIDUINO firmware for the ATmega16U2.


Written by tttapa, 29-06-2017
https://github.com/tttapa/MIDI_controller
*/

#include <MIDI_controller.h> // include the library

const static size_t analogAverage = 8; // Use the average of 8 samples to get smooth transitions and prevent noise
//________________________________________________________________________________________________________________________________

AnalogHiRes fader(A0, 1); // Create a new instance of the class 'Analog, called 'fader', on pin A0, that sends MIDI messages with controller 7 (channel volume) on channel 1.

//________________________________________________________________________________________________________________________________

void setup(){
  USBMidiController.blink(LED_BUILTIN);  // flash the built-in LED (pin 13 on most boards) on every message
  USBMidiController.setDelay(15);  // wait 15 ms after each message not to flood the connection
  delay(1000); // Wait a second...
  fader.average(analogAverage); // Use the average of 8 samples to get smooth transitions and prevent noise
  // fader.map([](int value){return (int)((unsigned long)value*value/16383);}); // Map to a quadratic curve
}

//________________________________________________________________________________________________________________________________

void loop(){
  fader.refresh(); // refresh the fader (check whether the input has changed since last time, if so, send it over MIDI)
}