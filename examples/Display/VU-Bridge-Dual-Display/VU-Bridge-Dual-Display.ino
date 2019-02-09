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

constexpr int8_t OLED_DC = 19;
constexpr int8_t OLED_reset = -1;
constexpr int8_t OLED_CSA = 17;
constexpr int8_t OLED_CSB = 18;

constexpr uint32_t SPI_Frequency = SPI_MAX_SPEED;

// Instantiate the displays
Adafruit_SSD1306 ssd1306DisplayA = {
  SCREEN_WIDTH, SCREEN_HEIGHT,
  &SPI, OLED_DC, OLED_reset, OLED_CSA, SPI_Frequency
};
Adafruit_SSD1306 ssd1306DisplayB = {
  SCREEN_WIDTH, SCREEN_HEIGHT,
  &SPI, OLED_DC, OLED_reset, OLED_CSB, SPI_Frequency
};

void initializeDisplays() {
  ssd1306DisplayA.begin();
  ssd1306DisplayB.begin();
  // ssd1306DisplayA.setRotation(2); // Flip the display upside down
  // ssd1306DisplayB.setRotation(2); // Flip the display upside down
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
      disp.drawRect(0, 0, 128, 64, WHITE);
    }
} displayA = ssd1306DisplayA, displayB = ssd1306DisplayB;

// -------------------------- MIDI Output Elements -------------------------- //
// ========================================================================== //

// A potentiometer that sets the master volume on analog pin A1
PBPotentiometer masterVolume = { A1, MCU::MASTER_VOLUME };

// A button to press play or pause on digital pin 7
NoteButton playButton = {7, MCU::PLAY};

// -------------------------- MIDI Input Elements --------------------------- //
// ========================================================================== //

MIDINote mute[8] = {
  {{ MCU::MUTE_1, CHANNEL_1, 0x0 }}, // The mute status of the first track
  {{ MCU::MUTE_2, CHANNEL_1, 0x0 }},
  {{ MCU::MUTE_3, CHANNEL_1, 0x0 }},
  {{ MCU::MUTE_4, CHANNEL_1, 0x0 }},
  {{ MCU::MUTE_5, CHANNEL_1, 0x0 }},
  {{ MCU::MUTE_6, CHANNEL_1, 0x0 }},
  {{ MCU::MUTE_7, CHANNEL_1, 0x0 }},
  {{ MCU::MUTE_8, CHANNEL_1, 0x0 }},
};
MIDINote muteXT[8] = {
  {{ MCU::MUTE_1, CHANNEL_1, 0x1 }}, // The mute status of the first track
  {{ MCU::MUTE_2, CHANNEL_1, 0x1 }},
  {{ MCU::MUTE_3, CHANNEL_1, 0x1 }},
  {{ MCU::MUTE_4, CHANNEL_1, 0x1 }},
  {{ MCU::MUTE_5, CHANNEL_1, 0x1 }},
  {{ MCU::MUTE_6, CHANNEL_1, 0x1 }},
  {{ MCU::MUTE_7, CHANNEL_1, 0x1 }},
  {{ MCU::MUTE_8, CHANNEL_1, 0x1 }},
};

MIDINote solo[8] = {
  {{ MCU::SOLO_1, CHANNEL_1, 0x0 }}, // The solo status of the first track
  {{ MCU::SOLO_2, CHANNEL_1, 0x0 }},
  {{ MCU::SOLO_3, CHANNEL_1, 0x0 }},
  {{ MCU::SOLO_4, CHANNEL_1, 0x0 }},
  {{ MCU::SOLO_5, CHANNEL_1, 0x0 }},
  {{ MCU::SOLO_6, CHANNEL_1, 0x0 }},
  {{ MCU::SOLO_7, CHANNEL_1, 0x0 }},
  {{ MCU::SOLO_8, CHANNEL_1, 0x0 }},
};
MIDINote soloXT[8] = {
  {{ MCU::SOLO_1, CHANNEL_1, 0x1 }}, // The solo status of the first track
  {{ MCU::SOLO_2, CHANNEL_1, 0x1 }},
  {{ MCU::SOLO_3, CHANNEL_1, 0x1 }},
  {{ MCU::SOLO_4, CHANNEL_1, 0x1 }},
  {{ MCU::SOLO_5, CHANNEL_1, 0x1 }},
  {{ MCU::SOLO_6, CHANNEL_1, 0x1 }},
  {{ MCU::SOLO_7, CHANNEL_1, 0x1 }},
  {{ MCU::SOLO_8, CHANNEL_1, 0x1 }},
};

