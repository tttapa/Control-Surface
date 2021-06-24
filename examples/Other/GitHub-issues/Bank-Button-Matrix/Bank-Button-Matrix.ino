/**
 * <https://github.com/tttapa/Control-Surface/issues/38>
 */

#include <Control_Surface.h>

USBMIDI_Interface midi;

Bank<4> bank {6}; // 4 banks, 6 addresse per banks
IncrementDecrementSelector<4> selector {bank, {A0, A1}, Wrap::Wrap};

AddressMatrix<2, 3> notes {{
  {0, 1, 2},
  {3, 4, 5},
}};

Bankable::NoteButtonMatrix<2, 3> buttons {
  bank,      //
  {2, 3},    // row pins (outputs, driven low-Z low !)
  {4, 5, 6}, // column pins (inputs, hi-Z)
  notes,     //
  CHANNEL_1, //
};

Bankable::NoteLED<4> leds[] {
  {bank, 7, {notes[0][0], CHANNEL_1}},  //
  {bank, 8, {notes[0][1], CHANNEL_1}},  //
  {bank, 9, {notes[0][2], CHANNEL_1}},  //
  {bank, 10, {notes[1][0], CHANNEL_1}}, //
  {bank, 11, {notes[1][1], CHANNEL_1}}, //
  {bank, 12, {notes[1][2], CHANNEL_1}}, //
};

void setup() {
  Control_Surface.begin();
}

void loop() {
  Control_Surface.loop();
}