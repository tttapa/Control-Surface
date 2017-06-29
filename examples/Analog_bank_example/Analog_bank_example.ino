/*
This is an example of the bank function of the "Analog" class of the MIDI_controller library. 
Connect 2 faders or potentiometers to A0 and A1. These will be MIDI channel volumes of channel 1-4.
Map these in your DAW or DJ software. Connect a switch (toggle switch or similar) to digital pin 12.
Connect the other pin of the switch to the ground, a pull-up resistor is not necessary, since the 
internal ones will be used.

When the switch is in the 'off' position, fader1 will be the volume of channel 1, and fader2 will be the volume of channel 2.
When the switch is in the 'on'  position, fader1 will be the volume of channel 3, and fader2 will be the volume of channel 4.
This allows you to create 'banks' of faders, and switch between them.
The number of faders is only limited by the number of inputs.
The number of banks is limited to 2, but the code in Analog.h and Analog.cpp can easily be modified if you need more.

If you are using a Teensy, make sure you have the USB type set to MIDI.
If you are using an Arduino Uno or Mega, use the HIDUINO firmware for the ATmega16U2.


Written by tttapa, 21/08/2015
https://github.com/tttapa/MIDI_controller
*/

#include <MIDI_controller.h> // include the library

const static byte Channel_Volume = 0x7; // controller number 7 is defined as Channel Volume in the MIDI implementation.
const static size_t analogAverage = 8; // Use the average of 8 samples to get smooth transitions and prevent noise
const static byte switchPin = 12; // the switch is connected to pin 12.

//________________________________________________________________________________________________________________________________

Analog fader1(A0, Channel_Volume, 1); // Create a new instance of the class 'Analog, called 'fader1', on pin A0, that sends MIDI messages with controller 7 (channel volume) on channel 1. 
Analog fader2(A1, Channel_Volume, 2);

//________________________________________________________________________________________________________________________________

void setup(){
  USBMidiController.blink(LED_BUILTIN);  // flash the built-in LED (pin 13 on most boards) on every message
  USBMidiController.setDelay(15);  // wait 15 ms after each message not to flood the connection
  delay(1000); // Wait a second...
  fader1.bank(switchPin, Channel_Volume, 3); // Enable the bank functionality. If switchPin is low (=switch on) the message will be sent on channel 3 instead of channel 1
  fader2.bank(switchPin, Channel_Volume, 4);
  fader1.average(analogAverage); // Use the average of 8 samples to get smooth transitions and prevent noise
  fader2.average(analogAverage);
}

//________________________________________________________________________________________________________________________________

void loop(){
  fader1.refresh(); // refresh the fader (check whether the input has changed since last time, if so, send it over MIDI)
  fader2.refresh();
}
