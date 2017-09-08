/*
  This is the code I used for my specific MIDI controller.
  I used an Arduino Leonardo with 4 faders on A0-A3, 8 potentiometers on A4-A11, a rotary encoder on pin 0&1, 4 toggle switches on pins 2, 3, 5 and 7, a toggle switch (for bank select) on pin 11, and an LED on pin 13.

  When bank select is 0, faders 1-4 are Pitch Bends on channels 1-4, potSide 1-4 are pan/balance of channel 1-4, switches 1-4 are mute channel 5-8.
  When bank select is 1, faders 1-4 are Pitch Bends on 5-8, potSide 1-4 are pan/balance of channel 5-8, switches 1-4 are mute channel 1-4.
  PotTop 1-4 are not in the bank, because I use them as effect or EQ parameters, and they are channel independent.
  Enc1 is used for scrolling.

  Note that I used a custom control mapping in my DAW.

  If you are using a Teensy, make sure you have the USB type set to MIDI.
  If you are using an Arduino Uno or Mega, use the HIDUINO firmware for the ATmega16U2.


  Written by tttapa, 29-06-2017
  https://github.com/tttapa/MIDI_controller
*/


#include <MIDI_Controller.h>

#define VELOCITY          0b01111111  // The velocity of the buttons (0b01111111 = 127 = 100%)
#define LATCHTIME         100         // How long a note will be held on, in DigitalLatch mode (in milliseconds).

#define SPEED_MULTIPLY    1           // If the jog wheels or other encoders are too slow in your software, increase this value 
// (it will be multiplied with the actual speed of the encoder, as the name implies.) Default is 1.
#define PULSES_PER_STEP   4           // This is the number of pulses the encoder outputs when you turn it one step (or click) further. Use 4 for a normal rotary encoder, and 1 for a jogwheel.
// If you set it to 1, this uses the maximum resolution. If it is set to 4, one message will be sent per click of the encoder. 1 click matches 1 unit in the software. This is more logical for most usages (except jogwheels).

#define ANALOG_AVERAGE    8           // Use the average of 8 samples to get smooth transitions and prevent noise

//_____________________________________________________________________________________________________________________________________________________________________________________________

AnalogHiRes fader1(A0, 1);                  // Create a new instance of class 'AnalogHiRes' called 'fader1', on pin A0, on MIDI channel 1.
AnalogHiRes fader2(A1, 2);
AnalogHiRes fader3(A2, 3);
AnalogHiRes fader4(A3, 4);

Analog potTop1(A4, 0x14, 1);                // Create a new instance of class 'Analog' called 'potTop1', on pin A4, controller number 0x14, on MIDI channel 1.
Analog potTop2(A5, 0x15, 1);
Analog potTop3(A6, 0x16, 1);
Analog potTop4(A7, 0x17, 1);

Analog potSide1(A8, 0x18, 1);               // Create a new instance of class 'Analog' called 'potSide1', on pin A8, controller number 0x18, on MIDI channel 1.
Analog potSide2(A9, 0x19, 1);
Analog potSide3(A10, 0x1A, 1);
Analog potSide4(A11, 0x1B, 1);

DigitalLatch switch1(2, 60, 1, VELOCITY, LATCHTIME);    // Create a new instance of class 'DigitalLatch' called 'switch1', on pin 0, note number 60 on MIDI channel 1, with a predefined latch time
DigitalLatch switch2(3, 61, 1, VELOCITY, LATCHTIME);
DigitalLatch switch3(5, 62, 1, VELOCITY, LATCHTIME);
DigitalLatch switch4(7, 63, 1, VELOCITY, LATCHTIME);

RotaryEncoder enc1(1, 0, 0x2F, 1, SPEED_MULTIPLY, NORMAL_ENCODER, POS1_NEG127); // Create a new instance of class 'RotaryEncoder' called enc1, on pins 1 and 0, controller number 0x2F, on MIDI channel 1, at normal speed, using a normal encoder (4 pulses per click/step), using the POS1_NEG127 sign option


//_____________________________________________________________________________________________________________________________________________________________________________________________

void setup()
{
  fader1.bank(11, 5);             // if pin 11 is pulled low (by a switch) fader1 will operate on channel 5, if the switch is off (i.e. 1) it will keep the channel specified in the section above.
  fader2.bank(11, 6);
  fader3.bank(11, 7);
  fader4.bank(11, 8);

  // potTop1.bank(11, 0x14, 2);   // you could add these pots to the same bank if you wanted to
  // potTop2.bank(11, 0x15, 2);
  // potTop3.bank(11, 0x16, 2);
  // potTop4.bank(11, 0x17, 2);

  potSide1.bank(11, 0x18, 2);     // if pin 11 is pulled low (by a switch) potSide1 will be controller number 0x18, on channel 2, if the switch is off (i.e. 1) it will keep the settings declared in the section above.
  potSide2.bank(11, 0x19, 2);
  potSide3.bank(11, 0x1A, 2);
  potSide4.bank(11, 0x1B, 2);

  switch1.bank(11, 60, 2);
  switch2.bank(11, 61, 2);
  switch3.bank(11, 62, 2);
  switch4.bank(11, 63, 2);

  fader1.average(ANALOG_AVERAGE * 8);      // Use the average of 64 samples to get smooth transitions and prevent noise (HiRes inputs are more susceptable to noise)
  fader2.average(ANALOG_AVERAGE * 8);
  fader3.average(ANALOG_AVERAGE * 8);
  fader4.average(ANALOG_AVERAGE * 8);

  potTop1.average(ANALOG_AVERAGE);         // Use the average of 8 samples to get smooth transitions and prevent noise
  potTop2.average(ANALOG_AVERAGE);
  potTop3.average(ANALOG_AVERAGE);
  potTop4.average(ANALOG_AVERAGE);

  potSide1.average(ANALOG_AVERAGE);
  potSide2.average(ANALOG_AVERAGE);
  potSide3.average(ANALOG_AVERAGE);
  potSide4.average(ANALOG_AVERAGE);

  USBMidiController.blink(LED_BUILTIN);  // flash the built-in LED (pin 13 on most boards) on every message
  USBMidiController.setDelay(0);  // When using HiRes faders with large average buffers, it's best to minimize the delay after each message
  USBMidiController.begin();  // Initialise the USB MIDI connection

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