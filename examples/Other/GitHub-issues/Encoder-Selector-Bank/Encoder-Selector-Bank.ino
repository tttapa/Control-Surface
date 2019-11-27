/**
 * https://github.com/tttapa/Control-Surface/issues/66
 */

#include <Encoder.h>
// Encoder must be included before Control Surface
#include <Control_Surface.h>

USBMIDI_Interface midi;

Bank<120> bank = {1};

EncoderSelector<120> selector = {
  bank,        // bank to manage
  {2, 3},      // encoder pins
  4,           // encoder pulses per step
  Wrap::Clamp, // clamp or wrap around when going beyond maximum/minimum setting
};

Bankable::CCPotentiometer potentiometer = {
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