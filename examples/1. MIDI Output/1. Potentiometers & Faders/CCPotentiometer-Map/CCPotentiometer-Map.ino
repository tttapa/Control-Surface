/**
 * This is an example of the `CCPotentiometer` class of the Control_Surface
 * library, and specifically, how to use the `map` function to account for
 * non-linearities of the potentiometer.
 * 
 * The map function in this example corrects for potentiometers that don't go
 * all the way to the extreme values. Some potentiometers still read a value
 * that's greater than 0 when turned all the way to the left. To fix that, tweak
 * the `minimumValue` constant. If the potentiometer doesn't read the maximum
 * value when turned all the way to the right, tweak the `maximumValue` 
 * constant.
 * 
 * @boards  AVR, AVR USB, Nano Every, Due, Nano 33 IoT, Nano 33 BLE, UNO R4, Pi Pico, ESP32, Teensy 3.x
 * 
 * Connections
 * -----------
 * 
 * - A0: wiper of a potentiometer
 * 
 * Connect the left terminal of the potentiometer to ground, and the right one
 * to V<sub>CC</sub>.
 * 
 * Behavior
 * --------
 * 
 * - If you open the Serial Monitor, you'll see the raw potentiometer values 
 *   being printed.
 * - Turn the potentiometer all the way to the left, and read the value. 
 *   It should print a value close to zero.  
 *   Set `minimumValue` to this printed value plus some safety margin 
 *   (add 5%, for example).  
 * - Now turn the potentiometer all the way to the right, and do the same thing
 *   for `maximumValue`.
 * 
 * Mapping
 * -------
 * 
 * Select the Arduino as a custom MIDI controller in your DAW, and use the 
 * MIDI learn option to assign the potentiometer to a function.  
 * It will send the MIDI Control Change Channel Volume parameter for channel 1.
 * 
 * Written by Pieter P, 16-06-2019  
 * https://github.com/tttapa/Control-Surface
 */

// Include the Control Surface library
#include <Control_Surface.h>

// Instantiate a MIDI over USB interface.
USBMIDI_Interface midi;

// Create a new instance of the class `CCPotentiometer`, called `potentiometer`,
// on pin A0, that sends MIDI messages with controller 7 (channel volume)
// on channel 1.
CCPotentiometer potentiometer {A0, {MIDI_CC::Channel_Volume, Channel_1}};

// The maximum value that can be measured (usually 16383 = 2¹⁴-1)
constexpr analog_t maxRawValue = CCPotentiometer::getMaxRawValue();
// The filtered value read when potentiometer is at the 0% position
constexpr analog_t minimumValue = maxRawValue / 64;
// The filtered value read when potentiometer is at the 100% position
constexpr analog_t maximumValue = maxRawValue - maxRawValue / 64;

// A mapping function to eliminate the dead zones of the potentiometer:
// Some potentiometers don't output a perfect zero signal when you move them to
// the zero position, they will still output a value of 1 or 2, and the same
// goes for the maximum position.
analog_t mappingFunction(analog_t raw) {
  // make sure that the analog value is between the minimum and maximum
  raw = constrain(raw, minimumValue, maximumValue);
  // map the value from [minimumValue, maximumValue] to [0, maxRawValue]
  return map(raw, minimumValue, maximumValue, 0, maxRawValue);
}

void setup() {
  // Add the mapping function to the potentiometer
  potentiometer.map(mappingFunction);
  // Initialize everything
  Control_Surface.begin();
  Serial.begin(115200);
}

void loop() {
  // Update the Control Surface (check whether the potentiometer's
  // input has changed since last time, if so, send the new value over MIDI).
  Control_Surface.loop();
  // Use this to find minimumValue and maximumValue: it prints the raw value
  // of the potentiometer, without the mapping function
  Serial.println(potentiometer.getRawValue());
}
