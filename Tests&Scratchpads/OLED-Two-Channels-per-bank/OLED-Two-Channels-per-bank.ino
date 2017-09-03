#include "Display.h"
#include "play.h"
#include "pause.h"
#include <MIDI_controller.h>

using namespace ExtIO;

// #define FPS

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

Bank bank(2); // two channels per bank

BankSelector bs(bank, { 6, 5 }, {
  SR_BS.blue(0),
  SR_BS.blue(1),
  SR_BS.blue(2),
  SR_BS.blue(3),
  /*SR_BS.blue(4),
    SR_BS.blue(5),
    SR_BS.blue(6),
    SR_BS.blue(7),*/
});

Digital channelButtons[] = {
  {2, MUTE, 1, 127},
  {3, SOLO, 1, 127},
  {4, REC_RDY, 1, 127},
  {8, MUTE + 1, 1, 127},
  {9, SOLO + 1, 1, 127},
};

Digital playButton =   {7, PLAY, 1, 127};

RotaryEncoder encoder(0, 1, VPOT, 1, 1, NORMAL_ENCODER, MACKIE_CONTROL_RELATIVE);

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

MCU_TimeDisplay tdisp;
MIDI_Input_Note_Buffer play(PLAY, 1, 1, 1);

MIDI_Input_Note_Buffer muteA(MUTE + 0, 1, 8, 1);
MIDI_Input_Note_Buffer muteB(MUTE + 1, 1, 8, 1);

MIDI_Input_Note_Buffer soloA(SOLO + 0, 1, 8, 1);
MIDI_Input_Note_Buffer soloB(SOLO + 1, 1, 8, 1);

MIDI_Input_Note_Buffer recrdyA(REC_RDY + 0, 1, 8, 1);
MIDI_Input_Note_Buffer recrdyB(REC_RDY + 1, 1, 8, 1);

MCU_VU vuA(0, 8, false);
MCU_VU vuB(1, 8, false);
VUPeak vuPA(vuA);
VUPeak vuPB(vuB);

MCU_VPot_Ring ringA(0, 8);
MCU_VPot_Ring ringB(1, 8);

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
  bank.add(channelButtons, Bank::CHANGE_ADDRESS);

  bank.add(encoder, Bank::CHANGE_ADDRESS);
  bank.add(ringA, Bank::CHANGE_ADDRESS);
  bank.add(vuA, Bank::CHANGE_ADDRESS);
  bank.add(muteA, Bank::CHANGE_ADDRESS);
  bank.add(soloA, Bank::CHANGE_ADDRESS);
  bank.add(recrdyA, Bank::CHANGE_ADDRESS);

  bank.add(ringB, Bank::CHANGE_ADDRESS);
  bank.add(vuB, Bank::CHANGE_ADDRESS);
  bank.add(muteB, Bank::CHANGE_ADDRESS);
  bank.add(soloB, Bank::CHANGE_ADDRESS);
  bank.add(recrdyB, Bank::CHANGE_ADDRESS);

  MIDI_Controller.begin();

  display.setTextColor(WHITE);
}

void loop() {
  static unsigned long start = 0;
  unsigned int loopTime = millis() - start;
  start = millis();

  display.clearDisplay();

  drawTimeDisplay(display, tdisp, 0, 0);

  if (play.getState())
    display.drawBitmap(100, 0, play_bits, play_width, play_height, WHITE);
  else
    display.drawBitmap(100, 0, pause_bits, pause_width, pause_height, WHITE);

  display.drawLine(1, 8, 126, 8, WHITE);

  display.setTextSize(2);
  display.setCursor(0 , 50);
  display.print(bs.getBankSetting() * 2 + 1);
  display.setCursor(64, 50);
  display.print(bs.getBankSetting() * 2 + 2);

  drawGoodCircle(display,                       16     , 16 + 10, 14);
  drawVPotSegment(display, ringA.getPosition(), 16     , 16 + 10, 12);

  drawGoodCircle(display,                       16 + 64, 16 + 10, 14);
  drawVPotSegment(display, ringB.getPosition(), 16 + 64, 16 + 10, 12);


  drawVUPeak(display, vuPA, 32      + 8, 60, 16, 3);
  drawVUPeak(display, vuPB, 32 + 64 + 8, 60, 16, 3);


  drawCharacter(display, muteA,   'M', 1, 12,      49);
  drawCharacter(display, soloA,   'S', 1, 12,      49);
  drawCharacter(display, recrdyA, 'R', 1, 12 + 16, 49);

  drawCharacter(display, muteB,   'M', 1, 26 + 64, 49);
  drawCharacter(display, soloB,   'S', 1, 26 + 64, 49);
  drawCharacter(display, recrdyB, 'R', 1, 26 + 64, 49);

#ifdef FPS
  display.setTextSize(1);
  display.setCursor(5, 64 - 8);
  display.print(1000 / loopTime);
#endif


  display.display();
  MIDI_Controller.refresh();
}


