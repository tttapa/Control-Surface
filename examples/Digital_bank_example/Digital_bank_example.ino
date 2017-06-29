/*
This is an example of the bank function of the "Digital" class of the MIDI_controller library. 
Connect 2 pushbuttons to pin 2 and 3. Connect the other pin of the button to the ground, 
a pull-up resistor is not necessary, since the internal ones will be used. These buttons will be MIDI notes C4, C#4, D4, D#4.
Map these in your DAW or DJ software. 
Connect a switch (toggle switch or similar) to digital pin 12.
Connect the other pin of the switch to the ground, a pull-up resistor is not necessary, since the 
internal ones will be used.

When the switch is in the 'off' position, button1 will be the C4, and button2 will be C#4.
When the switch is in the 'on'  position, button1 will be the D4, and button2 will be D#4.
This allows you to create 'banks' of buttons, and switch between them.
The number of buttons is only limited by the number of inputs.
The number of banks is limited to 2, but the code in Digital.h and Digital.cpp can easily be modified if you need more.

If you are using a Teensy, make sure you have the USB type set to MIDI.
If you are using an Arduino Uno or Mega, use the HIDUINO firmware for the ATmega16U2.


Written by tttapa, 21/08/2015
https://github.com/tttapa/MIDI_controller
*/

#include <MIDI_controller.h> // include the library

const static byte velocity = 127; // the maximum velocity, since MIDI uses a 7-bit number for velocity.
const static byte C4 = 60; // note number 60 is defined as middle C in the MIDI implementation.

const static byte switchPin = 12; // the switch is connected to pin 12.

//________________________________________________________________________________________________________________________________

Digital button1(2, C4, 1, velocity); // Create a new instance of the class 'Digital', called 'button1', on pin 2, that sends MIDI messages with note 'C4' (60) on channel 1, with velocity 127).
Digital button2(3, C4+1, 1, velocity); // C4 + 1 = C#4

//________________________________________________________________________________________________________________________________

void setup(){
  USBMidiController.blink(LED_BUILTIN);  // flash the built-in LED (pin 13 on most boards) on every message
  USBMidiController.setDelay(15);  // wait 15 ms after each message not to flood the connection
  delay(1000); // Wait a second...
  button1.bank(switchPin, C4+2, 1); // Enable the bank functionality. If switchPin is low (=switch on) the message will be note D4 instead of C4 (still on channel 1)
  button2.bank(switchPin, C4+3, 1); // C4 + 3 = D#4
}

//________________________________________________________________________________________________________________________________

void loop(){
  button1.refresh(); // refresh the button (check whether the input has changed since last time, if so, send it over MIDI)
  button2.refresh();
}
