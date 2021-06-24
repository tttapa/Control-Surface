/**
 * An example demonstrating the use of DisplayElement%s to display information
 * from the DAW on two small OLED displays.
 *
 * @boards  Mega, Teensy 3.x, ESP32
 * 
 * Connections
 * -----------
 * 
 * - 5:  Push button (to ground)
 * - 11: OLED Data/D1 (SPI MOSI)
 * - 13: OLED Clock/D0 (SPI SCK)
 * - 17: OLED Data/Command
 * - 10: Left OLED Cable Select
 * - 18: Right OLED Cable Select
 * 
 * Add a capacitor between the reset pins of the displays and ground, and a 
 * resistor from reset to 3.3V. The values are not critical, 0.1µF and 10kΩ 
 * work fine.  
 * You do need some way to reset the displays, without it, it won't work.  
 * Alternatively, you could use an IO pin from the Teensy to reset the 
 * displays, but this just "wastes" a pin.
 * 
 * Behavior
 * --------
 * 
 * - The time (bars, beats, fraction), play and record status are shown at the 
 *   top of the display.
 * - For each of the 8 first tracks, a VU level meter with peak indicator and
 *   a V-Pot ring showing the pan are displayed, as well as the the mute, solo 
 *   and record arm status.
 * - Four tracks are displayed at once. By pressing the push button connected
 *   to pin 5, you can switch between two banks to display all 8 tracks.
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
 * Demo
 * ----
 * 
 * @htmlonly
 * <iframe width="560" height="315"
 * src="https://www.youtube.com/embed/upL609Vkzug" frameborder="0"
 * allow="accelerometer; autoplay; encrypted-media; gyroscope;
 * picture-in-picture" allowfullscreen></iframe>
 * @endhtmlonly
 * 
 * Written by PieterP, 2019-10-12  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h> // Include the Control Surface library
// Include the display interface you'd like to use
#include <Display/DisplayInterfaces/DisplayInterfaceSSD1306.hpp>

// ----------------------------- MIDI Interface ----------------------------- //
// ========================================================================== //

/*
 * Instantiate a MIDI interface to use for the Control Surface.
 */

USBMIDI_Interface midi;

// ----------------------------- Display setup ------------------------------ //
// ========================================================================== //

/*
 * Instantiate and initialize the SSD1306 OLED display
 */

constexpr uint8_t SCREEN_WIDTH = 128;
constexpr uint8_t SCREEN_HEIGHT = 64;

constexpr int8_t OLED_DC = 17;    // Data/Command pin of the display
constexpr int8_t OLED_reset = -1; // Use the external RC circuit for reset
constexpr int8_t OLED_CS_L = 10;  // Chip Select pin of the left display
constexpr int8_t OLED_CS_R = 18;  // Chip Select pin of the right display

constexpr uint32_t SPI_Frequency = SPI_MAX_SPEED;

// Instantiate the displays
Adafruit_SSD1306 ssd1306Display_L {
  SCREEN_WIDTH, SCREEN_HEIGHT, &SPI,          OLED_DC,
  OLED_reset,   OLED_CS_L,     SPI_Frequency,
};
Adafruit_SSD1306 ssd1306Display_R {
  SCREEN_WIDTH, SCREEN_HEIGHT, &SPI,          OLED_DC,
  OLED_reset,   OLED_CS_R,     SPI_Frequency,
};

// --------------------------- Display interface ---------------------------- //
// ========================================================================== //

#if defined(ADAFRUIT_SSD1306_HAS_SETBUFFER) && ADAFRUIT_SSD1306_HAS_SETBUFFER
  // We'll use a static buffer to avoid dynamic memory usage, and to allow
  // multiple displays to reuse one single buffer.
  static uint8_t buffer[(SCREEN_WIDTH * SCREEN_HEIGHT + 7) / 8];
#endif

// Implement the display interface, specifically, the begin and drawBackground
// methods.
class MySSD1306_DisplayInterface : public SSD1306_DisplayInterface {
 public:
  MySSD1306_DisplayInterface(Adafruit_SSD1306 &display)
    : SSD1306_DisplayInterface(display) {}

  void begin() override {
#if defined(ADAFRUIT_SSD1306_HAS_SETBUFFER) && ADAFRUIT_SSD1306_HAS_SETBUFFER
    disp.setBuffer(buffer);
#endif
    // Initialize the Adafruit_SSD1306 display
    if (!disp.begin())
      FATAL_ERROR(F("SSD1306 initialization failed."), 0x1306);

    // If you override the begin method, remember to call the super class method
    SSD1306_DisplayInterface::begin();
  }

  void drawBackground() override { disp.drawLine(1, 8, 126, 8, WHITE); }

} display_L = ssd1306Display_L, display_R = ssd1306Display_R;

// ------------------------------- Bank setup ------------------------------- //
// ========================================================================== //

/*
 * Create a bank and a bank selector to change its setting.
 */

Bank<2> bank(4); // Create a new bank with four tracks per bank

// Create a new bank selector to control the bank using a push button
IncrementSelector<2> bankselector {bank, 5};

// -------------------------- MIDI Input Elements --------------------------- //
// ========================================================================== //

/*
 * Define all elements that listen for MIDI messages.
 */

// Main MCU LCD screen, used to get track names
MCU::LCD<> lcd {};

// Time display_L keeps track of the bar counter
MCU::TimeDisplay timedisplay {};

// Play / Record
NoteValue play {MCU::PLAY};
NoteValue record {MCU::RECORD};

