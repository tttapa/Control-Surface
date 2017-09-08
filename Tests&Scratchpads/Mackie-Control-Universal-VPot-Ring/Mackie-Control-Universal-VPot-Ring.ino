#include <MIDI_Controller.h>
#include "MIDI_Input_V-Pot_Ring.h"

using namespace ExtIO;

#define DEBUG

const uint8_t clockPin = 10;
const uint8_t latchPin = 11;
const uint8_t dataPin = 12;

const uint8_t SOLO = 8;
const uint8_t MUTE = 0x10;

const uint8_t VPOT = 0x10;

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

#ifdef DEBUG
USBDebugMIDI_Interface dbg(115200); // Instantiate a USB Debug output
#else
USBMIDI_Interface midi; // Instantiate a USB MIDI output
#endif

ShiftRegisterOut SR_BS(dataPin, clockPin, latchPin, LSBFIRST, 24);
ShiftRegisterOut SR(dataPin, clockPin, 16, LSBFIRST, 8);

Bank bank;

BankSelector bs(bank, { 6, 5 }, {
  SR_BS.blue(0),
  SR_BS.blue(1),
  SR_BS.blue(2),
  SR_BS.blue(3),
  SR_BS.blue(4),
  SR_BS.blue(5),
  SR_BS.blue(6),
  SR_BS.blue(7),
});

const pin_t ledring[11] = {
  SR_BS.green(0),
  SR_BS.green(1),
  SR_BS.green(2),
  SR_BS.green(3),
  SR_BS.green(4),
  SR_BS.green(5),
  SR_BS.green(6),
  SR_BS.green(7),
  SR.pin(0),
  SR.pin(1),
  SR.pin(2),
};

RotaryEncoder encoder(0, 1, VPOT, 1, 1, NORMAL_ENCODER, MACKIE_CONTROL_RELATIVE);
MCU_VPot_Ring_LED ring(ledring, SR.pin(7), 0, 8);

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void setup() {
  Serial.begin(115200);
#ifdef DEBUG
  while (!Serial);
  delay(500);
#endif
  startMIDI();

  SR.begin();
  SR_BS.begin();

  bank.add(ring, Bank::CHANGE_ADDRESS);

  bs.init();
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void loop() {
  MIDI_Controller.refresh();
}



