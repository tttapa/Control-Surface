/*
  This is the code I used for my specific MIDI controller.
  I used an Arduino Leonardo with 4 faders on A0-A3, 8 potentiometers on A4-A11, a rotary encoder on pin 0&1, 4 toggle switches on pins 2, 3, 5 and 7, a toggle switch (for bank select) on pin 11, and an LED on pin 13.

  When bank select is 0, faders 1-4 are channel volumes 1-4, potSide 1-4 are pan/balance of channel 1-4, switches 1-4 are mute channel 5-8.
  When bank select is 1, faders 1-4 are channel volumes 5-8, potSide 1-4 are pan/balance of channel 5-8, switches 1-4 are mute channel 1-4.
  PotTop 1-4 are not in the bank, because I use them as effect or EQ parameters, and they are channel independent.
  Enc1 is used for scrolling.

  Note that I used a custom control mapping in my DAW.

  If you are using a Teensy, make sure you have the USB type set to MIDI + Serial.
  If you are using an Arduino Uno or Mega, use the HIDUINO firmware for the ATmega16U2.


  Written by tttapa, 07-09-2017
  https://github.com/tttapa/Control-Surface
*/

#include <Encoder.h>
#include <Control_Surface.h>

const int speedMultiply = 1; // If the jog wheels or other encoders are too slow in your software, increase this value
                             // (it will be multiplied with the actual speed of the encoder, as the name implies.) Default is 1.

//_____________________________________________________________________________________________________________________________________________________________________________________________

CCPotentiometer faders[] = {
    {A0, MIDI_CC::Channel_Volume, 1}, // Create a new instance of class 'Analog' on pin A0, controller number 0x07 (channel volume), on MIDI channel 1.
    {A1, MIDI_CC::Channel_Volume, 2},
    {A2, MIDI_CC::Channel_Volume, 3},
    {A3, MIDI_CC::Channel_Volume, 4},
};

CCPotentiometer knobsTop[] = {
    {A4, 0x10, 1}, // Create a new instance of class 'Analog' on pin A4, controller number 0x10 (General Purpose Controller 1), on MIDI channel 1.
    {A5, 0x11, 1},
    {A6, 0x12, 1},
    {A7, 0x13, 1},
};

CCPotentiometer knobsSide[] = {
    {A8,  MIDI_CC::Pan, 1}, // Create a new instance of class 'Analog' called 'potSide1', on pin A8, controller number 0x0A (pan), on MIDI channel 1.
    {A9,  MIDI_CC::Pan, 2},
    {A10, MIDI_CC::Pan, 3},
    {A11, MIDI_CC::Pan, 4},
};

NoteButtonLatching switches[] = {
    {2, 0x10, 1}, // Create a new instance of class 'DigitalLatch' on pin 0, note number 16 (mute) on MIDI channel 1
    {3, 0x11, 1},
    {5, 0x12, 1},
    {7, 0x13, 1},
};

RotaryEncoder enc = {1, 0, 0x2F, 1, speedMultiply, NORMAL_ENCODER, TWOS_COMPLEMENT}; // Create a new instance of class 'RotaryEncoder' called enc, on pins 1 and 0, controller number 0x2F, on MIDI channel 1, at normal speed, using a normal encoder (4 pulses per click/step), using the TWOS_COMPLEMENT sign option

Bank bank(4); // A bank with four channels

BankSelector bankselector(bank, 11, LED_BUILTIN, BankSelector::TOGGLE); // A bank selector with a single toggle switch on pin 11 and an LED for feedback on pin 13

//_____________________________________________________________________________________________________________________________________________________________________________________________

void setup()
{
    bank.add(faders, Bank::CHANGE_CHANNEL); // Add the control elements to the bank
    bank.add(knobsSide, Bank::CHANGE_CHANNEL);
    bank.add(switches, Bank::CHANGE_ADDRESS);
}

//_____________________________________________________________________________________________________________________________________________________________________________________________

void loop() // Refresh all inputs
{
    Control_Surface.update();
}
