/*
This is an example of the "DigitalLatch" class of the MIDI_controller library.
Connect 4 switches to pin 2, 3, 4 and 5. Connect the other pin of the switch to the ground, 
a pull-up resistor is not necessary, since the internal ones will be used. These switches will be MIDI notes E0, F0, F#0 and G0.
Map these in your DAW or DJ software as mute of channels 1-4. (Or set control surface to Mackie Control Universal).

If you flip the switch to 'on', the appropriate channel will be muted, if you turn the switch 'off' again, the channel will be
un-muted.

If you are using an Arduino Leonardo, make sure you have Teensyduino and TeeOnArdu installed, 
that you are using the Arduino IDE version 1.0.6, and board type is set to TeeOnArdu in MIDI mode.
If you are using an Arduino Uno, use the HIDUINO firmware for the ATmega16U2.

This library and these examples are part of the Instructable

Written by tttapa, 28/08/2015
*/

#include <MIDI_controller.h> // include the library

const static byte velocity = 127; // the maximum velocity, since MIDI uses a 7-bit number for velocity.
const static int  latchTime = 100;  // the amount of time (in ms) the note is held on. Read documentation or see source code for more information.
const static byte E0 = 16; // note number 60 is defined as middle C in the MIDI implementation, so 16 is E0.

//________________________________________________________________________________________________________________________________

DigitalLatch switch1(2, E0,   1, velocity, latchTime); // Create a new member of the class 'Digital', called 'button1', on pin 2, that sends MIDI messages with note 'C4' (60) on channel 1, with velocity 127).
DigitalLatch switch2(3, E0+1, 1, velocity, latchTime);
DigitalLatch switch3(4, E0+2, 1, velocity, latchTime);
DigitalLatch switch4(5, E0+3, 1, velocity, latchTime);

//________________________________________________________________________________________________________________________________

void setup(){
  setupMidi(13, 10); // Setup the MIDI communication, with an LED on pin 13, and a delay of 10ms after every message.
  delay(1000); // Wait a second...
}

//________________________________________________________________________________________________________________________________

void loop(){
  switch1.refresh(); // refresh the switch (check whether the input has changed since last time, if so, send it over MIDI)
  switch2.refresh();
  switch3.refresh();
  switch4.refresh();
}
