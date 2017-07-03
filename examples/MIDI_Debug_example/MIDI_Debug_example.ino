/*
This is an example that demonstrates the debug functionality of the library.
Connect some buttons to pins 2-9, upload the sketch, and open the Serial Monitor.

When you press the first button, you should see:
90	0	0	7F
80	0	0	7F

If you are using a Teensy, make sure you have the USB type set to MIDI.


Written by tttapa, 03-7-2017
https://github.com/tttapa/MIDI_controller
*/

#include <MIDI_controller.h> // include the library

#define DEBUG true
// #define DEBUG false

const uint8_t digitalPins[] = { 2,  3,  4,  5,            // change these pins to the actual pins
                                6,  7,  8,  9             // you have buttons connected to
                              };

const static byte velocity = 127; // the maximum velocity, since MIDI uses a 7-bit number for velocity.
const static byte channel = 1   ; // the MIDI channel to use

//________________________________________________________________________________________________________________________________

Digital* buttons[sizeof(digitalPins)]; // create an empty array of pointers to Digital objects

//________________________________________________________________________________________________________________________________

void setup(){
  USBMidiController.blink(LED_BUILTIN);  // flash the built-in LED (pin 13 on most boards) on every message
  USBMidiController.setDelay(5);  // wait 5 ms after each message not to flood the connection
  USBMidiController.begin(115200, DEBUG);  // Initialise the Serial connection @115200 baud

  delay(1000); // Wait a second...
  for(unsigned int i = 0; i < sizeof(digitalPins); i++) {
    buttons[i] = new Digital(digitalPins[i], i, channel, velocity); // create a new Digital object and store it in the array
  }
}

//________________________________________________________________________________________________________________________________

void loop(){
  for(unsigned int i = 0; i < sizeof(digitalPins); i++) { // refresh all Digital inputs
    buttons[i]->refresh();
  }
}
