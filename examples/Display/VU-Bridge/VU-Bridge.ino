#include <Control_Surface.h> // Include the Control Surface library
// Include the display interface you'd like to use
#include <Display/DisplayInterfaces/DisplayInterfaceSSD1306.hpp>


#include <Adafruit_SSD1306.h> // Include the library for your specific display
#include <Wire.h>             // Include the I²C library for the display

// ----------------------------- MIDI Interface ----------------------------- //
// ========================================================================== //

// Instantiate a MIDI interface to use for the Control Surface.
USBMIDI_Interface midi;

// ----------------------------- Display setup ------------------------------ //
// ========================================================================== //

Adafruit_SSD1306 ssd1306Display; // Instantiate a display

void initializeDisplay() {
  // Initialize with the display with I²C address 0x3C
  ssd1306Display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Wire.setClock(1800000); // Set the I²C clock to 1.8 MHz for faster refresh
  ssd1306Display.setRotation(2); // Flip the display upside down
}

// --------------------------- Display interface ---------------------------- //
// ========================================================================== //

// Define and instantiate a display interface
class MySSD1306_DisplayInterface : public SSD1306_DisplayInterface {
  public:
    MySSD1306_DisplayInterface(Adafruit_SSD1306 &display)
      : SSD1306_DisplayInterface(display) {}

    void drawBackground() override {
        disp.drawFastHLine(0, 52, 128, WHITE);
        // disp.drawFastHLine(0, 63, 128, WHITE);
        disp.drawRect(0, 0, 128, 64, WHITE);
    }
} display = ssd1306Display;

// -------------------------- MIDI Output Elements -------------------------- //
// ========================================================================== //

constexpr uint8_t channel = 1;

// A potentiometer that sets the master volume on analog pin A1
PBPotentiometer masterVolume = { A1, MCU::MASTER_VOLUME };

// A button to press play or pause on digital pin 7
NoteButton playButton = {7, MCU::PLAY, channel};

// -------------------------- MIDI Input Elements --------------------------- //
// ========================================================================== //

MIDINote mute[8] = {
  { MCU::MUTE_1, channel }, // The mute status of the first track
  { MCU::MUTE_2, channel },
  { MCU::MUTE_3, channel },
  { MCU::MUTE_4, channel },
  { MCU::MUTE_5, channel },
  { MCU::MUTE_6, channel },
  { MCU::MUTE_7, channel },
  { MCU::MUTE_8, channel },
};

MIDINote solo[8] = {
  { MCU::SOLO_1, channel }, // The solo status of the first track
  { MCU::SOLO_2, channel },
  { MCU::SOLO_3, channel },
  { MCU::SOLO_4, channel },
  { MCU::SOLO_5, channel },
  { MCU::SOLO_6, channel },
  { MCU::SOLO_7, channel },
  { MCU::SOLO_8, channel },
};

// VU meters
MCU::VU VUMeters[8] = {
  { 1, channel, 0 }, // The VU meter for the first track, don't decay
  { 2, channel, 0 },
  { 3, channel, 0 },
  { 4, channel, 0 },
  { 5, channel, 0 },
  { 6, channel, 0 },
  { 7, channel, 0 },
  { 8, channel, 0 },
};

// ---------------------------- Display Elements ---------------------------- //
// ========================================================================== //

MCU::VUDisplay vuDisp[8] = {
  // Draw the first VU meter to the display, at position (2, 48),
  // (12) pixels wide, blocks of 3 pixels high, a spacing between 
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
  initializeDisplay();     // Start the OLED display
  Control_Surface.begin(); // Initialize Control Surface
}

// ---------------------------------- Loop ---------------------------------- //
// ========================================================================== //

void loop() {
  Control_Surface.loop(); // Refresh all elements
}
