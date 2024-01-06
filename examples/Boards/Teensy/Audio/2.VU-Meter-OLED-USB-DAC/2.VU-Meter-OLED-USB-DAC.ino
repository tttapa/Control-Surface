/**
 * This example shows the usage of the AudioVU and AnalogVUDisplay classes of 
 * the Control Surface library.  
 * 
 * It displays two analog-style VU meters on two 128×64 OLED displays.  
 * The VU meters imitate the inertia and ballistics of real analog VU meters.
 * 
 * It acts as a USB Audio DAC (Digital-to-Analog Converter).
 * 
 * @boards  Teensy 3.x
 * 
 * Connections
 * -----------
 * 
 * - A0: wiper of a potentiometer to change the output volume
 * - A1: wiper of a potentiometer to change the VU gain
 * 
 * - 9:  BCK (I²S)
 * - 11: SCK (I²S)
 * - 22: DIN (I²S)
 * - 23: LRCK (I²S)
 * 
 * - 7:  OLED Data/D1 (SPI MOSI)
 * - 13: OLED Clock/D0 (SPI SCK)
 * - 17: OLED Data/Command
 * - 10: Left OLED %Cable Select
 * - 18: Right OLED %Cable Select
 * 
 * Add a capacitor between the reset pin of the displays and ground, and a 
 * resistor from reset to 3.3V. The values are not critical, 0.1µF and 10kΩ 
 * work fine.  
 * You do need some way to reset the displays, without it, it won't work.  
 * Alternatively, you could use an IO pin from the Teensy to reset the 
 * displays, but this just "wastes" a pin.
 * 
 * Behavior
 * --------
 * 
 * Upload the sketch, and select the Control Surface as the audio output of your
 * computer. Connect the output of the DAC to a pair of headphones or powered
 * speakers, and play some music.  
 * You should see the VU meters come to life and jump around to the music.
 * 
 * You can now adjust the volume using the potentiometer on pin A0, and the 
 * gain/sensitivity of the meters using the potentiometer on pin A1.
 * 
 * Mapping
 * -------
 * 
 * None.
 * 
 * Demo
 * ----
 * 
 * @todo Add a demo video.
 * 
 * Written by PieterP, 2019-08-09  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h> // Include the Control Surface library
// Include the display interface you'd like to use
#include <Display/DisplayInterfaces/DisplayInterfaceSSD1306.hpp>

// #define DIGITAL_VU

// ----------------------------- Display setup ------------------------------ //
// ========================================================================== //

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
// Instantiate the displays
Adafruit_SSD1306 ssd1306Display_R {
  SCREEN_WIDTH, SCREEN_HEIGHT, &SPI,          OLED_DC,
  OLED_reset,   OLED_CS_R,     SPI_Frequency,
};

// --------------------------- Display interface ---------------------------- //
// ========================================================================== //

// Implement the display interface, specifically, the begin and drawBackground
// methods.
// If you want, you can draw a background image of a VU meter first, and then
// draw the needle on top.
class MySSD1306_DisplayInterface : public SSD1306_DisplayInterface {
 public:
  MySSD1306_DisplayInterface(Adafruit_SSD1306 &display)
    : SSD1306_DisplayInterface(display) {}

  void begin() override {
    // Initialize the Adafruit_SSD1306 display
    if (!disp.begin())
      FATAL_ERROR(F("SSD1306 allocation failed."), 0x1306);

#ifdef DIGITAL_VU
    disp.setRotation(1); // Rotate the display in portrait mode
#endif

    // If you override the begin method, remember to call the super class method
    SSD1306_DisplayInterface::begin();
  }

  void drawBackground() override {}

} display_L = ssd1306Display_L, display_R = ssd1306Display_R;

// --------------------------- Audio connections ---------------------------- //
// ========================================================================== //

AudioInputUSB audioInputUSB;
AudioMixer4 mixer_L;
AudioAnalyzeRMS rms_L;
AudioMixer4 mixer_R;
AudioAnalyzeRMS rms_R;
AudioOutputI2S audioOutputI2S;
// Connect the input to the mixers (volume control), the input to the RMS
// meters, and the mixers to the output
AudioConnection patchCord1(audioInputUSB, 0, mixer_L, 0);
AudioConnection patchCord2(audioInputUSB, 0, rms_L, 0);
AudioConnection patchCord3(audioInputUSB, 1, mixer_R, 0);
AudioConnection patchCord4(audioInputUSB, 1, rms_R, 0);
AudioConnection patchCord5(mixer_L, 0, audioOutputI2S, 0);
AudioConnection patchCord6(mixer_R, 0, audioOutputI2S, 1);

// ----------------------------- Volume control ----------------------------- //
// ========================================================================== //

VolumeControl<2> volume {{&mixer_L, &mixer_R}, A0, 1.0};

// ------------------------------- VU Meters -------------------------------- //
// ========================================================================== //

#ifdef DIGITAL_VU
AudioVU vu_L {rms_L, MovingCoilBallistics::noOvershoot(), 1, 25};
AudioVU vu_R {rms_R, MovingCoilBallistics::noOvershoot(), 1, 25};

MCU::VUDisplay<> vu_display_L {display_L, vu_L, {0, 127}, 64, 4, 1, WHITE};
MCU::VUDisplay<> vu_display_R {display_R, vu_R, {0, 127}, 64, 4, 1, WHITE};
#else
AudioVU vu_L {rms_L, MovingCoilBallistics::responsiveVU(1)};
AudioVU vu_R {rms_R, MovingCoilBallistics::responsiveVU(1)};

MCU::AnalogVUDisplay<> vu_display_L {
  display_L,       // Display to display on
  vu_L,            // VU meter to display
  {63, 63},        // Location of the needle pivot
  63,              // Length of the needle
  -140 * PI / 180, // Minimum angle (radians)
  100 * PI / 180,  // Total angle range (radians)
  WHITE,           // Color
};
// Note that the y axis points downwards (as is common in computer graphics).
// This means that a positive angle is clockwise, and -140° lies in the top left
// quadrant
MCU::AnalogVUDisplay<> vu_display_R {
  display_R, vu_R, {63, 63}, 63, -140 * PI / 180, 100 * PI / 180, WHITE,
};
#endif

// ------------------------------- Gain knob -------------------------------- //
// ========================================================================== //

constexpr float maxGain = 5;
FilteredAnalog<> gainKnob = A1;

// --------------------------------- Setup ---------------------------------- //
// ========================================================================== //

void setup() {
  AudioMemory(8);
  FilteredAnalog<>::setupADC();
  display_L.begin();
  display_R.begin();
}

// ---------------------------------- Loop ---------------------------------- //
// ========================================================================== //

void loop() {
  const unsigned long frametime = 1'000'000 / MAX_FPS;
  static unsigned long previousFrameTime = micros();
  if (micros() - previousFrameTime >= frametime) {
    previousFrameTime += frametime;
    Updatable<AudioVU>::updateAll();       // Update VU meters
    Control_Surface.updateDisplays();      // Update displays
    Updatable<Potentiometer>::updateAll(); // Update volume controls
  }
  if (gainKnob.update()) {
    float gain = gainKnob.getFloatValue() * maxGain;
    vu_L.setGain(gain);
    vu_R.setGain(gain);
  }
}