/**
 * https://github.com/tttapa/Control-Surface/issues/66
 */

#include <Control_Surface.h>

// MIDI Interface to use
USBMIDI_Interface midi;

// Bank with 120 settings in total, increments of 1 address at a time
Bank<120> bank {1};

// A rotary encoder to select one of the 120 bank settings
EncoderSelector<120> selector {
  bank,        // bank to manage
  {2, 3},      // encoder pins
  4,           // encoder pulses per step
  Wrap::Clamp, // clamp or wrap around when going beyond maximum/minimum setting
};

// The potentiometer that sends MIDI messages
// the address (controller number) depends on the bank setting
Bankable::CCPotentiometer potentiometer {
  {bank, BankType::CHANGE_ADDRESS}, // Bank configuration
  A0,                               // Analog pin for potentiometer
  {0x00, CHANNEL_1},                // Base CC address
};

void setup() {
  Control_Surface.begin();
}

void loop() {
  Control_Surface.loop();
  // You can get the bank number using:
  //   bank.getSelection();
}