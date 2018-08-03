#include <Control_Surface.h> // Include the Control Surface library

// Instantiate a MIDI interface to use for the Control Surface.
USBMIDI_Interface midi;

// Instantiate a program changer with four programs: 0x10, 0x20, 0x30, 0x40 
// on channel 1
ProgramChanger<4> pc = { { 0x10, 0x20, 0x30, 0x40 }, 1 };

// Instantiate a selector for the program changer on pins 5 (increment) and
// pin 6 (decrement).
IncrementDecrementSelector<4> sel = { pc, {5, 6} };

void setup() {
  Control_Surface.begin(); // Initialize Control Surface
}

void loop() {
  Control_Surface.loop();
}
