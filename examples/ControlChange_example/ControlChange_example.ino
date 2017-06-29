/*
This is an example of the "ControlChange" class of the MIDI_controller library.
Connect a fader or a potentiometer to A0. This will be the channel volume of MIDI channel 1.
Map this in your DAW or DJ software.

The ControlChange class allows more control over the actual value that is sent, 
for example, you use inputs other than the analog inputs, like digital sensors,
inputs from multiplexers, etc.
You can also map these values if you need to.

If you are using a Teensy, make sure you have the USB type set to MIDI.
If you are using an Arduino Uno or Mega, use the HIDUINO firmware for the ATmega16U2.


Written by tttapa, 21/08/2015
https://github.com/tttapa/MIDI_controller
*/

#include <MIDI_controller.h> // include the library

const static byte Channel_Volume = 0x7; // controller number 7 is defined as Channel Volume in the MIDI implementation.
const static size_t analogAverage = 8; // Use the average of 8 samples to get smooth transitions and prevent noise
//________________________________________________________________________________________________________________________________

ControlChange fader(Channel_Volume, 1); // Create a new instance of the class 'ControlChange', called 'fader', that sends MIDI messages with controller 7 (channel volume) on channel 1.

//________________________________________________________________________________________________________________________________

void setup(){
  USBMidiController.blink(LED_BUILTIN);  // flash the built-in LED (pin 13 on most boards) on every message
  USBMidiController.setDelay(15);  // wait 15 ms after each message not to flood the connection
  delay(1000); // Wait a second...
  fader.average(analogAverage); // Use the average of 8 samples to get smooth transitions and prevent noise
}

//________________________________________________________________________________________________________________________________

void loop(){
  uint8_t value1 = analogRead(A0)>>3; // map the analog input from a number between 0 and 1023 to a number between 0 and 127
  fader.refresh(value1); // refresh the fader: update with the new value, and send a new MIDI message if it changed
  float value2 = analogRead(A0)/1023.0;
  fader.refresh(value2); // you can also use floats
}