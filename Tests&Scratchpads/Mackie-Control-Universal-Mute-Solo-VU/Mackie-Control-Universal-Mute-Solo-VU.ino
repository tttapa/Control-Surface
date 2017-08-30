#include <MIDI_controller.h>
#include "VU.h"

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

MIDI_Input_LED muteled(14, MUTE, 1, 8, 1);
MIDI_Input_LED sololed(15, SOLO, 1, 8, 1);

Digital mutebutton(2, MUTE, 1, 127);
Digital solobutton(3, SOLO, 1, 127);

Bank bank;
Bank bankVU;

BankSelector bs(bank, { 9, 8 }, {
  SR.red(0),
  SR.red(1),
  SR.red(2),
  SR.red(3),
  SR.red(4),
  SR.red(5),
  SR.red(6),
  SR.red(7),
});

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

VU vu(SR_VU.pin(0), 6, SR_VU.pin(6), 0, 8);

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void setup() {
  Serial.begin(115200);
  while (!Serial);
  delay(500);
  startMIDI();
  
  SR.begin();
  SR_VU.begin();

  bank.add(muteled, Bank::CHANGE_ADDRESS);
  bank.add(sololed, Bank::CHANGE_ADDRESS);

  bank.add(mutebutton, Bank::CHANGE_ADDRESS);
  bank.add(solobutton, Bank::CHANGE_ADDRESS);

  bankVU.add(vu, Bank::CHANGE_ADDRESS);

  bs.init();
  bsVU.init();
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void loop() {
  MIDI_Controller.refresh();
}



