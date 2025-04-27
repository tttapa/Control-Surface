/** 
 * This example shows how to dynamically enable or disable specific MIDI
 * elements.
 *
 * @boards  AVR, AVR USB, Nano Every, Due, Nano 33 IoT, Nano 33 BLE, UNO R4, Pi Pico, Teensy 3.x, ESP32, ESP8266
 *
 * Connections
 * -----------
 *
 * - 2: momentary push button (to ground)
 * - A0: wiper of a potentiometer
 *
 * The internal pull-up resistor for the button will be enabled automatically.
 * Connect the left terminal of the potentiometer to ground, and the right one
 * to V<sub>CC</sub>.
 *
 * Behavior
 * --------
 *
 * At program start-up, the potentiometer is enabled, and it sends MIDI Pitch
 * Bend messages when its position changes.
 * By pressing the push button, the potentiometer is disabled (this is indicated
 * by the on-board LED turning off). When disabled, Control Surface stops
 * sampling the analog input of the potentiometer, and no Pitch Bend messages
 * are sent for this MIDI output element (any other elements are unaffected).
 * Pressing the push button again enables the potentiometer again.
 * 
 * Written by PieterP, 2018-07-26  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h>

USBMIDI_Interface midi;

Button enableButton = 2; // Momentary push button between pin 2 and ground
PBPotentiometer masterVolume {A0, Channel_9}; // Potentiometer to pin A0

void setup() {
  enableButton.begin();
  Control_Surface.begin();
  // Note: if you disable an element before calling Control_Surface.begin(), you
  // are responsible for initializing it manually (using the begin() method of
  // that element).
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  if (enableButton.getState() == Button::Falling) {
    static bool state = true;
    state = !state;
    digitalWrite(LED_BUILTIN, state);
    state ? masterVolume.enable() : masterVolume.disable();
  }
  Control_Surface.loop();
}
