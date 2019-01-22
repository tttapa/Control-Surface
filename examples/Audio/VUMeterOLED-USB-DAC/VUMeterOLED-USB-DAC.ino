/**
 * Connections:
 * 
 * A0: wiper of a potentiometer to change the output volume
 * A1: wiper of a potentiometer to change the VU gain
 * 
 * 9:  BCK (I²S)
 * 11: SCK (I²S)
 * 22: DIN (I²S)
 * 23: LRCK (I²S)
 */

#include <Control_Surface.h> // Include the Control Surface library
// Include the display interface you'd like to use
#include <Display/DisplayInterfaces/DisplayInterfaceSSD1306.hpp>
#include <Wire.h> // Include the I²C library for the display

// ----------------------------- Display setup ------------------------------ //
// ========================================================================== //

/* Instantiate and initialize the SSD1306 OLED display */

Adafruit_SSD1306 ssd1306Display; // Instantiate a display

void initializeDisplay() {
    // Initialize with the display with I²C address 0x3C
    ssd1306Display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    Wire.setClock(1800000); // Set the I²C clock to 1.8 MHz for faster refresh
    ssd1306Display.setRotation(3); // Rotate the display in portrait mode
}

// --------------------------- Display interface ---------------------------- //
// ========================================================================== //

/*
   This creates a mapping between your specific display driver and the
   general display interface used by the Control Surface library.
   It is roughly based on the Adafruit_GFX API.
*/

class MySSD1306_DisplayInterface : public SSD1306_DisplayInterface {
  public:
    MySSD1306_DisplayInterface(Adafruit_SSD1306 &display)
        : SSD1306_DisplayInterface(display) {}

    void drawBackground() override {}
} display = ssd1306Display;

// --------------------------- Audio connections ---------------------------- //
// ========================================================================== //

AudioInputUSB audioInputUSB;
AudioAnalyzePeak peak_L;
AudioMixer4 mixer_L;
AudioMixer4 mixer_R;
AudioAnalyzePeak peak_R;
AudioOutputI2S audioOutputI2S;
AudioConnection patchCord1(audioInputUSB, 0, mixer_L, 0);
AudioConnection patchCord2(audioInputUSB, 0, peak_L, 0);
AudioConnection patchCord3(audioInputUSB, 1, mixer_R, 0);
AudioConnection patchCord4(audioInputUSB, 1, peak_R, 0);
AudioConnection patchCord5(mixer_L, 0, audioOutputI2S, 1);
AudioConnection patchCord6(mixer_R, 0, audioOutputI2S, 0);

// ----------------------------- Volume control ----------------------------- //
// ========================================================================== //

VolumeControl<2> volume = {{&mixer_L, &mixer_R}, A0, 1.0};

// ------------------------------- VU Meters -------------------------------- //
// ========================================================================== //

AudioVU vu_L = {peak_L, 1, 25};
AudioVU vu_R = {peak_R, 1, 25};

MCU::VUDisplay vu_disp_L = {display, vu_L, {0, 127}, 31, 4, 1, WHITE};
MCU::VUDisplay vu_disp_R = {display, vu_R, {33, 127}, 31, 4, 1, WHITE};

// ------------------------------- Gain knob -------------------------------- //
// ========================================================================== //

constexpr float maxGain = 5;
FilteredAnalog<7> gainKnob = A1;

// --------------------------------- Setup ---------------------------------- //
// ========================================================================== //

void setup() {
    AudioMemory(8);
    initializeDisplay();
    Serial.begin(115200);
}

// ---------------------------------- Loop ---------------------------------- //
// ========================================================================== //

void loop() {
    static elapsedMillis frametime = 0;
    if (frametime >= 20) {                     // 50 fps
        Updatable<AudioVU>::updateAll();       // Update VU meters
        Updatable<Potentiometer>::updateAll(); // Update volume controls
        Control_Surface.updateDisplays();      // Update displays
        frametime = 0;
    }
    if (gainKnob.update()) {
        float gain = gainKnob.getValue() * maxGain / 127.0;
        vu_L.setGain(gain);
        vu_R.setGain(gain);
    }
}
