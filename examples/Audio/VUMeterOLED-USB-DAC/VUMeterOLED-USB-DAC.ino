#include <Control_Surface.h> // Include the Control Surface library
// Include the display interface you'd like to use
#include <Display/DisplayInterfaces/DisplayInterfaceSSD1306.hpp>
#include <Wire.h>             // Include the I²C library for the display

// ----------------------------- Display setup ------------------------------ //
// ========================================================================== //

/*
   Instantiate and initialize the SSD1306 OLED display
*/

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

#include <Audio.h>

AudioInputUSB            audioInputUSB;
AudioAnalyzePeak         peak_L;
AudioMixer4              mixer_L;
AudioMixer4              mixer_R;
AudioAnalyzePeak         peak_R;
AudioOutputI2S           audioOutputI2S;
AudioConnection          patchCord1(audioInputUSB, 0, mixer_L, 0);
AudioConnection          patchCord2(audioInputUSB, 0, peak_L, 0);
AudioConnection          patchCord3(audioInputUSB, 1, mixer_R, 0);
AudioConnection          patchCord4(audioInputUSB, 1, peak_R, 0);
AudioConnection          patchCord5(mixer_L, 0, audioOutputI2S, 1);
AudioConnection          patchCord6(mixer_R, 0, audioOutputI2S, 0);

// ----------------------------- Volume control ----------------------------- //
// ========================================================================== //

template <uint8_t N>
class VolumeControl : public Updatable {
  public:
    VolumeControl(const Array<AudioMixer4*, N> &mixers, pin_t analogPin,
                  float maxGain = 1.0) 
      : mixers(mixers), filteredAnalog(analogPin), maxGain(maxGain) {}
    void update() override {
      if (filteredAnalog.update()) {
        float gain = filteredAnalog.getValue() * maxGain / 127.0;
        for (AudioMixer4 *mixer : mixers)
          for (uint8_t ch = 0; ch < 4; ch ++) 
            mixer->gain(ch, gain);
      }
    }
    void begin() override {}
    
  private:
    Array<AudioMixer4*, N> mixers;
    FilteredAnalog<7> filteredAnalog;
    const float maxGain;
};

VolumeControl<2> volume = { {&mixer_L, &mixer_R}, A0, 1.0 };

// ------------------------ Averaging filter for VU ------------------------- //
// ========================================================================== //

class EMA_f {
  public:
    EMA_f(float pole) : alpha(1 - pole) {}
    float filter(float value) {
      filtered = filtered + (value - filtered) * alpha;
      return filtered;
    }
    float operator()(float value) { return filter(value); }
  private:
    const float alpha;
    float filtered = 0;
};

// ------------------------------- VU Meters -------------------------------- //
// ========================================================================== //

class AudioVU : public IVU {
  public:
    AudioVU(AudioAnalyzePeak &level, float gain = 1.0, uint8_t max = 12)
      : level(level), gain(gain), max(max) {}
    uint8_t getValue() override {
      uint8_t value = filter(level.read()) * gain * max;
      if (value > max)
        value = max;
      return value;
    }
    bool getOverload() override {
      return false;
    }
    void setGain(float gain) { this->gain = gain; }
  private:
    EMA_f filter = 0.6;
    AudioAnalyzePeak &level;
    float gain;
    const uint8_t max;
};

AudioVU vu_L = {peak_L, 1, 25};
AudioVU vu_R = {peak_R, 1, 25};

MCU::VUDisplay vu_disp_L = { display, vu_L, {0, 127}, 31, 4, 1, WHITE };
MCU::VUDisplay vu_disp_R = { display, vu_R, {33, 127}, 31, 4, 1, WHITE };

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
  if (frametime >= 20) { // 50 fps
    DisplayInterface::clearAll();
    DisplayInterface::drawAllBackgrounds();
    DisplayElement::drawAll();
    DisplayInterface::displayAll();
    frametime = 0;
  }
  if (gainKnob.update()) {
    float gain = gainKnob.getValue() * maxGain / 127.0;
    vu_L.setGain(gain);
    vu_R.setGain(gain);
  }
  Updatable::updateAll();
}
