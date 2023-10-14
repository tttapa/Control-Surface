/**
 * <https://github.com/tttapa/MIDI_controller/issues/97#issuecomment-564247602>
 * 
 * Many bankable potentiometers and buttons. Bank selector displays bank setting
 * using LEDs.  
 * If any of the buttons is pressed, the LED on pin 22 is turned on.
 * 
 * @boards  Mega
 */

#include <Control_Surface.h>

#include <AH/STL/algorithm> // std::any_of

USBMIDI_Interface midi; // MIDI Interface to use

Bank<4> bank(16);

// Create a new bank selector that changes the bank setting of the bank we just
// created.
// It has push buttons connected to pins 21 and 20 that increment or decrement
// the bank setting, and 4 LEDs to pins 46, 48, 50, 52 that display the current
// bank setting.
IncrementDecrementSelectorLEDs<4> bankSelector {
  bank,
  {21, 20},         // button pins
  {46, 48, 50, 52}, // LED pins
};

using namespace MIDI_CC;
Bankable::CCPotentiometer potentiometers[] {
  {{bank, BankType::ChangeChannel}, A0, {Channel_Volume, Channel_1}},
  {{bank, BankType::ChangeChannel}, A1, {Channel_Volume, Channel_2}},
  {{bank, BankType::ChangeChannel}, A2, {Sound_Controller_3, Channel_3}},
  {{bank, BankType::ChangeChannel}, A3, {Sound_Controller_4, Channel_4}},
  {{bank, BankType::ChangeChannel}, A4, {Sound_Controller_5, Channel_5}},
  {{bank, BankType::ChangeChannel}, A5, {Sound_Controller_6, Channel_6}},
  {{bank, BankType::ChangeChannel}, A6, {Effects_1, Channel_7}},
  {{bank, BankType::ChangeChannel}, A7, {Effects_2, Channel_8}},
  {{bank, BankType::ChangeChannel}, A8, {Effects_3, Channel_9}},
  {{bank, BankType::ChangeChannel}, A9, {Effects_4, Channel_10}},
  {{bank, BankType::ChangeChannel}, A10, {Effect_Control_1, Channel_11}},
  {{bank, BankType::ChangeChannel}, A11, {Effect_Control_2, Channel_12}},
};

Bankable::NoteButton muteButtons[] {
  {bank, 13, 0x14},
  {bank, 12, 0x15},
  {bank, 11, 0x16},
  {bank, 10, 0x17},
  {bank, 9, 0x18},
  {bank, 8, 0x19},
  {bank, 7, 0x1A},
  {bank, 6, 0x1B},
  {bank, 5, 0x1C},
  {bank, 4, 0x1D},
  {bank, 3, 0x1E},
  {bank, 2, 0x1F},
  {bank, 14, 0x20},
  {bank, 15, 0x21},
  {bank, 16, 0x22},
  {bank, 17, 0x23},
  {bank, 18, {0x33, Channel_2}},
  {bank, 19, {0x34, Channel_2}},
};

constexpr pin_t ledPin = 22;

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

void setup() {
  Control_Surface.begin();
  pinMode(ledPin, OUTPUT);
}

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

void loop() {
  Control_Surface.loop();

  // Function that checks if a given button is pressed
  auto checkButtonPressed = [](const Bankable::NoteButton &button) {
    return button.getButtonState() == Button::Pressed;
  };
  // If any of the push buttons is pressed
  bool pressed = std::any_of(std::begin(muteButtons), std::end(muteButtons),
                             checkButtonPressed);
  // Turn on the LED
  digitalWrite(ledPin, pressed);
}