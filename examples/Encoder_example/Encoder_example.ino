/*
This is an example of the "RotaryEncoder" class of the MIDI_controller library. 
Connect the A and B pins of the encoder to 2 interrupt pins (2 and 3). It's recommended to use 100nF capacitors between the A and B pins and ground.
Connect the common (C) pin to the ground. Pull-up resistors are not necessary, since the internal ones will be used.
Map the control change message 0x14 to the right control in your DJ or DAW software, and select 'relative' instead of 'absolute'.

If you are using a jog wheel, use JOG, if you are using a normal encoder (for normal controls like EQ or volume etc.) use NORMAL_ENCODER.
If you have strange results in your software, try another relative mode: ADD_64, SIGN_BIT or POS1_NEG127.
If the control works, but it goes in the wrong direction, swap the pins A and B of the encoder (physically, or when creating the 
RotaryEncoder member).

If you are using a Teensy, make sure you have the USB type set to MIDI.
If you are using an Arduino Uno or Mega, use the HIDUINO firmware for the ATmega16U2.


Written by tttapa, 21/08/2015
https://github.com/tttapa/MIDI_controller
*/

#include <MIDI_controller.h>

const static byte Channel = 1;
const static byte Controller = 0x14;

//________________________________________________________________________________________________________________________________

RotaryEncoder enc(2,3,Controller,Channel,1,JOG,POS1_NEG127); // Create a new instance of the class 'RotaryEncoder', called 'enc', on pin 2 and 3, controller number 0x14, on channel1, no change in speed (speed is multiplied by 1), it's used as a Jog wheel, and the mode is set to POS1_NEG127.

//________________________________________________________________________________________________________________________________

void setup(){
  USBMidiController.blink(13);  // flash the LED on pin 13 on every message
  USBMidiController.setDelay(15);  // wait 15 ms after each message not to flood the connection
  delay(1000); // Wait a second...
}

//________________________________________________________________________________________________________________________________

void loop(){
  enc.refresh();
}
