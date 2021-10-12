/** 
 * This example demonstrates the use of absolute MIDI CC rotary encoders that
 * can be used for changing effect parameters, volumes, pan and balance
 * controls, etc.
 * 
 * The absolute here means that the Arduino keeps track of the position of the 
 * encoder, and it sends this absolute position over MIDI.  
 * Usually, encoders send relative messages ("5 ticks to the left", 
 * "1 tick to the right").
 * 
 * Similar to the CCAbsoluteEncoder, there's also PBAbsoluteEncoder that sends
 * absolute MIDI Pitch Bend messages.
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
 * - When you turn the encoder, you should receive absolute MIDI Control Change
 *   messages with the absolute position of the encoder.
 * 
 * Mapping
 * -------
 * 
 * Select the Arduino as a custom MIDI controller in your DAW, and map it like 
 * a normal CC controller (not a relative one).
 * 
 * Written by PieterP, 2019-12-18  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h> // Include the Control Surface library

// Instantiate a MIDI over USB interface.
USBMIDI_Interface midi;

// Instantiate a CCAbsoluteEncoder object
CCAbsoluteEncoder enc {
  {2, 3},       // pins
  MIDI_CC::Pan, // MIDI address (CC number + optional channel)
  1,            // optional multiplier if the control isn't fast enough
};

// Similarly, for Pitch Bend
//    PBAbsoluteEncoder enc {
//      {2, 3},    // pins
//      CHANNEL_1, // MIDI channel
//      127,       // large multiplier because Pitch Bend has high resolution
//    };


void setup() {
  Control_Surface.begin(); // Initialize Control Surface
}

void loop() {
  Control_Surface.loop(); // Update the Control Surface
}
