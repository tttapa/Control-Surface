/*
  This is the code I used for my specific MIDI controller.
  I used an Arduino Leonardo with 4 faders on A0-A3, 8 potentiometers on A4-A11, a rotary encoder on pin 0&1, 4 toggle switches on pins 2, 3, 5 and 7, a toggle switch (for bank select) on pin 11, and an LED on pin 13.

  When bank select is 0, faders 1-4 are channel volumes 1-4, potSide 1-4 are pan/balance of channel 1-4, switches 1-4 are mute channel 5-8.
  When bank select is 1, faders 1-4 are channel volumes 5-8, potSide 1-4 are pan/balance of channel 5-8, switches 1-4 are mute channel 1-4.
  PotTop 1-4 are not in the bank, because I use them as effect or EQ parameters, and they are channel independent.
  Enc1 is used for scrolling.

  Note that I used a custom control mapping in my DAW.

  If you are using a Teensy, make sure you have the USB type set to MIDI.
  If you are using an Arduino Uno or Mega, use the HIDUINO firmware for the ATmega16U2.


  Written by tttapa, 21/08/2015
  https://github.com/tttapa/MIDI_controller
*/


#include <MIDI_controller.h>

#define VELOCITY          0b01111111  // The velocity of the buttons (0b01111111 = 127 = 100%)
#define LATCHTIME         100         // How long a note will be held on, in DigitalLatch mode (in milliseconds).

#define SPEED_MULTIPLY    1           // If the jog wheels or other encoders are too slow in your software, increase this value 
// (it will be multiplied with the actual speed of the encoder, as the name implies.) Default is 1.
#define PULSES_PER_STEP   4           // This is the number of pulses the encoder outputs when you turn it one step (or click) further. Use 4 for a normal rotary encoder, and 1 for a jogwheel.
// If you set it to 1, this uses the maximum resolution. If it is set to 4, one message will be sent per click of the encoder. 1 click matches 1 unit in the software. This is more logical for most usages (except jogwheels).

/* Resolution of the analog values. Min = 2, Max = 128, Default = 128. 2 gives either 0 or 127, 128 gives a value between 0 and 127 (= 128 possible values).
  Limited to 128 due to the fact that it's converted to a 7-bit number in the MIDI message.
  Increase to enhance precision. Decrease to (try to) remove noise. */
#define RESOLUTION     64

//_____________________________________________________________________________________________________________________________________________________________________________________________

Analog fader1(A0, 0x10, 1 , RESOLUTION);                 // Create a new instance of class 'Analog' called 'fader1', on pin A0, controller number 0x10, on MIDI channel 1, with resolution 64
Analog fader2(A1, 0x11, 1 , RESOLUTION);
Analog fader3(A2, 0x12, 1 , RESOLUTION);
Analog fader4(A3, 0x13, 1 , RESOLUTION);

Analog potTop1(A4, 0x14, 1 , RESOLUTION);                // Create a new instance of class 'Analog' called 'potTop1', on pin A4, controller number 0x14, on MIDI channel 1, with resolution 64
Analog potTop2(A5, 0x15, 1 , RESOLUTION);
Analog potTop3(A6, 0x16, 1 , RESOLUTION);
Analog potTop4(A7, 0x17, 1 , RESOLUTION);

Analog potSide1(A8, 0x18, 1 , RESOLUTION);               // Create a new instance of class 'Analog' called 'potSide1', on pin A8, controller number 0x18, on MIDI channel 1, with resolution 64
Analog potSide2(A9, 0x19, 1 , RESOLUTION);
Analog potSide3(A10, 0x1A, 1 , RESOLUTION);
Analog potSide4(A11, 0x1B, 1 , RESOLUTION);

DigitalLatch switch1(2, 60, 1, VELOCITY, LATCHTIME);    // Create a new instance of class 'DigitalLatch' called 'switch1', on pin 0, note number 60 on MIDI channel 1, with a predefined latch time
DigitalLatch switch2(3, 61, 1, VELOCITY, LATCHTIME);
DigitalLatch switch3(5, 62, 1, VELOCITY, LATCHTIME);
DigitalLatch switch4(7, 63, 1, VELOCITY, LATCHTIME);

RotaryEncoder enc1(1, 0, 0x2F, 1, SPEED_MULTIPLY, NORMAL_ENCODER, POS1_NEG127); // Create a new instance of class 'RotaryEncoder' called enc1, on pins 1 and 0, controller number 0x2F, on MIDI channel 1, at normal speed, using a normal encoder (4 pulses per click/step), using the POS1_NEG127 sign option


//_____________________________________________________________________________________________________________________________________________________________________________________________

void setup()
{
  fader1.bank(11, 0x10, 2);        // if pin 11 is pulled low (by a switch) fader1 will be controller number 0x10, on channel 2, if the switch is off (i.e. 1) it will keep the settings declared in the section above.
  fader2.bank(11, 0x11, 2);
  fader3.bank(11, 0x12, 2);
  fader4.bank(11, 0x13, 2);

  //potTop1.bank(11, 0x14, 2);    // you could add these pots to the same bank if you wanted to
  //potTop2.bank(11, 0x15, 2);
  //potTop3.bank(11, 0x16, 2);
  //potTop4.bank(11, 0x17, 2);

  potSide1.bank(11, 0x18, 2);
  potSide2.bank(11, 0x19, 2);
  potSide3.bank(11, 0x1A, 2);
  potSide4.bank(11, 0x1B, 2);

  switch1.bank(11, 60, 2);
  switch2.bank(11, 61, 2);
  switch3.bank(11, 62, 2);
  switch4.bank(11, 63, 2);

  USBMidiController.blink(13);  // flash the LED on pin 13 on every message
  USBMidiController.setDelay(15);  // wait 15 ms after each message not to flood the connection

  delay(1000);         // Wait a second...
}

//_____________________________________________________________________________________________________________________________________________________________________________________________

void loop() // Refresh all inputs
{
  fader1.refresh();
  fader2.refresh();
  fader3.refresh();
  fader4.refresh();

  potTop1.refresh();
  potTop2.refresh();
  potTop3.refresh();
  potTop4.refresh();

  potSide1.refresh();
  potSide2.refresh();
  potSide3.refresh();
  potSide4.refresh();

  switch1.refresh();
  switch2.refresh();
  switch3.refresh();
  switch4.refresh();

  enc1.refresh();
}