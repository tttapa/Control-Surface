/*
This is an example of the bank function of the "Digital" class of the MIDI_controller library. 
Connect 2 switches (toggle switch or similar, but not a momentary pushbutton, that's what the Digital class is for) to pins 2 and 3. Connect the other pin of the switch to the ground, 
a pull-up resistor is not necessary, since the internal ones will be used. These switches will be MIDI notes E0, F0, F#0 and G0.
Map these in your DAW or DJ software as mute of channels 1-4. (Or set control surface to Mackie Control Universal).
Connect a switch (toggle switch or similar) to digital pin 12.
Connect the other pin of the switch to the ground, a pull-up resistor is not necessary, since the 
internal ones will be used.

When the switch (on pin 12) is in the 'off' position, button1 will be the E0, and button2 will be F0.
When the switch (on pin 12) is in the 'on'  position, button1 will be the F#0, and button2 will be G0.
This allows you to create 'banks' of switches, and switch between them.
The number of switches is only limited by the number of inputs.
The number of banks is limited to 2, but the code in DigitalLatch.h and DigitalLatch.cpp can easily be modified if you need more.

The difference between the Digital and DigitalLatch classes, is that the Digital class is to be used with momentary pushbuttons, while DigitalLatch can be used with toggle switches.
When a momentary pushbutton (Digital) is pressed, a 'note on' event is sent. When it's released, a 'note off' event is sent.
When a toggle switch (DigitalLatch) is turned on, a 'note on' event is sent, then after a certain amount of milliseconds (defined in latchTime), a 'note off' event is sent.
The same thing happens when the toggle switch is turned off.

If you are using a Teensy, make sure you have the USB type set to MIDI.
If you are using an Arduino Uno or Mega, use the HIDUINO firmware for the ATmega16U2.


Written by tttapa, 21/08/2015
https://github.com/tttapa/MIDI_controller
*/

#include <MIDI_controller.h> // include the library

const static byte velocity = 127; // the maximum velocity, since MIDI uses a 7-bit number for velocity.
const static byte E0 = 16; // note number 60 is defined as middle C in the MIDI implementation, so 16 is E0.
const static int latchTime = 100; // How long a note will be held on, in DigitalLatch mode.

const static byte switchPin = 12; // the switch is connected to pin 12.

//________________________________________________________________________________________________________________________________

DigitalLatch switch1(2, E0  , 1, velocity, latchTime); // Create a new instance of the class 'Digital', called 'switch1', on pin 2, that sends MIDI messages with note 'E0' (16) on channel 1, with velocity 127), for a duration of 100ms.
DigitalLatch switch2(3, E0+1, 1, velocity, latchTime);

//________________________________________________________________________________________________________________________________

void setup(){
  USBMidiController.blink(13);  // flash the LED on pin 13 on every message
  USBMidiController.setDelay(15);  // wait 15 ms after each message not to flood the connection
  delay(1000); // Wait a second...
  switch1.bank(switchPin, E0+2, 1); // Enable the bank functionality. If switchPin is low (=switch on) the message will be note F#0 instead of E0.
  switch2.bank(switchPin, E0+3, 1);
}

//________________________________________________________________________________________________________________________________

void loop(){
  switch1.refresh(); // refresh the switch (check whether the input has changed since last time, if so, send it over MIDI)
  switch2.refresh();
}
