#include <Control_Surface.h> // Include the Control Surface library

// Instantiate a MIDI interface to use for the Control Surface.
// USBMIDI_Interface midi;
USBDebugMIDI_Interface midi(115200);

// Instantiate a program changer with programs 0x10, 0x20, 0x30, 0x40 
// on channel 1
ProgramChanger<4> pc = { { 0x10, 0x20, 0x30, 0x40 }, 1 };

IncrementDecrementSelector<4> sel = { pc, {5, 6} };

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Control_Surface.begin(); // Initialize Control Surface
}

void loop() {
  Control_Surface.loop(); // Refresh all elements
}
