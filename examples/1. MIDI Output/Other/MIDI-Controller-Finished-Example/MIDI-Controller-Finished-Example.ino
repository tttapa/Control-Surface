/** 
 * This is the code I used for my specific MIDI controller.  
 * 
 * @boards  Leonardo
 * 
 * Connections
 * -----------
 * 
 * I used an Arduino Leonardo with:
 * - 4 faders on A0-A3
 * - 8 potentiometers on A4-A11
 * - 1 rotary encoder on pins 0-1
 * - 4 toggle switches on pins 2, 3, 5 and 7
 * - 1 toggle switch (for bank select) on pin 11
 * - 1 LED on pin 13
 *
 * Behavior
 * --------
 * 
 * - When bank select is 0, faders 1-4 are channel volumes 1-4, potSide 1-4 are 
 *   pan/balance of channel 1-4, switches 1-4 are mute channel 5-8.
 * - When bank select is 1, faders 1-4 are channel volumes 5-8, potSide 1-4 are 
 *   pan/balance of channel 5-8, switches 1-4 are mute channel 1-4.
 * - PotTop 1-4 are not in the bank, because I use them as effect or EQ
 *   parameters, and they are channel independent.
 * - Enc1 is used for scrolling.
 *
 * Mapping
 * -------
 * 
 * Create a custom mapping using the MIDI learn option in your DAW.
 *
 * Written by PieterP, 07-09-2017
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h>

USBMIDI_Interface usbmidi;

// If the jog wheels or other encoders are too slow in your software, increase
// this value.
// (It will be multiplied with the actual speed of the encoder, as the name
// implies.) Default is 1.
const int speedMultiplier = 1;

Bank<2> bank(4); // A bank with four channels, and 2 bank settings

Bankable::CCPotentiometer faders[] {
  {{bank, BankType::ChangeChannel}, A0, {MIDI_CC::Channel_Volume, Channel_1}},
  {{bank, BankType::ChangeChannel}, A1, {MIDI_CC::Channel_Volume, Channel_2}},
  {{bank, BankType::ChangeChannel}, A2, {MIDI_CC::Channel_Volume, Channel_3}},
  {{bank, BankType::ChangeChannel}, A3, {MIDI_CC::Channel_Volume, Channel_4}},
};

CCPotentiometer knobsTop[] {
  {A4, MIDI_CC::General_Purpose_Controller_1},
  {A5, MIDI_CC::General_Purpose_Controller_2},
  {A6, MIDI_CC::General_Purpose_Controller_3},
  {A7, MIDI_CC::General_Purpose_Controller_4},
};

Bankable::CCPotentiometer knobsSide[] {
  {{bank, BankType::ChangeChannel}, A8, {MIDI_CC::Pan, Channel_1}},
  {{bank, BankType::ChangeChannel}, A9, {MIDI_CC::Pan, Channel_2}},
  {{bank, BankType::ChangeChannel}, A10, {MIDI_CC::Pan, Channel_3}},
  {{bank, BankType::ChangeChannel}, A11, {MIDI_CC::Pan, Channel_4}},
};

Bankable::NoteButtonLatching switches[] {
  {{bank, BankType::ChangeAddress}, 2, MCU::MUTE_1},
  {{bank, BankType::ChangeAddress}, 3, MCU::MUTE_2},
  {{bank, BankType::ChangeAddress}, 5, MCU::MUTE_3},
  {{bank, BankType::ChangeAddress}, 7, MCU::MUTE_4},
};

CCRotaryEncoder enc {
  {1, 0},                                // pins
  MIDI_CC::General_Purpose_Controller_5, // address
  speedMultiplier,                       // multiplier
  4,                                     // pulses per click
};

SwitchSelector selector {bank, 11};

void setup() {
  Control_Surface.begin();
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() { // Refresh all inputs
  Control_Surface.loop();
  digitalWrite(LED_BUILTIN, digitalRead(11));
}