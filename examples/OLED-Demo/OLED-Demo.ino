#define DISPLAY_GFX // Enable display functions of the Control Surface library (requires Adafruit_GFX to be installed)
#include <Control_Surface.h>

#include <Adafruit_SSD1306.h> // Include the library for your specific display (it should inherit from Adafruit_GFX)
#include <Wire.h> // Include the I²C library for the display

// use namespaces for constants
using namespace MCU;
using namespace ExtIO;

// #define DEBUG_MIDI
// #define SERIAL_MIDI

// #define FLIP_DISPLAY

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

Adafruit_SSD1306 display; // Instantiate a display

// Implement the display interface
class SSD1306_Display : public DisplayInterface {
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

SSD1306_Display dispIface(display); // Instantiate a display interface

void initializeDisplay() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Initialize with the display with I²C address 0x3C
  Wire.setClock(1800000); // Set the I²C clock to 1.8 MHz (set F_CPU to 96 MHz)
#ifdef FLIP_DISPLAY
  display.setRotation(2);
#endif
  display.clearDisplay();
  display.display();
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

#ifdef DEBUG_MIDI
USBDebugMIDI_Interface dbg(115200); // Instantiate a USB Debug output
#elif defined (SERIAL_MIDI)
USBSerialMIDI_Interface smidi(115200);
#else
USBMIDI_Interface midi; // Instantiate a USB MIDI output
#endif

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

Bank bank(2); // two channels per bank

BankSelector bs(bank, { 6, 5 }, 4);

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

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
MIDI_Input_Note_Buffer muteA(MUTE_1, 1, 4, 1);
MIDI_Input_Note_Buffer muteB(MUTE_2, 1, 4, 1);

NoteDisplay muteDispA(display, muteA, XBM::mute10, 14,      50, WHITE);
NoteDisplay muteDispB(display, muteB, XBM::mute10, 14 + 64, 50, WHITE);

// Solo
MIDI_Input_Note_Buffer soloA(SOLO_1, 1, 4, 1);
MIDI_Input_Note_Buffer soloB(SOLO_2, 1, 4, 1);

MIDI_Input_Note_Buffer rudeSolo(RUDE_SOLO, 1, 1, 1);

NoteDisplay soloDispA(display, soloA, XBM::solo10, 14,      50, WHITE);
NoteDisplay soloDispB(display, soloB, XBM::solo10, 14 + 64, 50, WHITE);

NoteDisplay rudeSoloDisp(display, rudeSolo, XBM::solo7, 36 + 64, 0, WHITE);

// Record arm / ready
MIDI_Input_Note_Buffer recrdyA(REC_RDY_1, 1, 4, 1);
MIDI_Input_Note_Buffer recrdyB(REC_RDY_2, 1, 4, 1);

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

// Time display
TimeDisplay timeDisp(display, tdisp, 0, 0, 1, WHITE);

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void setup()   {
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

  initializeDisplay();

#ifdef DEBUG_MIDI
  while (!Serial);
  delay(100);
#endif

  Control_Surface.begin();
}

void loop() {
  Control_Surface.refresh();
}

