/*
  This is an example of the "RotaryEncoder" class of the MIDI_controller library.
  Connect the A and B pins of the encoder to 2 interrupt pins (2 and 3). It's recommended to use 100nF capacitors between the A and B pins and ground.
  Connect the common (C) pin to the ground. Pull-up resistors are not necessary, since the internal ones will be used.
  Map the control change message 0x14 to the right control in your DJ or DAW software, and select 'relative' instead of 'absolute'.

  Connect a switch (toggle switch or similar) to digital pin 12.
  Connect the other pin of the switch to the ground, a pull-up resistor is not necessary, since the 
  internal one will be used.
  When the switch is in the 'off' position, the encoder will send Control Change events on MIDI channel 1.
  When the switch is in the 'on'  position, the encoder will send Control Change events on MIDI channel 2.
  This allows you to create 'banks' of encoders, and switch between them.

  If you are using a jog wheel, use JOG, if you are using a normal encoder (for normal controls like EQ or volume etc.) use NORMAL_ENCODER.
  If you have strange results in your software, try another relative mode: TWOS_COMPLEMENT, BINARY_OFFSET or SIGN_MAGNITUDE.
  If the control works, but it goes in the wrong direction, swap the pins A and B of the encoder (physically, or when creating the
  RotaryEncoder member).

  If you are using a Teensy, make sure you have the USB type set to MIDI.
  If you are using an Arduino Uno or Mega, use the HIDUINO firmware for the ATmega16U2.


  Written by tttapa, 16/08/2017
  https://github.com/tttapa/MIDI_controller
*/

#include <MIDI_Controller.h>

const byte Channel = 1;
const byte Controller = 0x14;
const int speedMultiply = 1; // no change in speed of the encoder
const byte switchPin = 12; // The bank select switch is connected to pin 12.

//________________________________________________________________________________________________________________________________

RotaryEncoder enc(2, 3, Controller, Channel, speedMultiply, JOG, TWOS_COMPLEMENT); // Create a new instance of the class 'RotaryEncoder', called 'enc', on pin 2 and 3, controller number 0x14, on channel1, no change in speed (speed is multiplied by 1), it's used as a Jog wheel, and the sign mode is set to two's complement.

//________________________________________________________________________________________________________________________________

void setup() {
  enc.bank(switchPin, Controller, Channel + 1); // Enable the bank functionality. If switchPin is low (= switch on) the message will be sent on channel 2 instead of channel 1
  
  USBMidiController.blink(LED_BUILTIN);  // flash the built-in LED (pin 13 on most boards) on every message
  USBMidiController.setDelay(15);  // wait 15 ms after each message not to flood the connection
  USBMidiController.begin();  // Initialise the USB MIDI connection
  delay(1000); // Wait a second...
}

//________________________________________________________________________________________________________________________________

void loop() {
  enc.refresh();
}
