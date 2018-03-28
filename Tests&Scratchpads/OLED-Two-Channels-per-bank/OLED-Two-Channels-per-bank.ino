#define DISPLAY_GFX
#include <Control_Surface.h>

#include <Adafruit_SSD1306.h>
#include <Wire.h>

using namespace MCU;
using namespace ExtIO;

#define FPS
// #define SERIAL_FPS

#define DEBUG_MIDI
// #define SERIAL_MIDI

Adafruit_SSD1306 display;

class SSD1306_Display : public Display {
  public:
    SSD1306_Display(Adafruit_SSD1306 &display)
      : disp(display) {}
    void display() {
      disp.display();
    }
    void clearDisplay() {
      disp.clearDisplay();
      disp.drawLine(1, 8, 126, 8, WHITE);
    }
  private:
    Adafruit_SSD1306 &disp;
};

SSD1306_Display d(display);

const uint8_t clockPin = 10;
const uint8_t latchPin = 11;
const uint8_t dataPin = 12;

const uint8_t blinkPin = LED_BUILTIN;

const unsigned long blinkInterval = 500;

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
  {2, MUTE_1, 1, 127},
  {3, SOLO_1, 1, 127},
  {4, REC_RDY_1, 1, 127},
  {8, MUTE_2, 1, 127},
  {9, SOLO_2, 1, 127},
};

Digital playButton =   {7, PLAY, 1, 127};

RotaryEncoder encoder(0, 1, V_POT_1, 1, 1, NORMAL_ENCODER, MACKIE_CONTROL_RELATIVE);

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

// Time display
MCU_TimeDisplay tdisp;

// Play / Record
MIDI_Input_Note_Buffer play(PLAY, 1, 1, 1);
MIDI_Input_Note_Buffer record(RECORD, 1, 1, 1);

NoteDisplay playDisp  (display, play,   XBM::play7,  16 + 64, 0, WHITE);
NoteDisplay recordDisp(display, record, XBM::record7, 26 + 64, 0, WHITE);

// Mute
MIDI_LED muteA(SR_BS.red(7), MUTE_1, 1, 4, 1);
MIDI_LED muteB(SR_BS.red(6), MUTE_2, 1, 4, 1);

NoteDisplay muteDispA(display, muteA, XBM::mute10, 14,      50, WHITE);
NoteDisplay muteDispB(display, muteB, XBM::mute10, 14 + 64, 50, WHITE);

// Solo
MIDI_LED soloA(SR_BS.green(7), SOLO_1, 1, 4, 1);
MIDI_LED soloB(SR_BS.green(6), SOLO_2, 1, 4, 1);

MIDI_Input_Note_Buffer rudeSolo(RUDE_SOLO, 1, 1, 1);

NoteDisplay soloDispA(display, soloA, XBM::solo10, 14,      50, WHITE);
NoteDisplay soloDispB(display, soloB, XBM::solo10, 14 + 64, 50, WHITE);

NoteDisplay rudeSoloDisp(display, rudeSolo, XBM::solo7, 36 + 64, 0, WHITE);

// Record arm / ready
MIDI_LED recrdyA(SR_BS.red(5), REC_RDY_1, 1, 4, 1);
MIDI_LED recrdyB(SR_BS.red(4), REC_RDY_2, 1, 4, 1);

NoteDisplay recrdyDispA(display, recrdyA, XBM::recordRdy10, 14 + 14,      50, WHITE);
NoteDisplay recrdyDispB(display, recrdyB, XBM::recordRdy10, 14 + 14 + 64, 50, WHITE);

// VU meters
MCU_VU vuA(1, 4, false, 150);
MCU_VU vuB(2, 4, false, 150);

VUDisplay vuDisp_A(display, vuA, 32 + 11,      60, 16, 3, 1, WHITE);
VUDisplay vuDisp_B(display, vuB, 32 + 11 + 64, 60, 16, 3, 1, WHITE);

// VPot rings
MCU_VPot_Ring ringA(1, 4);
MCU_VPot_Ring ringB(2, 4);

VPotDisplay vpotDisp_A(display, ringA, 0,  10, 16, 14, WHITE);
VPotDisplay vpotDisp_B(display, ringB, 64, 10, 16, 14, WHITE);

// Bank selector
SelectorDisplay bsA(display, bs, 1, 2, 0,  50, 2, WHITE); // first track of the bank (1), two tracks per bank (2), location (0, 50), font size (2)
SelectorDisplay bsB(display, bs, 2, 2, 64, 50, 2, WHITE); // second track of the bank (2), two tracks per bank (2), location (64, 50), font size (2)

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void setup()   {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C
  Wire.setClock(1800000); // 1.8 MHz (set F_CPU to 96 MHz)
  display.setRotation(2);
  display.clearDisplay();
  display.display();
#ifdef DEBUG_MIDI
  while (!Serial);
  delay(500);
#endif

  pinMode(blinkPin, OUTPUT);

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

  Control_Surface.begin();

  display.setTextColor(WHITE);
}

void loop() {
  static unsigned long previousBlink = millis();

  if (millis() - previousBlink >= blinkInterval) {
    digitalWrite(blinkPin, !digitalRead(blinkPin));
    previousBlink += blinkInterval;
  }

  static unsigned long start = 0;
  unsigned int loopTime = millis() - start;
  start = millis();
  static EMA64<4> averageLoopTime;
  unsigned int avglt = averageLoopTime.filter(loopTime);

  // display.clearDisplay();

  // drawTimeDisplay(display, tdisp, 0, 0);

  // display.drawLine(1, 8, 126, 8, WHITE);

  // display.setTextSize(2);
  // display.setCursor(0 , 50);
  // display.print(bs.getSetting() * 2 + 1);
  // display.setCursor(64, 50);
  // display.print(bs.getSetting() * 2 + 2);


  /*
    vpotDisp_A.draw();
    vpotDisp_B.draw();

    vuDisp_A.draw();
    vuDisp_B.draw();

    muteDispA.draw();
    muteDispB.draw();

    soloDispA.draw();
    soloDispB.draw();

    rudeSoloDisp.draw();

    playDisp.draw();
    recordDisp.draw();

    recrdyDispA.draw();
    recrdyDispB.draw();
  */

/*
#ifdef FPS
  display.setTextSize(1);
  display.setCursor(115, 0);
  display.print(1000 / avglt);
#endif
#ifdef SERIAL_FPS
  Serial.println(1000 / avglt);
  Serial.flush();
#endif
*/

  // display.display();
  Control_Surface.refresh();

  //  delay(100);
}

