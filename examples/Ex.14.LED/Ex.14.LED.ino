#include <Control_Surface.h>

const uint8_t velocity = 0b1111111;

Digital button1(2, MCU::MUTE_1, 1, velocity);
Digital button2(3, MCU::MUTE_2, 1, velocity);

MIDI_LED led1(12, MCU::MUTE_1, 1); // LED on pin 12, note MCU::MUTE_1, channel 1
MIDI_LED led2(13, MCU::MUTE_2, 1);

void setup() {}

void loop() {
  Control_Surface.refresh();
}