// const auto decay = MCU::VU::NO_DECAY;
const auto decay = 75;

// VU meters
MCU::VU VUMeters[8] = {
  // The VU meter for the first track, don't decay
  { {1, CHANNEL_1, 0x0}, decay }, 
  { {2, CHANNEL_1, 0x0}, decay },
  { {3, CHANNEL_1, 0x0}, decay },
  { {4, CHANNEL_1, 0x0}, decay },
  { {5, CHANNEL_1, 0x0}, decay },
  { {6, CHANNEL_1, 0x0}, decay },
  { {7, CHANNEL_1, 0x0}, decay },
  { {8, CHANNEL_1, 0x0}, decay },
};
MCU::VU VUMetersXT[8] = {
  // The VU meter for the first track, don't decay
  { {1, CHANNEL_1, 0x1}, decay }, 
  { {2, CHANNEL_1, 0x1}, decay },
  { {3, CHANNEL_1, 0x1}, decay },
  { {4, CHANNEL_1, 0x1}, decay },
  { {5, CHANNEL_1, 0x1}, decay },
  { {6, CHANNEL_1, 0x1}, decay },
  { {7, CHANNEL_1, 0x1}, decay },
  { {8, CHANNEL_1, 0x1}, decay },
};

// ---------------------------- Display Elements ---------------------------- //
// ========================================================================== //

MCU::VUDisplay vuDisp[8] = {
  // Draw the first VU meter to the display, at position (2, 48),
  // (12) pixels wide, blocks of 3 pixels high, a spacing between
  // blocks of (1) pixel, and draw in white.
  { displayA, VUMeters[0], { 2 + 16 * 0, 50 }, 12, 3, 1, WHITE },
  { displayA, VUMeters[1], { 2 + 16 * 1, 50 }, 12, 3, 1, WHITE },
  { displayA, VUMeters[2], { 2 + 16 * 2, 50 }, 12, 3, 1, WHITE },
  { displayA, VUMeters[3], { 2 + 16 * 3, 50 }, 12, 3, 1, WHITE },
  { displayA, VUMeters[4], { 2 + 16 * 4, 50 }, 12, 3, 1, WHITE },
  { displayA, VUMeters[5], { 2 + 16 * 5, 50 }, 12, 3, 1, WHITE },
  { displayA, VUMeters[6], { 2 + 16 * 6, 50 }, 12, 3, 1, WHITE },
  { displayA, VUMeters[7], { 2 + 16 * 7, 50 }, 12, 3, 1, WHITE },
};
MCU::VUDisplay vuDispXT[8] = {
  { displayB, VUMetersXT[0], { 2 + 16 * 0, 50 }, 12, 3, 1, WHITE },
  { displayB, VUMetersXT[1], { 2 + 16 * 1, 50 }, 12, 3, 1, WHITE },
  { displayB, VUMetersXT[2], { 2 + 16 * 2, 50 }, 12, 3, 1, WHITE },
  { displayB, VUMetersXT[3], { 2 + 16 * 3, 50 }, 12, 3, 1, WHITE },
  { displayB, VUMetersXT[4], { 2 + 16 * 4, 50 }, 12, 3, 1, WHITE },
  { displayB, VUMetersXT[5], { 2 + 16 * 5, 50 }, 12, 3, 1, WHITE },
  { displayB, VUMetersXT[6], { 2 + 16 * 6, 50 }, 12, 3, 1, WHITE },
  { displayB, VUMetersXT[7], { 2 + 16 * 7, 50 }, 12, 3, 1, WHITE },
};

