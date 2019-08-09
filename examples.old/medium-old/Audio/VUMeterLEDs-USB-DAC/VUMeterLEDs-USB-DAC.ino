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

// --------------------------- Audio connections ---------------------------- //
// ========================================================================== //

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

VolumeControl<2> volume = { {&mixer_L, &mixer_R}, A0, 1.0 };

// ----------------------------- Shift Register ----------------------------- //
// ========================================================================== //

const pin_t dataPin  = 12;  // Pin connected to DS of 74HC595
const pin_t clockPin = 13;  // Pin connected to SH_CP of 74HC595
const pin_t latchPin = 10;  // Pin connected to ST_CP of 74HC595

// Create a new shift register output connected to pins 11, 13 and 10,
// shift the data out with the most significant bit first.
// There are 24 (= 3·8) outputs in total.
ShiftRegisterOut<24> ShiftReg(dataPin, clockPin, latchPin, MSBFIRST);

// ------------------------------- VU Meters -------------------------------- //
// ========================================================================== //

AudioVULEDs<12> vu_L = {
  generateIncrementalArray<pin_t, 12>(ShiftReg.pin(0), 1),
  peak_L, 1.0
};
AudioVULEDs<12> vu_R = {
  generateIncrementalArray<pin_t, 12>(ShiftReg.pin(12), 1),
  peak_R, 1.0
};

// ------------------------------- Gain knob -------------------------------- //
// ========================================================================== //

constexpr float maxGain = 5;
FilteredAnalog<7> gainKnob = A1;

// --------------------------------- Setup ---------------------------------- //
// ========================================================================== //

void setup() {
  AudioMemory(8);
  ShiftReg.begin();
  Updatable<AudioVU>::beginAll();
  Updatable<Potentiometer>::beginAll();
}

// ---------------------------------- Loop ---------------------------------- //
// ========================================================================== //

void loop() {
  static elapsedMillis frametime = 0;
  if (frametime >= 20) { // 50 fps
    Updatable<AudioVU>::updateAll();
    Updatable<Potentiometer>::updateAll();
    frametime = 0;
    if (gainKnob.update()) {
      float gain = gainKnob.getValue() * maxGain / 127.0;
      vu_L.setGain(gain);
      vu_R.setGain(gain);
    }
  }
}
