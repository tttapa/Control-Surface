/*
This is an example of the "RotaryEncoder" class of the MIDI_controller library. 
Connect the A & B pins of the encoder to 2 interrupt pins (2&3). It's recommended to use 100nF capacitors between the A & B pins and ground.
Connect the common C pin to the ground. Pull-up resistors are not ncessary, since the internal ones will be used.
Map the control change message 0x14 to the right control in your software.

If you are using a jog wheel use JOG, if you are using a normal encoder, for normal controls, like scrolling etc. use NORMAL_ENCODER.
If you have strange results in your software, try another mode: ADD_64, SIGN_BIT or POS1_NEG127.
If the control works, but it goes in the wrong direction, swap the pins A&B of the encoder (physically, or when creating the 
RotaryEncoder member).

If you are using an Arduino Leonardo, make sure you have Teensyduino and TeeOnArdu installed, 
that you are using the Arduino IDE version 1.0.6, and board type is set to TeeOnArdu in MIDI mode.
If you are using an Arduino Uno, use the HIDUINO firmware for the ATmega16U2.

This library and these examples are part of the Instructable

Written by tttapa, 28/08/2015
*/

#include <MIDI_controller.h>

const static byte Channel = 1;
const static byte Controller = 0x14;

//________________________________________________________________________________________________________________________________

RotaryEncoder enc(2,3,Controller,Channel,1,JOG,POS1_NEG127); // Create a new member of the class 'RotaryEncoder', called 'enc', on pin 2 and 3, controller number 0x14, on channel1, no change in speed (speed is multiplied by 1), it's used as a Jog wheel, and the mode is set to POS1_NEG127.

//________________________________________________________________________________________________________________________________

void setup(){
  setupMidi(13, 10); // Setup the MIDI communication, with an LED on pin 13, and a delay of 10ms after every message.
  delay(1000); // Wait a second...
}

//________________________________________________________________________________________________________________________________

void loop(){
  enc.refresh();
}
