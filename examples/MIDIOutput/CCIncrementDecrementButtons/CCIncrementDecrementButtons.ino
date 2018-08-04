#include <Control_Surface.h> // Include the Control Surface library

// Instantiate a MIDI interface to use for the Control Surface.
USBMIDI_Interface midi;

CCIncrementDecrementButtons buttons = { {5, 6}, MCU::V_POT_1, 1, 5, MCU::V_POT_SELECT_1 };

void setup() {
  RelativeCCSender::setMode(MACKIE_CONTROL_RELATIVE);
  Control_Surface.begin(); // Initialize Control Surface
}

void loop() {
  Control_Surface.loop();
}
