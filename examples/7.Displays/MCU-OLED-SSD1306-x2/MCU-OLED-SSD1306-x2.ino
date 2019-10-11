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
// USBDebugMIDI_Interface midi;

// ----------------------------- Display setup ------------------------------ //
// ========================================================================== //

/*
   Instantiate and initialize the SSD1306 OLED display
*/

constexpr uint8_t SCREEN_WIDTH = 128;
constexpr uint8_t SCREEN_HEIGHT = 64;

constexpr int8_t OLED_DC = 17;    // Data/Command pin of the display
constexpr int8_t OLED_reset = -1; // Use the external RC circuit for reset
constexpr int8_t OLED_CS_L = 10;  // Chip Select pin of the left display
constexpr int8_t OLED_CS_R = 18;  // Chip Select pin of the right display

constexpr uint32_t SPI_Frequency = SPI_MAX_SPEED;

// Instantiate the displays
Adafruit_SSD1306 ssd1306Display_L = {
  SCREEN_WIDTH, SCREEN_HEIGHT, &SPI,          OLED_DC,
  OLED_reset,   OLED_CS_L,     SPI_Frequency,
};
// Instantiate the displays
Adafruit_SSD1306 ssd1306Display_R = {
  SCREEN_WIDTH, SCREEN_HEIGHT, &SPI,          OLED_DC,
  OLED_reset,   OLED_CS_R,     SPI_Frequency,
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
    disp.setBuffer(buffer);
    if (!disp.begin())
      FATAL_ERROR(F("SSD1306 allocation failed."), 0x1306);

    // If you override the begin method, remember to call the super class method
    SSD1306_DisplayInterface::begin();
  }

  void drawBackground() override { disp.drawLine(1, 8, 126, 8, WHITE); }

  // We'll use a static buffer to avoid dynamic memory usage, and to allow
  // multiple displays to reuse one single buffer.
  static uint8_t buffer[(SCREEN_WIDTH * SCREEN_HEIGHT + 7) / 8];

} display_L = ssd1306Display_L, display_R = ssd1306Display_R;

uint8_t MySSD1306_DisplayInterface::buffer[];

// ------------------------------- Bank setup ------------------------------- //
// ========================================================================== //

/*
   Create a bank and a bank selector to change its setting.
*/

Bank<2> bank(4); // Create a new bank with four tracks per bank

// Create a new bank selector to control the bank using a push button
IncrementSelector<2> bankselector = {bank, 5};

// -------------------------- MIDI Input Elements --------------------------- //
// ========================================================================== //

/*
   Define all elements that listen for MIDI messages.
*/

// Time display_L keeps track of the bar counter
MCU::TimeDisplay timedisplay = {};

// Play / Record
NoteValue play = {MCU::PLAY};
NoteValue record = {MCU::RECORD};

// Mute
Bankable::NoteValue<2> mute[] = {
  {bank, MCU::MUTE_1},
  {bank, MCU::MUTE_2},
  {bank, MCU::MUTE_3},
  {bank, MCU::MUTE_4},
};

// Solo
Bankable::NoteValue<2> solo[] = {
  {bank, MCU::SOLO_1},
  {bank, MCU::SOLO_2},
  {bank, MCU::SOLO_3},
  {bank, MCU::SOLO_4},
};

NoteValue rudeSolo = {MCU::RUDE_SOLO};

// Record arm / ready
Bankable::NoteValue<2> recrdy[] = {
  {bank, MCU::REC_RDY_1},
  {bank, MCU::REC_RDY_2},
  {bank, MCU::REC_RDY_3},
  {bank, MCU::REC_RDY_4},
};

// VU meters
MCU::Bankable::VU<2> vu[] = {
  {bank, 1, 0},
  {bank, 2, 0},
  {bank, 3, 0},
  {bank, 4, 0},
};

// VPot rings
MCU::Bankable::VPotRing<2> vpot[] = {
  {bank, 1},
  {bank, 2},
  {bank, 3},
  {bank, 4},
};

// ---------------------------- Display Elements ---------------------------- //
// ========================================================================== //

/*
   Define all display_L elements that display_L the state of the input elements.
*/

// Time display_L
MCU::TimeDisplayDisplay timedisplaydisplay = {
  // position (0, 0), font size (1)
  display_L, timedisplay, {0, 0}, 1, WHITE,
};

// Play / Record
NoteBitmapDisplay playDisp = {
  display_L, play, XBM::play_7, {16 + 64, 0}, WHITE,
};
NoteBitmapDisplay recordDisp = {
  display_L, record, XBM::record_7, {26 + 64, 0}, WHITE,
};

// Mute
NoteBitmapDisplay muteDisp[] = {
  {display_L, mute[0], XBM::mute_10B, {14, 50}, WHITE},
  {display_L, mute[1], XBM::mute_10B, {14 + 64, 50}, WHITE},
  {display_R, mute[2], XBM::mute_10B, {14, 50}, WHITE},
  {display_R, mute[3], XBM::mute_10B, {14 + 64, 50}, WHITE},
};

// Solo
NoteBitmapDisplay soloDisp[] = {
  {display_L, solo[0], XBM::solo_10B, {14, 50}, WHITE},
  {display_L, solo[1], XBM::solo_10B, {14 + 64, 50}, WHITE},
  {display_R, solo[2], XBM::solo_10B, {14, 50}, WHITE},
  {display_R, solo[3], XBM::solo_10B, {14 + 64, 50}, WHITE},
};

NoteBitmapDisplay rudeSoloDisp = {
  display_L, rudeSolo, XBM::solo_7, {36 + 64, 0}, WHITE};

// Record arm / ready
NoteBitmapDisplay recrdyDisp[] = {
  {display_L, recrdy[0], XBM::rec_rdy_10B, {14 + 14, 50}, WHITE},
  {display_L, recrdy[1], XBM::rec_rdy_10B, {14 + 14 + 64, 50}, WHITE},
  {display_R, recrdy[2], XBM::rec_rdy_10B, {14 + 14, 50}, WHITE},
  {display_R, recrdy[3], XBM::rec_rdy_10B, {14 + 14 + 64, 50}, WHITE},
};

// VU meters
MCU::VUDisplay vuDisp[] = {
  // position (32+11, 60), width (16), bar height (3) px, bar spacing (1) px
  {display_L, vu[0], {32 + 11, 60}, 16, 3, 1, WHITE},
  {display_L, vu[1], {32 + 11 + 64, 60}, 16, 3, 1, WHITE},
  {display_R, vu[2], {32 + 11, 60}, 16, 3, 1, WHITE},
  {display_R, vu[3], {32 + 11 + 64, 60}, 16, 3, 1, WHITE},
};

// VPot rings
MCU::VPotDisplay vpotDisp[] = {
  // position (0, 10), outer radius (16) px, inner radius (13) px
  {display_L, vpot[0], {0, 10}, 16, 13, WHITE},
  {display_L, vpot[1], {64, 10}, 16, 13, WHITE},
  {display_R, vpot[2], {0, 10}, 16, 13, WHITE},
  {display_R, vpot[3], {64, 10}, 16, 13, WHITE},
};

// Bank seting
BankDisplay bankDisp[] = {
  // first track of the bank (1), position (0, 50), font size (2)
  {display_L, bank, 1, {0, 50}, 2, WHITE},
  {display_L, bank, 2, {64, 50}, 2, WHITE},
  {display_R, bank, 3, {0, 50}, 2, WHITE},
  {display_R, bank, 4, {64, 50}, 2, WHITE},
};

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
