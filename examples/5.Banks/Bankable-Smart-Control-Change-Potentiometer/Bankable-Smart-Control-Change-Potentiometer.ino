/** 
 * This example introduces smart bankable potentiometers to prevent
 * values jumping around when changing banks.
 *  
 * @boards  AVR, AVR USB, Nano Every, Due, Nano 33 IoT, Nano 33 BLE, Teensy 3.x, ESP32
 * 
 * Connections
 * -----------
 * 
 * - A0: wiper of a potentiometer
 * - 2:  momentary push button (other pin to ground)
 * - 3:  momentary push button (other pin to ground)
 * 
 * Connect the left terminal of the potentiometer to ground, and the right one
 * to V<sub>CC</sub>.  
 * The internal pull-up resistors for the push buttons will be enabled.
 * 
 * Behavior
 * --------
 * 
 * If you move the potentiometer to, say, 60%, then switch to bank #2, move the
 * potentiometer to 20%, and then switch back to bank #1, the potentiometer will
 * be disabled, until you move it up to 60% again.
 * 
 * This prevents values jumping around in your DAW when cycling through the
 * different banks.
 * 
 * Changing banks is done using the two push buttons. The push button on pin 2
 * increments the bank number, the push button on pin 3 decrements the bank 
 * number.
 * 
 * Mapping
 * -------
 * 
 * Select the Arduino as a custom MIDI controller in your DAW, and use the 
 * MIDI learn option to assign the potentiometer to a function, change the  
 * bank using the buttons, and map the potentiometer again.
 * It will send the MIDI Control Change Channel Volume parameter for channels 
 * 1, 2, 3 and 4 (for banks #1, #2, #3 and #4 respectively).
 * 
 * Written by PieterP, 2020-04-09  
 * https://github.com/tttapa/Control-Surface
 */
#include <Control_Surface.h> // Include the Control Surface library

// Instantiate a MIDI over USB interface.
USBDebugMIDI_Interface midi;

// Four banks to change the channel of the potentiometer
Bank<4> bank;

// Selector to change banks
IncrementDecrementSelector<4> selector {bank, {2, 3}};

// Handy type alias
using  CCSmartPot = Bankable::CCSmartPotentiometer<4>;

// Instantiate a CCPotentiometer object
CCSmartPot potentiometer {
  {bank, BankType::CHANGE_CHANNEL},     // Bank configuration
  A0,                                   // Analog pin connected to potentiometer
  {MIDI_CC::Channel_Volume, CHANNEL_1}, // Channel volume of channel 1
};

/*
 * You can do the same for PBSmartPotentiometers (Pitch Bend)
 * 
 * // Handy type alias
 * using  PBSmartPot = Bankable::PBSmartPotentiometer<4>;
 *
 * // Instantiate a CCPotentiometer object
 * PBSmartPot potentiometer {
 *   {bank, BankType::CHANGE_CHANNEL},     // Bank configuration
 *   A0,                                   // Analog pin connected to potentiometer
 *   CHANNEL_1,                            // Channel 1
 * };
 */

void setup() {
  Control_Surface.begin(); // Initialize Control Surface
}

void loop() {
  Control_Surface.loop(); // Update the Control Surface

  // You can request the state of the smart potentiometer:
  if (potentiometer.getState() == CCSmartPot::Lower) {
    // Turn on an LED to indicate that the potentiometer
    // value is too low to be active
  } else if (potentiometer.getState() == CCSmartPot::Higher) {
    // Turn on an LED to indicate that the potentiometer 
    // value is too high to be active
  }
}
