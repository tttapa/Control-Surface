#include <Control_Surface.h>

USBMIDI_Interface midi;

PCButton pcBtn = {
  2,                                 // pin
  {MIDI_PC::Steel_Drums, CHANNEL_1}, // address
};

void setup() {
  Control_Surface.begin();
}

void loop() {
  Control_Surface.loop();
}
