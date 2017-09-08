#include <MIDI_Controller.h>
#include "MIDI_Input_VU.h"
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

ShiftRegisterOut SR(dataPin, clockPin, latchPin, LSBFIRST, 24);
ShiftRegisterOut SR_VU(dataPin, clockPin, 16, LSBFIRST, 8);

MCU_TimeDisplay tdisp;
MCU_AssignmentDisplay adisp;

Bank bankVU;

BankSelector bsVU(bankVU, { 6, 5 }, {
  SR.blue(0),
  SR.blue(1),
  SR.blue(2),
  SR.blue(3),
  SR.blue(4),
  SR.blue(5),
  SR.blue(6),
  SR.blue(7),
});

MCU_VU_LED vu(SR_VU.pin(0), 6, SR_VU.pin(7), 0, 8);

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void setup() {
  Serial.begin(115200);
#ifdef DEBUG
  while (!Serial);
  delay(500);
#endif
  startMIDI();

  SR.begin();
  SR_VU.begin();

  bankVU.add(vu, Bank::CHANGE_ADDRESS);

  bsVU.init();
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void loop() {
  MIDI_Controller.refresh();
}



