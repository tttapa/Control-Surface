#include <MIDI_Controller.h>

using namespace ExtIO;

// #define DEBUG

const uint8_t dataPin = 12;
const uint8_t clockPin = 10;
const uint8_t latchPin = 11;

const uint8_t REC_RDY = 0;
const uint8_t SOLO = 8;
const uint8_t MUTE = 0x10;
const uint8_t SELECT = 0x18;

const uint8_t PAN = 0x2A;
const uint8_t PLUGIN = 0x2B;
const uint8_t VPOT_SW = 0x20;

const uint8_t VPOT = 0x10;

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

#ifdef DEBUG
USBDebugMIDI_Interface dbg(115200); // Instantiate a USB Debug output
#else
USBMIDI_Interface midi; // Instantiate a USB MIDI output
#endif

ShiftRegisterOut SR(dataPin, clockPin, latchPin, LSBFIRST, 24);

MIDI_Input_LED muteled(14, MUTE, 1, 8, 1);
MIDI_Input_LED sololed(15, SOLO, 1, 8, 1);

Bank bank;

BankSelector bs(bank, { 8, 9 }, {
  SR.red(0),
  SR.red(1),
  SR.red(2),
  SR.red(3),
  SR.red(4),
  SR.red(5),
  SR.red(6),
  SR.red(7),
});

Digital channelButtons[] = {
  {2, MUTE, 1, 127},
  {3, SOLO, 1, 127},
  {4, VPOT_SW, 1, 127},
  {5, SELECT, 1, 127},
};
const size_t nb_channelButtons = sizeof(channelButtons) / sizeof(Digital);

RotaryEncoder encoder(0, 1, VPOT, 1, 1, NORMAL_ENCODER, MACKIE_CONTROL_RELATIVE);

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void setup() {
  Serial.begin(115200);
  while (!Serial);
  delay(500);
  Serial.println(&midi == MIDI_Interface::getDefault());
  Serial.print(stringToPrint);
  startMIDI();
  SR.begin();

  bank.add(muteled, Bank::CHANGE_ADDRESS);
  bank.add(sololed, Bank::CHANGE_ADDRESS);

  for (uint8_t i = 0; i < nb_channelButtons; i++)
    bank.add(channelButtons[i], Bank::CHANGE_ADDRESS);
  bank.add(encoder, Bank::CHANGE_ADDRESS);

  bs.init();
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void loop() {
  bs.refresh();
}
