#include <MIDI_controller.h>

const uint8_t MUTE = 0x10;

MIDI_LED muteled(13, MUTE, 1, 1, 1);

Digital mutebutton(2, MUTE, 1, 127);

void setup() {}

void loop() {
  MIDI_Controller.refresh();
}