// Mute
Bankable::NoteValue<2> mute[] {
  {bank, MCU::MUTE_1},
  {bank, MCU::MUTE_2},
  {bank, MCU::MUTE_3},
  {bank, MCU::MUTE_4},
};

// Solo
Bankable::NoteValue<2> solo[] {
  {bank, MCU::SOLO_1},
  {bank, MCU::SOLO_2},
  {bank, MCU::SOLO_3},
  {bank, MCU::SOLO_4},
};

NoteValue rudeSolo {MCU::RUDE_SOLO};

// Record arm / ready
Bankable::NoteValue<2> recrdy[] {
  {bank, MCU::REC_RDY_1},
  {bank, MCU::REC_RDY_2},
  {bank, MCU::REC_RDY_3},
  {bank, MCU::REC_RDY_4},
};

// VU meters
MCU::Bankable::VU<2> vu[] {
  {bank, 1, MCU::VUDecay::Hold},
  {bank, 2, MCU::VUDecay::Hold},
  {bank, 3, MCU::VUDecay::Hold},
  {bank, 4, MCU::VUDecay::Hold},
};

// VPot rings
MCU::Bankable::VPotRing<2> vpot[] {
  {bank, 1},
  {bank, 2},
  {bank, 3},
  {bank, 4},
};

// ---------------------------- Display Elements ---------------------------- //
// ========================================================================== //

/*
 * Define all display_L elements that display_L the state of the input elements.
 */

// Track names
MCU::LCDDisplay lcddisps[] {
  // track (1), position (0, 40), font size (1)
  {display_L, lcd, bank, 1, {0, 40}, 1, WHITE},
  {display_L, lcd, bank, 2, {64, 40}, 1, WHITE},
  {display_R, lcd, bank, 3, {0, 40}, 1, WHITE},
  {display_R, lcd, bank, 4, {64, 40}, 1, WHITE},
};

// Time display
MCU::TimeDisplayDisplay timedisplaydisplay {
  // position (0, 0), font size (1)
  display_L, timedisplay, {0, 0}, 1, WHITE,
};

// Play / Record
BitmapDisplay<> playDisp {
  display_L, play, XBM::play_7, {16 + 64, 0}, WHITE,
};
BitmapDisplay<> recordDisp {
  display_L, record, XBM::record_7, {26 + 64, 0}, WHITE,
};

// Mute
BitmapDisplay<> muteDisp[] {
  {display_L, mute[0], XBM::mute_10B, {14, 50}, WHITE},
  {display_L, mute[1], XBM::mute_10B, {14 + 64, 50}, WHITE},
  {display_R, mute[2], XBM::mute_10B, {14, 50}, WHITE},
  {display_R, mute[3], XBM::mute_10B, {14 + 64, 50}, WHITE},
};

// Solo
BitmapDisplay<> soloDisp[] {
  {display_L, solo[0], XBM::solo_10B, {14, 50}, WHITE},
  {display_L, solo[1], XBM::solo_10B, {14 + 64, 50}, WHITE},
  {display_R, solo[2], XBM::solo_10B, {14, 50}, WHITE},
  {display_R, solo[3], XBM::solo_10B, {14 + 64, 50}, WHITE},
};

BitmapDisplay<> rudeSoloDisp {
  display_L, rudeSolo, XBM::solo_7, {36 + 64, 0}, WHITE};

// Record arm / ready
BitmapDisplay<> recrdyDisp[] {
  {display_L, recrdy[0], XBM::rec_rdy_10B, {14 + 14, 50}, WHITE},
  {display_L, recrdy[1], XBM::rec_rdy_10B, {14 + 14 + 64, 50}, WHITE},
  {display_R, recrdy[2], XBM::rec_rdy_10B, {14 + 14, 50}, WHITE},
  {display_R, recrdy[3], XBM::rec_rdy_10B, {14 + 14 + 64, 50}, WHITE},
};

// VU meters
MCU::VUDisplay<> vuDisp[] {
  // position (32+11, 60), width (16), bar height (3) px, bar spacing (1) px
  {display_L, vu[0], {32 + 11, 60}, 16, 3, 1, WHITE},
  {display_L, vu[1], {32 + 11 + 64, 60}, 16, 3, 1, WHITE},
  {display_R, vu[2], {32 + 11, 60}, 16, 3, 1, WHITE},
  {display_R, vu[3], {32 + 11 + 64, 60}, 16, 3, 1, WHITE},
};

// VPot rings
MCU::VPotDisplay<> vpotDisp[] {
  // position (0, 10), outer radius (14) px, inner radius (12) px
  {display_L, vpot[0], {0, 10}, 14, 12, WHITE},
  {display_L, vpot[1], {64, 10}, 14, 12, WHITE},
  {display_R, vpot[2], {0, 10}, 14, 12, WHITE},
  {display_R, vpot[3], {64, 10}, 14, 12, WHITE},
};

// Bank seting
BankDisplay bankDisp[] {
  // first track of the bank (1), position (0, 50), font size (2)
  {display_L, bank, 1, {0, 50}, 2, WHITE},
  {display_L, bank, 2, {64, 50}, 2, WHITE},
  {display_R, bank, 3, {0, 50}, 2, WHITE},
  {display_R, bank, 4, {64, 50}, 2, WHITE},
};

// --------------------------------- Setup ---------------------------------- //
// ========================================================================== //

void setup() {
  Control_Surface.begin(); // Initialize Control Surface
}

// ---------------------------------- Loop ---------------------------------- //
// ========================================================================== //

void loop() {
  Control_Surface.loop(); // Refresh all elements
}
