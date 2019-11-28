// TODO: documentation
// @boards  AVR, AVR USB, Due, Teensy 3.x, ESP32

#include <Control_Surface.h>

USBMIDI_Interface midi;

// The note numbers corresponding to the buttons in the matrix
const AddressMatrix<4, 3> addresses = {{
  {1, 2, 3},
  {4, 5, 6},
  {7, 8, 9},
  {10, 11, 12},
}};

NoteButtonMatrix<4, 3> buttonmatrix = {
  {2, 3, 4, 5}, // row pins
  {6, 7, 8},    // column pins
  addresses,    // address matrix
  CHANNEL_1,    // channel and cable number
};

void setup() {
  Control_Surface.begin();
}

void loop() {
  Control_Surface.loop();
}