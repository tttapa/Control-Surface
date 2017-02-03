/*
This is an example of the "Digital" class of the MIDI_controller library.
Connect 4 pushbuttons to pin 2, 3, 4 and 5. Connect the other pin of the button to the ground, 
a pull-up resistor is not necessary, since the internal ones will be used. These buttons will be MIDI notes C4, C#4, D4, D#4.
Map these in your DAW or DJ software.

If you are using a Teensy, make sure you have the USB type set to MIDI.
If you are using an Arduino Uno or Mega, use the HIDUINO firmware for the ATmega16U2.


Written by tttapa, 21/08/2015
https://github.com/tttapa/MIDI_controller
*/

#include <MIDI_controller.h> // include the library

const static byte velocity = 127; // the maximum velocity, since MIDI uses a 7-bit number for velocity.
const static byte C4 = 60; // note number 60 is defined as middle C in the MIDI implementation.

//________________________________________________________________________________________________________________________________

Digital button1(2, C4, 1, velocity); // Create a new member of the class 'Digital', called 'button1', on pin 2, that sends MIDI messages with note 'C4' (60) on channel 1, with velocity 127).
Digital button2(3, C4+1, 1, velocity); // C4 + 1 = C#4
Digital button3(4, C4+2, 1, velocity); // C4 + 2 = D4
Digital button4(5, C4+3, 1, velocity); // C4 + 3 = D#4

//________________________________________________________________________________________________________________________________

void setup(){
  setupMidi(13, 10); // Setup the MIDI communication, with an LED on pin 13, and a delay of 10ms after every message.
  delay(1000); // Wait a second...
}

//________________________________________________________________________________________________________________________________

void loop(){
  button1.refresh(); // refresh the button (check whether the input has changed since last time, if so, send it over MIDI)
  button2.refresh();
  button3.refresh();
  button4.refresh();
}
