/*
This is an example of the "Analog" class of the MIDI_controller library.
Connect faders or potentiometers to all analog inputs.
Map these in your DAW or DJ software.

If you are using a Teensy, make sure you have the USB type set to MIDI.
If you are using an Arduino Uno or Mega, use the HIDUINO firmware for the ATmega16U2.


Written by tttapa, 29-06-2017
https://github.com/tttapa/MIDI_controller
*/

#include <MIDI_Controller.h> // include the library

const static size_t analogAverage = 8; // Use the average of 8 samples to get smooth transitions and prevent noise

//________________________________________________________________________________________________________________________________

Analog* potentiometers[NUM_ANALOG_INPUTS]; // create an empty array of pointers to Analog objects

//________________________________________________________________________________________________________________________________

void setup(){
  USBMidiController.blink(LED_BUILTIN);  // flash the built-in LED (pin 13 on most boards) on every message
  USBMidiController.setDelay(15);  // wait 15 ms after each message not to flood the connection
  USBMidiController.begin();  // Initialise the USB MIDI connection
  delay(1000); // Wait a second...
  for(int i = 0; i < NUM_ANALOG_INPUTS; i++) {
    potentiometers[i] = new Analog(A0+i, i, 1); // create a new Analog object and store it in the array
    potentiometers[i]->average(analogAverage); // Use the average of 8 samples to get smooth transitions and prevent noise
  }
}

//________________________________________________________________________________________________________________________________

void loop(){
  for(int i = 0; i < NUM_ANALOG_INPUTS; i++) { // refresh all Analog inputs
    potentiometers[i]->refresh();
  }
}