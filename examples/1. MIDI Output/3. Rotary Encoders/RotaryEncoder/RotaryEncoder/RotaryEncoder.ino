/** 
 * This example demonstrates the use of relative MIDI CC rotary encoders that
 * can be used for changing effect parameters, volumes, pan and balance
 * controls, etc.
 *
 * @boards  AVR, AVR USB, Nano Every, Due, Nano 33 IoT, Nano 33 BLE, Teensy 3.x
 * 
 * Connections
 * -----------
 * 
 * - 2: pin A of the encoder
 * - 3: pin B of the encoder
 * 
 * Connect the common terminal of the encoder to ground. The built-in pullup
 * resistors will be enabled.
 * 
 * Behavior
 * --------
 * 
 * - When you turn the encoder, you should receive incremental MIDI Control
 *   Change messages.
 * 
 * Mapping
 * -------
 * 
 * Select the Arduino as a custom MIDI controller in your DAW, and use the 
 * relative CC mode.  
 * There are three main modes for sending relative messages, make sure you
 * use the same one in this sketch as in your DAW.
 * 
 * @see relativeCCmode
 * 
 * Written by PieterP, 2019-12-18  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h> // Include the Control Surface library

// Instantiate a MIDI over USB interface.
USBMIDI_Interface midi;

// Instantiate a CCRotaryEncoder object
CCRotaryEncoder enc {
  {2, 3},       // pins
  MCU::V_POT_1, // MIDI address (CC number + optional channel)
  1,            // optional multiplier if the control isn't fast enough
};

void setup() {
  // Select the correct relative MIDI CC mode.
  // Options:
  //   - TWOS_COMPLEMENT (default)
  //   - BINARY_OFFSET
  //   - SIGN_MAGNITUDE
  //   - NEXT_ADDRESS
  // Aliases:
  //   - REAPER_RELATIVE_1
  //   - REAPER_RELATIVE_2
  //   - REAPER_RELATIVE_3
  //   - TRACKTION_RELATIVE
  //   - MACKIE_CONTROL_RELATIVE
  //   - KORG_KONTROL_INC_DEC_1
  RelativeCCSender::setMode(relativeCCmode::MACKIE_CONTROL_RELATIVE);
  Control_Surface.begin(); // Initialize Control Surface
}

void loop() {
  Control_Surface.loop(); // Update the Control Surface
}
