/**
 * An example demonstrating the use of DisplayElement%s to display information
 * from the DAW on a small OLED display.
 *
 * @boards  Mega, Teensy 3.x, ESP32
 * 
 * Connections
 * -----------
 * 
 * - 5:  Push button (to ground)
 * - 6:  Push button (to ground)
 * - 7:  OLED Data/D1 (SPI MOSI)
 * - 13: OLED Clock/D0 (SPI SCK)
 * - 17: OLED Data/Command
 * - 10: OLED Cable Select
 * 
 * Add a capacitor between the reset pin of the display and ground, and a 
 * resistor from reset to 3.3V. The values are not critical, 0.1µF and 10kΩ 
 * work fine.  
 * You do need some way to reset the display, without it, it won't work.  
 * Alternatively, you could use an IO pin from the Teensy to reset the 
 * display, but this just "wastes" a pin.
 * 
 * Behavior
 * --------
 * 
 * - The time (bars, beats, fraction), play and record status are shown at the 
 *   top of the display.
 * - For each of the 8 first tracks, a VU level meter with peak indicator and
 *   a V-Pot ring showing the pan are displayed, as well as the the mute, solo 
 *   and record arm status.
 * - Two tracks are displayed at once. By pressing the push buttons connected
 *   to pins 5 and 6, you can cycle through four banks to display all 8 tracks.
 * 
 * Mapping
 * -------
 * 
 * Map "Control Surface" as a Mackie Control Universal unit in your DAW.
 * 
 * @note    There seem to be some differences in the way some applications 
 *          handle VU meters: some expect the hardware to decay automatically,
 *          some don't.  
 *          If you notice that the meters behave strangely, try both 
 *          MCU::VUDecay::Hold and MCU::VUDecay::Default, or try a different 
 *          decay time.
 * 
 * Written by PieterP, 2019-11-12  
 * https://github.com/tttapa/Control-Surface
 */

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
Adafruit_SSD1306 ssd1306Display {
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
IncrementDecrementSelector<4> bankselector {bank, {5, 6}, Wrap::Wrap};

// -------------------------- MIDI Input Elements --------------------------- //
// ========================================================================== //

/*
   Define all elements that listen for MIDI messages.
*/

// Time display keeps track of the bar counter
MCU::TimeDisplay timedisplay {};

// Play / Record
NoteValue play {MCU::PLAY};
NoteValue record {MCU::RECORD};

// Mute
Bankable::NoteValue<4> mute[] {
  {bank, MCU::MUTE_1},
  {bank, MCU::MUTE_2},
};

// Solo
Bankable::NoteValue<4> solo[] {
  {bank, MCU::SOLO_1},
  {bank, MCU::SOLO_2},
};

NoteValue rudeSolo {MCU::RUDE_SOLO};

// Record arm / ready
Bankable::NoteValue<4> recrdy[] {
  {bank, MCU::REC_RDY_1},
  {bank, MCU::REC_RDY_2},
};

// VU meters
MCU::Bankable::VU<4> vu[] {
  {bank, 1, MCU::VUDecay::Hold},
  {bank, 2, MCU::VUDecay::Hold},
};

// VPot rings
MCU::Bankable::VPotRing<4> vpot[] {
  {bank, 1},
  {bank, 2},
};

// ---------------------------- Display Elements ---------------------------- //
// ========================================================================== //

/*
   Define all display elements that display the state of the input elements.
*/

// Time display
MCU::TimeDisplayDisplay timedisplaydisplay {
  // position (0, 0), font size (1)
  display, timedisplay, {0, 0}, 1, WHITE,
};

// Play / Record
BitmapDisplay<> playDisp {
  display, play, XBM::play_7, {16 + 64, 0}, WHITE,
};
BitmapDisplay<> recordDisp {
  display, record, XBM::record_7, {26 + 64, 0}, WHITE,
};

// Mute
BitmapDisplay<> muteDisp[] {
  {display, mute[0], XBM::mute_10B, {14, 50}, WHITE},
  {display, mute[1], XBM::mute_10B, {14 + 64, 50}, WHITE},
};

// Solo
BitmapDisplay<> soloDisp[] {
  {display, solo[0], XBM::solo_10B, {14, 50}, WHITE},
  {display, solo[1], XBM::solo_10B, {14 + 64, 50}, WHITE},
};

BitmapDisplay<> rudeSoloDisp {
  display, rudeSolo, XBM::solo_7, {36 + 64, 0}, WHITE};

// Record arm / ready
BitmapDisplay<> recrdyDisp[] {
  {display, recrdy[0], XBM::rec_rdy_10B, {14 + 14, 50}, WHITE},
  {display, recrdy[1], XBM::rec_rdy_10B, {14 + 14 + 64, 50}, WHITE},
};

// VU meters
MCU::VUDisplay<> vuDisp[] {
  // position (32+11, 60), width (16), bar height (3) px, bar spacing (1) px
  {display, vu[0], {32 + 11, 60}, 16, 3, 1, WHITE},
  {display, vu[1], {32 + 11 + 64, 60}, 16, 3, 1, WHITE},
};

// VPot rings
MCU::VPotDisplay<> vpotDisp[] {
  // position (0, 10), outer radius (16) px, inner radius (13) px
  {display, vpot[0], {0, 10}, 16, 13, WHITE},
  {display, vpot[1], {64, 10}, 16, 13, WHITE},
};

// Bank seting
BankDisplay bankDisp[] {
  // first track of the bank (1), position (0, 50), font size (2)
  {display, bank, 1, {0, 50}, 2, WHITE},
  {display, bank, 2, {64, 50}, 2, WHITE},
};

// --------------------------------- Setup ---------------------------------- //
// ========================================================================== //

void setup() {
  // Correct relative mode for MCU rotary encoders
  RelativeCCSender::setMode(MACKIE_CONTROL_RELATIVE);
  Control_Surface.begin(); // Initialize Control Surface
}

// ---------------------------------- Loop ---------------------------------- //
// ========================================================================== //

void loop() {
  Control_Surface.loop(); // Refresh all elements
}
