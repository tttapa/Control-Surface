#include <MIDI_Controller.h>
// #include "VU.h"
#include "MIDI_Input_7SegmentDisplay.h"

using namespace ExtIO;

// #define DEBUG

const uint8_t clockPin = 10;
const uint8_t latchPin = 11;
const uint8_t dataPin = 12;

const uint8_t SOLO = 8;
const uint8_t MUTE = 0x10;

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

#ifdef DEBUG
USBDebugMIDI_Interface dbg(115200); // Instantiate a USB Debug output
#else
USBMIDI_Interface midi; // Instantiate a USB MIDI output
#endif

MCU_TimeDisplay tdisp;
MCU_AssignmentDisplay adisp;  


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void setup() {
  Serial.begin(115200);
  // #ifdef DEBUG
  while (!Serial);
  delay(500);
  // #endif
  startMIDI();
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void loop() {
  MIDI_Controller.refresh();
}



