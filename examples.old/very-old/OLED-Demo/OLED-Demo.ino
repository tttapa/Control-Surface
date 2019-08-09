/**
 * @example OLED-Demo.ino
 * 
 * An extensive example demonstrating the use of DisplayElement%s 
 * to display information from the DAW on a small OLED display.
 *        
 * The example displays the cursor time location, play and record status,
 * and for each of the 8 first tracks, it displays:
 * mute, solo, record ready, pan, and VU level meter with peak indicator.
 * 
 * @image html "OLED-Demo.png"
 * 
 * Using a BankSelector, it displays two channels at once, and you can
 * cycle through four banks to display all 8 tracks.
 * 
 * There are two rotary encoders that control the pan of the selected
 * tracks, two mute, solo, record, and select buttons for the tracks, 
 * and a play and record button.
 */

#define DISPLAY_GFX           // Enable display functions of the Control Surface library (requires Adafruit_GFX to be installed)
#include <Control_Surface.h>  // Include the Control Surface library

#include <Adafruit_SSD1306.h>  // Include the library for your specific display (it should inherit from Adafruit_GFX)
#include <Wire.h>              // Include the I²C library for the display


/* ------ Display setup ------ */
/* =========================== */

// #define FLIP_DISPLAY  // Uncomment to flip the display upside down

Adafruit_SSD1306 display;  // Instantiate a display

// Implement the display interface
class SSD1306_DisplayInterface : public DisplayInterface {
  public:
    SSD1306_DisplayInterface(Adafruit_SSD1306 &display)
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

SSD1306_DisplayInterface dispIface(display);  // Instantiate a display interface

void initializeDisplay() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Initialize with the display with I²C address 0x3C
  Wire.setClock(1800000);  // Set the I²C clock to 1.8 MHz (set F_CPU to 96 MHz)
#ifdef FLIP_DISPLAY
  display.setRotation(2);
#endif
  display.clearDisplay();
  display.display();
}


/* ------- Bank setup -------- */
/* =========================== */

Bank bank(2);  // Create a new bank with two tracks per bank (see Ex.06.Banks)

BankSelector bs(bank, { 4, 5 }, 4);  // Create a new bank selector with four bank settings 
                                     // and an increment button on pin 4 and a decrement button on pin 5
                                     // (see Ex.07.BankSelectors)

/* -- MIDI Control Elements -- */
/* =========================== */

using namespace MCU; // Use MCU namespace for constants

// Instantiate the buttons (see Ex.02.Button)
Digital channelButtons[] = {
  {6,  MUTE_1,         1, 127},
  {7,  SOLO_1,         1, 127},
  {8,  REC_RDY_1,      1, 127},
  {9,  V_POT_SELECT_1, 1, 127},  
  {10, MUTE_2,         1, 127},
  {11, SOLO_2,         1, 127},
  {12, V_POT_SELECT_2, 1, 127},
};

Digital playButton   (14, PLAY,   1, 127);
Digital recordButton (15, RECORD, 1, 127);

RotaryEncoder encoder_A (0, 1, V_POT_1, 1, 1, NORMAL_ENCODER, MACKIE_CONTROL_RELATIVE);
RotaryEncoder encoder_B (2, 3, V_POT_2, 1, 1, NORMAL_ENCODER, MACKIE_CONTROL_RELATIVE);


/* --- MIDI Input Elements --- */
/* =========================== */

// Time display
MCU_TimeDisplay tdisp;

// Play / Record
MIDIInputElementNote_Buffer play   (PLAY,   1, 1, 1); // channel (1), listen to this address only (1), listen to this channel only (1)
MIDIInputElementNote_Buffer record (RECORD, 1, 1, 1);

// Mute
MIDIInputElementNote_Buffer mute_A (MUTE_1, 1, 4, 1); // channel (1), listen to (4) addresses in total, listen to this channel only (1)
MIDIInputElementNote_Buffer mute_B (MUTE_2, 1, 4, 1);

// Solo
MIDIInputElementNote_Buffer solo_A (SOLO_1, 1, 4, 1);
MIDIInputElementNote_Buffer solo_B (SOLO_2, 1, 4, 1);

MIDIInputElementNote_Buffer rudeSolo (RUDE_SOLO, 1, 1, 1);

