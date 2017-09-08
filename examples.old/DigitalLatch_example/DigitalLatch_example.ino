/*
This is an example of the "DigitalLatch" class of the MIDI_controller library.
Connect 4 switches to pin 2, 3, 4 and 5. Connect the other pin of the switch to the ground, 
a pull-up resistor is not necessary, since the internal ones will be used. These switches will be MIDI notes E0, F0, F#0 and G0.
Map these in your DAW or DJ software as mute of channels 1-4. (Or set control surface to Mackie Control Universal).

If you flip the switch to 'on', the appropriate channel will be muted, if you turn the switch 'off' again, the channel will be
un-muted.

The difference between the Digital and DigitalLatch classes, is that the Digital class is to be used with momentary pushbuttons, while DigitalLatch can be used with toggle switches.
When a momentary pushbutton (Digital) is pressed, a 'note on' event is sent. When it's released, a 'note off' event is sent.
When a toggle switch (DigitalLatch) is turned on, a 'note on' event is sent, then after a certain amount of milliseconds (defined in latchTime), a 'note off' event is sent.
The same thing happens when the toggle switch is turned off.

If you are using a Teensy, make sure you have the USB type set to MIDI.
If you are using an Arduino Uno or Mega, use the HIDUINO firmware for the ATmega16U2.


Written by tttapa, 21/08/2015
https://github.com/tttapa/MIDI_controller
*/

#include <MIDI_Controller.h> // include the library

const static byte velocity = 127; // the maximum velocity, since MIDI uses a 7-bit number for velocity.
const static int  latchTime = 100;  // the amount of time (in ms) the note is held on. Read documentation or see source code for more information.
const static byte E0 = 16; // note number 60 is defined as middle C in the MIDI implementation, so 16 is E0.

//________________________________________________________________________________________________________________________________

DigitalLatch switch1(2, E0,   1, velocity, latchTime); // Create a new instance of the class 'Digital', called 'button1', on pin 2, that sends MIDI messages with note 'C4' (60) on channel 1, with velocity 127).
DigitalLatch switch2(3, E0+1, 1, velocity, latchTime);
DigitalLatch switch3(4, E0+2, 1, velocity, latchTime);
DigitalLatch switch4(5, E0+3, 1, velocity, latchTime);

//________________________________________________________________________________________________________________________________

void setup(){
  USBMidiController.blink(LED_BUILTIN);  // flash the built-in LED (pin 13 on most boards) on every message
  USBMidiController.setDelay(15);  // wait 15 ms after each message not to flood the connection
  USBMidiController.begin();  // Initialise the USB MIDI connection
  delay(1000); // Wait a second...
}

//________________________________________________________________________________________________________________________________

void loop(){
  switch1.refresh(); // refresh the switch (check whether the input has changed since last time, if so, send it over MIDI)
  switch2.refresh();
  switch3.refresh();
  switch4.refresh();
}
