/**
 * This is an example of the VolumeControl class of the Control Surface library.
 * It acts as a USB Audio DAC (Digital-to-Analog Converter).
 * 
 * @boards Teensy 3.x
 * 
 * Connections
 * -----------
 * 
 * This specific example uses an I²S DAC, but you can use other interfaces of 
 * the Teensy Audio library as well. For testing, I'm using a PCM5102a.
 * 
 * - A0: wiper of a potentiometer to change the output volume
 * - 9:  DAC BCK (I²S)
 * - 11: DAC SCK (I²S)
 * - 22: DAC DIN (I²S)
 * - 23: DAC LRCK (I²S)
 * 
 * Connect the left terminal of the potentiometer to ground, and the right one
 * to V<sub>CC</sub>.
 * 
 * Select a USB audio option from the Tools menu in the IDE.
 * 
 * Behavior
 * --------
 * 
 * Upload the sketch, and select the Control Surface as the audio output of your
 * computer. Connect the output of the DAC to a pair of headphones or powered
 * speakers, and play some music.  
 * You can now adjust the volume using the potentiometer.
 * 
 * Mapping
 * -------
 * 
 * None.
 * 
 * Written by PieterP, 2019-08-09  
 * https://github.com/tttapa/Control-Surface 
 */

#include <Control_Surface.h> // Include the Control Surface library

AudioInputUSB audioInputUSB; // The audio input from the USB connection
AudioMixer4 mixer_L;         // Two mixers for volume control
AudioMixer4 mixer_R;
AudioOutputI2S audioOutputI2S; // The output to the I²S DAC

AudioConnection patchCord1(audioInputUSB, 0, mixer_L, 0);  // Connect the input
AudioConnection patchCord3(audioInputUSB, 1, mixer_R, 0);  // to the mixer
AudioConnection patchCord5(mixer_L, 0, audioOutputI2S, 1); // Connect the mixer
AudioConnection patchCord6(mixer_R, 0, audioOutputI2S, 0); // to the output

// Instantiate a VolumeControl object with a potentiometer on pin A0 and a
// maximum gain of 1.0
VolumeControl<2> volume {{&mixer_L, &mixer_R}, A0, 1.0};

void setup() {
  volume.begin();
  // Add a dead zone if you can't get the volume all the way to zero
  // volume.map([](analog_t i) -> analog_t {
  //   const analog_t lower = 0 + 10;
  //   const analog_t upper = 1023 - 10;
  //   return map(constrain(i, lower, upper), lower, upper, 0, 1023);
  // });
  AudioMemory(6);
}

void loop() {
  volume.update();
}
