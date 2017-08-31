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


  Written by tttapa, 29-06-2017
  https://github.com/tttapa/MIDI_controller
*/


#include <MIDI_controller.h>

#define VELOCITY          0b01111111  // The velocity of the buttons (0b01111111 = 127 = 100%)
#define LATCHTIME         100         // How long a note will be held on, in DigitalLatch mode (in milliseconds).

#define SPEED_MULTIPLY    1           // If the jog wheels or other encoders are too slow in your software, increase this value 
// (it will be multiplied with the actual speed of the encoder, as the name implies.) Default is 1.
#define PULSES_PER_STEP   4           // This is the number of pulses the encoder outputs when you turn it one step (or click) further. Use 4 for a normal rotary encoder, and 1 for a jogwheel.
// If you set it to 1, this uses the maximum resolution. If it is set to 4, one message will be sent per click of the encoder. 1 click matches 1 unit in the software. This is more logical for most usages (except jogwheels).

#define ANALOG_AVERAGE    8           // Use the average of 8 samples to get smooth transitions and prevent noise

const uint8_t channelVolume = 0x07;   // Controller 7 is defined as MIDI channel volume

//_____________________________________________________________________________________________________________________________________________________________________________________________

Analog faders[] = {
  {A0, channelVolume, 1},        // Create a new instance of class 'Analog' on pin A0, controller number 0x07 (channel volume), on MIDI channel 1.
  {A1, channelVolume, 2},
  {A2, channelVolume, 3},
  {A3, channelVolume, 4},
};

Analog knobsTop[] = {
  {A4, 0x14, 1},                // Create a new instance of class 'Analog' on pin A4, controller number 0x14, on MIDI channel 1.
  {A5, 0x15, 1},
  {A6, 0x16, 1},
  {A7, 0x17, 1},
};

Analog knobsSide[] = {
  {A8, 0x18, 1},               // Create a new instance of class 'Analog' called 'potSide1', on pin A8, controller number 0x18, on MIDI channel 1.
  {A9, 0x19, 1},
  {A10, 0x1A, 1},
  {A11, 0x1B, 1},
};

DigitalLatch switches[] = {
  {2, 60, 1, VELOCITY, LATCHTIME},    // Create a new instance of class 'DigitalLatch' on pin 0, note number 60 on MIDI channel 1, with a predefined latch time
  {3, 61, 1, VELOCITY, LATCHTIME},
  {5, 62, 1, VELOCITY, LATCHTIME},
  {7, 63, 1, VELOCITY, LATCHTIME},
};

RotaryEncoder enc1 = {1, 0, 0x2F, 1, SPEED_MULTIPLY, NORMAL_ENCODER, POS1_NEG127}; // Create a new instance of class 'RotaryEncoder' called enc1, on pins 1 and 0, controller number 0x2F, on MIDI channel 1, at normal speed, using a normal encoder (4 pulses per click/step), using the POS1_NEG127 sign option

Bank bank;

BankSelector bankselector(bank, 11, LED_BUILTIN, BankSelector::TOGGLE);

//_____________________________________________________________________________________________________________________________________________________________________________________________

void setup()
{
  for (uint8_t i = 0; i < sizeof(faders) / sizeof(faders[0]); i++) {
    bank.add(faders[i], Bank::CHANGE_CHANNEL);
    faders[i].average(ANALOG_AVERAGE);
  }
  for (uint8_t i = 0; i < sizeof(knobsTop) / sizeof(knobsTop[0]); i++) {
    // bank.add(knobsTop[i], Bank::CHANGE_CHANNEL);
    knobsTop[i].average(ANALOG_AVERAGE);
  }
  for (uint8_t i = 0; i < sizeof(knobsSide) / sizeof(knobsSide[0]); i++) {
    bank.add(knobsSide[i], Bank::CHANGE_CHANNEL);
    knobsSide[i].average(ANALOG_AVERAGE);
  }
  for (uint8_t i = 0; i < sizeof(switches) / sizeof(switches[0]); i++) {
    bank.add(switches[i], Bank::CHANGE_CHANNEL);
  }
}

//_____________________________________________________________________________________________________________________________________________________________________________________________

void loop() // Refresh all inputs
{
  MIDI_Controller.refresh();
}
