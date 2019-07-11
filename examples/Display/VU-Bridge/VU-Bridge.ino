/**
 * @brief   This is an example on how to use an OLED display to display the 
 *          VU meters and mute/solo states of the eight first tracks, by using 
 *          the Arduino as a Mackie Control Universal.  
 * 
 * If you want to display more than eight tracks, have a look at the 
 * @ref VU-Bridge-Dual-Display.ino example, that uses MCU Extenders to display 
 * up to 32 tracks.
 * 
 * The OLED display uses a RAM frame buffer of 1 KiB (128×64 pixels / 8 pixels
 * per byte). This is a significant amount for AVR boards like the Arduino UNO,
 * Nano, Leonardo, etc. Keep in mind that you might run out of memory, and that
 * you'll need a more powerful board.
 * 
 * ### Connections
 * This example drives two SSD1306 OLED displays over SPI
 *  - SCK:  SSD1306 D0
 *  - MOSI: SSD1306 D1
 *  - 19:   SSD1306 DC
 *  - 18:   SSD1306 CS
 * 
 * Add a capacitor between the reset pin of the display and ground, and a 
 * resistor from reset to 3.3V. The values are not critical, 0.1µF and 10kΩ 
 * work fine.  
 * You do need some way to reset the display, without it, it won't work.  
 * Alternatively, you could use an IO pin from the Arduino to reset the 
 * display, but this just "wastes" a pin.
 * 
 * @note    Don't forget that most OLED displays are 3.3V only, so connecting 
 *          a display to a 5V Arduino directly will destroy it!
 * 
 * ### Behavior
 * Map "Control Surface" as a Mackie Control Universal unit in your DAW.
 * 
 * The first display should now display the level meters and mute/solo states
 * of the first 8 tracks.
 * 
 * @note    There seem to be some differences in the way some applications 
 *          handle VU meters: some expect the hardware to decay automatically,
 *          some don't.  
 *          If you notice that the meters behave strangely, try both decay 
 *          options of the MCU::VU class, or try a different decay time.
 * 
 * ### Demo
 * 
 * @htmlonly
 * <iframe width="560" height="315"
 * src="https://www.youtube.com/embed/_NJh6LyJjU8?start=25" frameborder="0"
 * allow="accelerometer; autoplay; encrypted-media; gyroscope;
 * picture-in-picture" allowfullscreen></iframe>
 * @endhtmlonly
 */

#include <Control_Surface.h> // Include the Control Surface library
// Include the display interface you'd like to use
#include <Display/DisplayInterfaces/DisplayInterfaceSSD1306.hpp>

// ----------------------------- MIDI Interface ----------------------------- //
// ========================================================================== //

// Instantiate a MIDI interface to use for the Control Surface.
USBMIDI_Interface midi;

// ----------------------------- Display setup ------------------------------ //
// ========================================================================== //

constexpr uint8_t SCREEN_WIDTH = 128;
constexpr uint8_t SCREEN_HEIGHT = 64;

constexpr int8_t OLED_DC = 19;    // Data/Command pin of the display
constexpr int8_t OLED_reset = -1; // Use the external RC circuit for reset
constexpr int8_t OLED_CS = 18;    // Chip Select pin of the display

constexpr uint32_t SPI_Frequency = SPI_MAX_SPEED;

// Instantiate the displays
Adafruit_SSD1306 ssd1306Display = {
  SCREEN_WIDTH, SCREEN_HEIGHT,
  &SPI, OLED_DC, OLED_reset, OLED_CS, SPI_Frequency
};

// --------------------------- Display interface ---------------------------- //
// ========================================================================== //

// Define and instantiate a display interface
class MySSD1306_DisplayInterface : public SSD1306_DisplayInterface {
  public:
    MySSD1306_DisplayInterface(Adafruit_SSD1306 &display)
      : SSD1306_DisplayInterface(display) {}

    void begin() override {
        if(!disp.begin())
            FATAL_ERROR(F("SSD1306 allocation failed."), 0x1306);
        SSD1306_DisplayInterface::begin(); // If you override the begin method,
                                           // remember to call the super class
                                           // method
    }

    void drawBackground() override {
      disp.drawFastHLine(0, 52, 128, WHITE);
      disp.drawRect(0, 0, 128, 64, WHITE);
    }
} display = ssd1306Display;

// -------------------------- MIDI Input Elements --------------------------- //
// ========================================================================== //

