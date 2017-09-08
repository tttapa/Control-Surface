#include "Display.h"
#include <MIDI_Controller.h>

using namespace ExtIO;

// #define DEBUG_MIDI
// #define SERIAL_MIDI

const uint8_t clockPin = 10;
const uint8_t latchPin = 11;
const uint8_t dataPin = 12;

const uint8_t REC_RDY = 0;
const uint8_t SOLO = 8;
const uint8_t MUTE = 0x10;
const uint8_t SELECT = 0x18;

const uint8_t PAN = 0x2A;
const uint8_t PLUGIN = 0x2B;
const uint8_t VPOT_SW = 0x20;

const uint8_t VPOT = 0x10;

const uint8_t PLAY = 0x5E;

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

#ifdef DEBUG_MIDI
USBDebugMIDI_Interface dbg(115200); // Instantiate a USB Debug output
#elif defined (SERIAL_MIDI)
USBSerialMIDI_Interface smidi(115200);
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

Digital channelButtons[] = {
  {2, MUTE, 1, 127},
  {3, SOLO, 1, 127},
  {4, REC_RDY, 1, 127},
  {8, SELECT, 1, 127},
  {9, VPOT_SW, 1, 127},
};
const size_t nb_channelButtons = sizeof(channelButtons) / sizeof(Digital);

Digital playButton =   {7, PLAY, 1, 127};

RotaryEncoder encoder(0, 1, VPOT, 1, 1, NORMAL_ENCODER, MACKIE_CONTROL_RELATIVE);
MCU_VPot_Ring ring(0, 8);

MCU_VU vu(0, 8, false);

MCU_TimeDisplay tdisp;

MIDI_Input_Note_Buffer mute(MUTE, 1, 8, 1);
MIDI_Input_Note_Buffer solo(SOLO, 1, 8, 1);
MIDI_Input_Note_Buffer recrdy(REC_RDY, 1, 8, 1);
MIDI_Input_Note_Buffer play(PLAY, 1, 1, 1);

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void setup()   {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C
  Wire.setClock(1800000); // 1.8 MHz (set F_CPU to 96 MHz)
  display.clearDisplay();
  display.display();
#ifdef DEBUG_MIDI
  while (!Serial);
  delay(500);
#endif
  for (uint8_t i = 0; i < nb_channelButtons; i++)
    bank.add(channelButtons[i], Bank::CHANGE_ADDRESS);

  bank.add(encoder, Bank::CHANGE_ADDRESS);
  bank.add(ring, Bank::CHANGE_ADDRESS);
  bank.add(vu, Bank::CHANGE_ADDRESS);
  bank.add(mute, Bank::CHANGE_ADDRESS);
  bank.add(solo, Bank::CHANGE_ADDRESS);
  bank.add(recrdy, Bank::CHANGE_ADDRESS);

  MIDI_Controller.begin();
}

void loop() {
  static unsigned long start = 0;
  unsigned int loopTime = millis() - start;
  start = millis();

  display.clearDisplay();
  uint8_t i = ring.getPosition();
  // drawVPotSegment(display, i, 32, 32, 28, WHITE);
  drawGoodCircle(display, 16 + 64, 16, 16);

  drawVPotSegment(display, i, 16 + 64, 16, 14);

  drawBankSelector(display, bs, 64 + 12, 32 + 16);

  drawVUPeak(display, vu.getValue(), 32 + 64 + 8, 32 + 24, 16, 3);

  drawTimeDisplay(display, tdisp, 0, 0);

  drawCharacter(display, mute, 'M', 16 - 2, 14);
  drawCharacter(display, solo, 'S', 16 - 2, 14);
  drawCharacter(display, recrdy, 'R', 16 - 2, 36);

  drawCharacter(display, play, 'P', 16 + 16, 32 + 14);

  display.setTextSize(1);
  display.setCursor(5, 64 - 8);
  display.print(1000 / loopTime);

  display.display();
  MIDI_Controller.refresh();
}


