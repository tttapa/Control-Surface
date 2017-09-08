#include <MIDI_Controller.h>

const uint8_t MUTE = 0x10;

MIDI_LED muteled(LED_BUILTIN, MUTE, 1, 1, 1);

Digital mutebutton(2, MUTE, 1, 127);

void setup() {}

void loop() {
  MIDI_Controller.refresh();
}