NoteBitmapDisplay muteDisp[8] = {
  // Draw the first mute indicator to the display, at position (4, 54),
  // using bitmap icon mute_7 with a white foreground color.
  { displayA, mute[0], XBM::mute_7, { 4 + 16 * 0, 54 }, WHITE },
  { displayA, mute[1], XBM::mute_7, { 4 + 16 * 1, 54 }, WHITE },
  { displayA, mute[2], XBM::mute_7, { 4 + 16 * 2, 54 }, WHITE },
  { displayA, mute[3], XBM::mute_7, { 4 + 16 * 3, 54 }, WHITE },
  { displayA, mute[4], XBM::mute_7, { 4 + 16 * 4, 54 }, WHITE },
  { displayA, mute[5], XBM::mute_7, { 4 + 16 * 5, 54 }, WHITE },
  { displayA, mute[6], XBM::mute_7, { 4 + 16 * 6, 54 }, WHITE },
  { displayA, mute[7], XBM::mute_7, { 4 + 16 * 7, 54 }, WHITE },
};
NoteBitmapDisplay muteDispXT[8] = {
  { displayB, muteXT[0], XBM::mute_7, { 4 + 16 * 0, 54 }, WHITE },
  { displayB, muteXT[1], XBM::mute_7, { 4 + 16 * 1, 54 }, WHITE },
  { displayB, muteXT[2], XBM::mute_7, { 4 + 16 * 2, 54 }, WHITE },
  { displayB, muteXT[3], XBM::mute_7, { 4 + 16 * 3, 54 }, WHITE },
  { displayB, muteXT[4], XBM::mute_7, { 4 + 16 * 4, 54 }, WHITE },
  { displayB, muteXT[5], XBM::mute_7, { 4 + 16 * 5, 54 }, WHITE },
  { displayB, muteXT[6], XBM::mute_7, { 4 + 16 * 6, 54 }, WHITE },
  { displayB, muteXT[7], XBM::mute_7, { 4 + 16 * 7, 54 }, WHITE },
};

NoteBitmapDisplay soloDisp[8] = {
  // Draw the first solo indicator to the display, at position (4, 54),
  // using bitmap icon solo_7 with a white foreground color.
  { displayA, solo[0], XBM::solo_7, { 4 + 16 * 0, 54 }, WHITE },
  { displayA, solo[1], XBM::solo_7, { 4 + 16 * 1, 54 }, WHITE },
  { displayA, solo[2], XBM::solo_7, { 4 + 16 * 2, 54 }, WHITE },
  { displayA, solo[3], XBM::solo_7, { 4 + 16 * 3, 54 }, WHITE },
  { displayA, solo[4], XBM::solo_7, { 4 + 16 * 4, 54 }, WHITE },
  { displayA, solo[5], XBM::solo_7, { 4 + 16 * 5, 54 }, WHITE },
  { displayA, solo[6], XBM::solo_7, { 4 + 16 * 6, 54 }, WHITE },
  { displayA, solo[7], XBM::solo_7, { 4 + 16 * 7, 54 }, WHITE },
};
NoteBitmapDisplay soloDispXT[8] = {
  { displayB, soloXT[0], XBM::solo_7, { 4 + 16 * 0, 54 }, WHITE },
  { displayB, soloXT[1], XBM::solo_7, { 4 + 16 * 1, 54 }, WHITE },
  { displayB, soloXT[2], XBM::solo_7, { 4 + 16 * 2, 54 }, WHITE },
  { displayB, soloXT[3], XBM::solo_7, { 4 + 16 * 3, 54 }, WHITE },
  { displayB, soloXT[4], XBM::solo_7, { 4 + 16 * 4, 54 }, WHITE },
  { displayB, soloXT[5], XBM::solo_7, { 4 + 16 * 5, 54 }, WHITE },
  { displayB, soloXT[6], XBM::solo_7, { 4 + 16 * 6, 54 }, WHITE },
  { displayB, soloXT[7], XBM::solo_7, { 4 + 16 * 7, 54 }, WHITE },
};

// --------------------------------- Setup ---------------------------------- //
// ========================================================================== //

void setup() {
  initializeDisplays();     // Start the OLED display
  Control_Surface.begin();  // Initialize Control Surface
}

// ---------------------------------- Loop ---------------------------------- //
// ========================================================================== //

void loop() {
  Control_Surface.loop(); // Refresh all elements
}
