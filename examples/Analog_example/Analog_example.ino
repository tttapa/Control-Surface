/*
This is an example of the "Analog" class of the MIDI_controller library.
Connect 4 faders or potentiometers to A0-A3. These will be MIDI channel volumes of channels 1-4.
Map these in your DAW or DJ software.

If you are using a Teensy, make sure you have the USB type set to MIDI.
If you are using an Arduino Uno or Mega, use the HIDUINO firmware for the ATmega16U2.


Written by tttapa, 21/08/2015
https://github.com/tttapa/MIDI_controller
*/

#include <MIDI_controller.h> // include the library

const static byte Channel_Volume = 0x7; // controller number 7 is defined as Channel Volume in the MIDI implementation.
const static size_t analogAverage = 8; // Use the average of 8 samples to get smooth transitions and prevent noise
//________________________________________________________________________________________________________________________________

Analog fader1(A0, Channel_Volume, 1); // Create a new instance of the class 'Analog, called 'fader1', on pin A0, that sends MIDI messages with controller 7 (channel volume) on channel 1.
Analog fader2(A1, Channel_Volume, 2);
Analog fader3(A2, Channel_Volume, 3);
Analog fader4(A3, Channel_Volume, 4);

//________________________________________________________________________________________________________________________________

void setup(){
  USBMidiController.blink(LED_BUILTIN);  // flash the built-in LED (pin 13 on most boards) on every message
  USBMidiController.setDelay(15);  // wait 15 ms after each message not to flood the connection
  USBMidiController.begin();  // Initialise the USB MIDI connection
  delay(1000); // Wait a second...
  fader1.average(analogAverage); // Use the average of 8 samples to get smooth transitions and prevent noise
  fader2.average(analogAverage);
  fader3.average(analogAverage);
  fader4.average(analogAverage);
}

//________________________________________________________________________________________________________________________________

void loop(){
  fader1.refresh(); // refresh the fader (check whether the input has changed since last time, if so, send it over MIDI)
  fader2.refresh();
  fader3.refresh();
  fader4.refresh();
}