// Record arm / ready
MIDIInputElementNote_Buffer recrdy_A (REC_RDY_1, 1, 4, 1);
MIDIInputElementNote_Buffer recrdy_B (REC_RDY_2, 1, 4, 1);

// VU meters
MCU_VU vu_A (1, 4, false);  // track (1), listen to (4) tracks in total, don't decay automatically (false)
MCU_VU vu_B (2, 4, false);

// VPot rings
MCU_VPot_Ring ring_A (1, 4);  // track (1), listen to (4) tracks in total
MCU_VPot_Ring ring_B (2, 4);


/* ---- Display Elements ----- */
/* =========================== */

// Time display
TimeDisplay timeDisp(display, tdisp, {0, 0}, 1, WHITE); // position (0, 0), font size (1)

// Play / Record
NoteDisplay playDisp   (display, play,   XBM::play_7,   {16 + 64, 0}, WHITE);
NoteDisplay recordDisp (display, record, XBM::record_7, {26 + 64, 0}, WHITE);

// Mute
NoteDisplay muteDisp_A (display, mute_A, XBM::mute_10B, {14,      50}, WHITE);
NoteDisplay muteDisp_B (display, mute_B, XBM::mute_10B, {14 + 64, 50}, WHITE);

// Solo
NoteDisplay soloDisp_A (display, solo_A, XBM::solo_10B, {14,      50}, WHITE);
NoteDisplay soloDisp_B (display, solo_B, XBM::solo_10B, {14 + 64, 50}, WHITE);

NoteDisplay rudeSoloDisp (display, rudeSolo, XBM::solo_7, {36 + 64, 0}, WHITE);

// Record arm / ready
NoteDisplay recrdyDisp_A (display, recrdy_A, XBM::rec_rdy_10B, {14 + 14,      50}, WHITE);
NoteDisplay recrdyDisp_B (display, recrdy_B, XBM::rec_rdy_10B, {14 + 14 + 64, 50}, WHITE);

// VU meters
VUDisplay vuDisp_A (display, vu_A, {32 + 11,      60}, 16, 3, 1, WHITE);  // position (32+11, 60), width (16), bar height (3) px, bar spacing (1) px
VUDisplay vuDisp_B (display, vu_B, {32 + 11 + 64, 60}, 16, 3, 1, WHITE);

// VPot rings
VPotDisplay vpotDisp_A (display, ring_A, {0,  10}, 16, 13, WHITE);  // position (0, 10), outer radius (16) px, inner radius (13) px
VPotDisplay vpotDisp_B (display, ring_B, {64, 10}, 16, 13, WHITE);

// Bank selector
SelectorDisplay bsDisp_A (display, bs, 1, 2, {0,  50}, 2, WHITE);  // first track of the bank (1), two tracks per bank (2), position (0, 50), font size (2)
SelectorDisplay bsDisp_B (display, bs, 2, 2, {64, 50}, 2, WHITE);


/* ---------- Setup ---------- */
/* =========================== */

void setup()   {
  // Add all channel input and control elements to the bank
  
  bank.add(channelButtons, Bank::CHANGE_ADDRESS);
  
  bank.add(encoder_A,      Bank::CHANGE_ADDRESS);
  bank.add(encoder_B,      Bank::CHANGE_ADDRESS);

  bank.add(ring_A,         Bank::CHANGE_ADDRESS);
  bank.add(vu_A,           Bank::CHANGE_ADDRESS);
  bank.add(mute_A,         Bank::CHANGE_ADDRESS);
  bank.add(solo_A,         Bank::CHANGE_ADDRESS);
  bank.add(recrdy_A,       Bank::CHANGE_ADDRESS);

  bank.add(ring_B,         Bank::CHANGE_ADDRESS);
  bank.add(vu_B,           Bank::CHANGE_ADDRESS);
  bank.add(mute_B,         Bank::CHANGE_ADDRESS);
  bank.add(solo_B,         Bank::CHANGE_ADDRESS);
  bank.add(recrdy_B,       Bank::CHANGE_ADDRESS);

  initializeDisplay();  // Start the OLED display

  Control_Surface.begin();  // Initialize Control Surface
}

/* ---------- Loop ----------- */
/* =========================== */

void loop() {
  Control_Surface.refresh();  // Refresh all elements
}
