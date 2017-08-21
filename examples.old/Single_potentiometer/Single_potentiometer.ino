/*
This is an example of the "Analog" class of the MIDI_controller library.
Connect a faders or potentiometer to analog pin A0. This will be the MIDI channel volume of channel 1.
Map it in your DAW or DJ software.

If you are using a Teensy, make sure you have the USB type set to MIDI.
If you are using an Arduino Uno or Mega, use the HIDUINO firmware for the ATmega16U2.


Written by tttapa, 21/08/2015
https://github.com/tttapa/MIDI_controller
*/

#include <MIDI_controller.h> // include the library

const static byte Channel_Volume = 0x7; // controller number 7 is defined as Channel Volume in the MIDI implementation.
const static size_t analogAverage = 8; // Use the average of 8 samples to get smooth transitions and prevent noise
//________________________________________________________________________________________________________________________________

Analog fader(A0, Channel_Volume, 1); // Create a new instance of the class 'Analog, called 'fader', on pin A0, that sends MIDI messages with controller 7 (channel volume) on channel 1.

//________________________________________________________________________________________________________________________________

void setup(){
  USBMidiController.blink(LED_BUILTIN);  // flash the built-in LED (pin 13 on most boards) on every message
  USBMidiController.setDelay(15);  // wait 15 ms after each message not to flood the connection
  USBMidiController.begin();  // Initialise the USB MIDI connection
  delay(1000); // Wait a second...
  fader.average(analogAverage); // Use the average of 8 samples to get smooth transitions and prevent noise
}

//________________________________________________________________________________________________________________________________

void loop(){
  fader.refresh(); // refresh the fader (check whether the input has changed since last time, if so, send it over MIDI)
}