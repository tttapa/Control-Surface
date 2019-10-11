/**
   @example OLED-Demo.ino
   @brief An extensive example demonstrating the use of DisplayElement%s
          to display information from the DAW on a small OLED display.

   The example displays the cursor time location, play and record status,
   and for each of the 8 first tracks, it displays:
   mute, solo, record ready, pan, and VU level meter with peak indicator.

   Using a BankSelector, it displays two channels at once, and you can
   cycle through four banks to display all 8 tracks.

   There are two rotary encoders that control the pan of the selected
   tracks, two mute, solo, record, and select buttons for the tracks,
   and a play and record button.
*/

#include <Encoder.h> // Include the Encoder library.
// This must be done before the Control Surface library.
#include <Control_Surface.h> // Include the Control Surface library
// Include the display interface you'd like to use
#include <Display/DisplayInterfaces/DisplayInterfaceSSD1306.hpp>

// ----------------------------- MIDI Interface ----------------------------- //
// ========================================================================== //

/*
   Instantiate a MIDI interface to use for the Control Surface.
*/

USBMIDI_Interface midi;
// USBDebugMIDI_Interface midi(115200);

// ----------------------------- Display setup ------------------------------ //
// ========================================================================== //

/*
   Instantiate and initialize the SSD1306 OLED display
*/

constexpr uint8_t SCREEN_WIDTH = 128;
constexpr uint8_t SCREEN_HEIGHT = 64;

constexpr int8_t OLED_DC = 17;    // Data/Command pin of the display
constexpr int8_t OLED_reset = -1; // Use the external RC circuit for reset
constexpr int8_t OLED_CS = 10;    // Chip Select pin of the display

constexpr uint32_t SPI_Frequency = SPI_MAX_SPEED;

// Instantiate the displays
Adafruit_SSD1306 ssd1306Display = {
  SCREEN_WIDTH, SCREEN_HEIGHT, &SPI,          OLED_DC,
  OLED_reset,   OLED_CS,       SPI_Frequency,
};

// --------------------------- Display interface ---------------------------- //
// ========================================================================== //

// Implement the display interface, specifically, the begin and drawBackground
// methods.
class MySSD1306_DisplayInterface : public SSD1306_DisplayInterface {
 public:
  MySSD1306_DisplayInterface(Adafruit_SSD1306 &display)
    : SSD1306_DisplayInterface(display) {}

  void begin() override {
    // Initialize the Adafruit_SSD1306 display
    if (!disp.begin())
      FATAL_ERROR(F("SSD1306 allocation failed."), 0x1306);

    // If you override the begin method, remember to call the super class method
    SSD1306_DisplayInterface::begin();
  }

  void drawBackground() override { disp.drawLine(1, 8, 126, 8, WHITE); }

} display = ssd1306Display;

// ------------------------------- Bank setup ------------------------------- //
// ========================================================================== //

/*
   Create a bank and a bank selector to change its setting.
*/

Bank<4> bank(2); // Create a new bank with two tracks per bank

// Create a new bank selector to control the bank using two push buttons
IncrementDecrementSelector<4> bankselector = {bank, {5, 6}, Wrap::Wrap};

// -------------------------- MIDI Input Elements --------------------------- //
// ========================================================================== //

/*
   Define all elements that listen for MIDI messages.
*/

// Time display keeps track of the bar counter
MCU::TimeDisplay timedisplay = {};

// Play / Record
NoteValue play = {MCU::PLAY};
NoteValue record = {MCU::RECORD};

// Mute
Bankable::NoteValue<4> mute_A = {bank, MCU::MUTE_1};
Bankable::NoteValue<4> mute_B = {bank, MCU::MUTE_2};

// Solo
Bankable::NoteValue<4> solo_A = {bank, MCU::SOLO_1};
Bankable::NoteValue<4> solo_B = {bank, MCU::SOLO_2};

NoteValue rudeSolo = {MCU::RUDE_SOLO};

// Record arm / ready
Bankable::NoteValue<4> recrdy_A = {bank, MCU::REC_RDY_1};
Bankable::NoteValue<4> recrdy_B = {bank, MCU::REC_RDY_2};

// VU meters
MCU::Bankable::VU<4> vu_A = {bank, 1, 0};
MCU::Bankable::VU<4> vu_B = {bank, 2, 0};

// VPot rings
MCU::Bankable::VPotRing<4> vpot_A = {bank, 1};
MCU::Bankable::VPotRing<4> vpot_B = {bank, 2};

// ---------------------------- Display Elements ---------------------------- //
// ========================================================================== //

/*
   Define all display elements that display the state of the input elements.
*/

// Time display
MCU::TimeDisplayDisplay timedisplaydisplay = {
  display, timedisplay, {0, 0}, 1, WHITE};
// position (0, 0), font size (1)

// Play / Record
NoteBitmapDisplay playDisp = {display, play, XBM::play_7, {16 + 64, 0}, WHITE};
NoteBitmapDisplay recordDisp = {
  display, record, XBM::record_7, {26 + 64, 0}, WHITE};

// Mute
NoteBitmapDisplay muteDisp_A = {
  display, mute_A, XBM::mute_10B, {14, 50}, WHITE};
NoteBitmapDisplay muteDisp_B = {
  display, mute_B, XBM::mute_10B, {14 + 64, 50}, WHITE};

// Solo
NoteBitmapDisplay soloDisp_A = {
  display, solo_A, XBM::solo_10B, {14, 50}, WHITE};
NoteBitmapDisplay soloDisp_B = {
  display, solo_B, XBM::solo_10B, {14 + 64, 50}, WHITE};

NoteBitmapDisplay rudeSoloDisp = {
  display, rudeSolo, XBM::solo_7, {36 + 64, 0}, WHITE};

// Record arm / ready
NoteBitmapDisplay recrdyDisp_A = {
  display, recrdy_A, XBM::rec_rdy_10B, {14 + 14, 50}, WHITE};
NoteBitmapDisplay recrdyDisp_B = {
  display, recrdy_B, XBM::rec_rdy_10B, {14 + 14 + 64, 50}, WHITE};

// VU meters
MCU::VUDisplay vuDisp_A = {display, vu_A, {32 + 11, 60}, 16, 3, 1, WHITE};
// position (32+11, 60), width (16), bar height (3) px, bar spacing (1) px
MCU::VUDisplay vuDisp_B = {display, vu_B, {32 + 11 + 64, 60}, 16, 3, 1, WHITE};

// VPot rings
MCU::VPotDisplay vpotDisp_A = {display, vpot_A, {0, 10}, 16, 13, WHITE};
// position (0, 10), outer radius (16) px, inner radius (13) px
MCU::VPotDisplay vpotDisp_B = {display, vpot_B, {64, 10}, 16, 13, WHITE};

// Bank selector
BankDisplay bsDisp_A = {display, bank, 1, {0, 50}, 2, WHITE};
// first track of the bank (1), position (0, 50), font size (2)
BankDisplay bsDisp_B = {display, bank, 2, {64, 50}, 2, WHITE};

// --------------------------------- Setup ---------------------------------- //
// ========================================================================== //

void setup() {
  // The default SPI MOSI pin (11) is used for I²S, so we need to use the
  // alternative MOSI pin (7)
  SPI.setMOSI(7);
  // Correct relative mode for MCU rotary encoders
  RelativeCCSender::setMode(MACKIE_CONTROL_RELATIVE);
  Control_Surface.begin(); // Initialize Control Surface
}

// ---------------------------------- Loop ---------------------------------- //
// ========================================================================== //

void loop() {
  Control_Surface.loop(); // Refresh all elements
}