MIDINote mute[8] = {
  { MCU::MUTE_1 }, // The mute status of the first track
  { MCU::MUTE_2 },
  { MCU::MUTE_3 },
  { MCU::MUTE_4 },
  { MCU::MUTE_5 },
  { MCU::MUTE_6 },
  { MCU::MUTE_7 },
  { MCU::MUTE_8 },
};

MIDINote solo[8] = {
  { MCU::SOLO_1 }, // The solo status of the first track
  { MCU::SOLO_2 },
  { MCU::SOLO_3 },
  { MCU::SOLO_4 },
  { MCU::SOLO_5 },
  { MCU::SOLO_6 },
  { MCU::SOLO_7 },
  { MCU::SOLO_8 },
};

// const auto decay = MCU::VU::NO_DECAY;
const auto decay = 60;

// VU meters
MCU::VU VUMeters[8] = {
  { 1, decay }, // The VU meter for the first track, don't decay
  { 2, decay },
  { 3, decay },
  { 4, decay },
  { 5, decay },
  { 6, decay },
  { 7, decay },
  { 8, decay },
};

// ---------------------------- Display Elements ---------------------------- //
// ========================================================================== //

MCU::VUDisplay vuDisp[8] = {
  // Draw the first VU meter to the display, at position (2, 48),
  // (12) pixels wide, blocks of (3) pixels high, a spacing between 
  // blocks of (1) pixel, and draw in white.
  { display, VUMeters[0], { 2 + 16 * 0, 50 }, 12, 3, 1, WHITE },
  { display, VUMeters[1], { 2 + 16 * 1, 50 }, 12, 3, 1, WHITE },
  { display, VUMeters[2], { 2 + 16 * 2, 50 }, 12, 3, 1, WHITE },
  { display, VUMeters[3], { 2 + 16 * 3, 50 }, 12, 3, 1, WHITE },
  { display, VUMeters[4], { 2 + 16 * 4, 50 }, 12, 3, 1, WHITE },
  { display, VUMeters[5], { 2 + 16 * 5, 50 }, 12, 3, 1, WHITE },
  { display, VUMeters[6], { 2 + 16 * 6, 50 }, 12, 3, 1, WHITE },
  { display, VUMeters[7], { 2 + 16 * 7, 50 }, 12, 3, 1, WHITE },
};

NoteBitmapDisplay muteDisp[8] = {
  // Draw the first mute indicator to the display, at position (4, 54),
  // using bitmap icon mute_7 with a white foreground color. 
  { display, mute[0], XBM::mute_7, { 4 + 16 * 0, 54 }, WHITE },
  { display, mute[1], XBM::mute_7, { 4 + 16 * 1, 54 }, WHITE },
  { display, mute[2], XBM::mute_7, { 4 + 16 * 2, 54 }, WHITE },
  { display, mute[3], XBM::mute_7, { 4 + 16 * 3, 54 }, WHITE },
  { display, mute[4], XBM::mute_7, { 4 + 16 * 4, 54 }, WHITE },
  { display, mute[5], XBM::mute_7, { 4 + 16 * 5, 54 }, WHITE },
  { display, mute[6], XBM::mute_7, { 4 + 16 * 6, 54 }, WHITE },
  { display, mute[7], XBM::mute_7, { 4 + 16 * 7, 54 }, WHITE },
};

NoteBitmapDisplay soloDisp[8] = {
  // Draw the first solo indicator to the display, at position (4, 54),
  // using bitmap icon solo_7 with a white foreground color.
  { display, solo[0], XBM::solo_7, { 4 + 16 * 0, 54 }, WHITE },
  { display, solo[1], XBM::solo_7, { 4 + 16 * 1, 54 }, WHITE },
  { display, solo[2], XBM::solo_7, { 4 + 16 * 2, 54 }, WHITE },
  { display, solo[3], XBM::solo_7, { 4 + 16 * 3, 54 }, WHITE },
  { display, solo[4], XBM::solo_7, { 4 + 16 * 4, 54 }, WHITE },
  { display, solo[5], XBM::solo_7, { 4 + 16 * 5, 54 }, WHITE },
  { display, solo[6], XBM::solo_7, { 4 + 16 * 6, 54 }, WHITE },
  { display, solo[7], XBM::solo_7, { 4 + 16 * 7, 54 }, WHITE },
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
