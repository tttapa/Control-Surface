/** 
 * This is an example that demonstrates the use of a rotary encoder that can 
 * be used for scrolling through a song list, tuning effects, etc.
 *
 * ### Connections
 * 
 *   - 2: pin A of the rotary encoder
 *   - 3: pin B of the rotary encoder
 * 
 * The internal pull-up resistors will be enabled automatically.  
 * Connect the common (C) pin of the rotary encoder to ground.
 * 
 * ### Behavior
 * 
 * If the encoder is rotated clockwise, a MIDI CC increment message is sent, 
 * if the encoder is rotated counter-clockwise, a MIDI CC decrement message is 
 * sent.  
 * The controller number `MCU::V_POT_1` will be used.  
 * 
 * To change the encoding of the MIDI CC increment and decrement messages, use
 * `RelativeCCSender::setMode`.
 * 
 * Map the Arduino as a Mackie Control Universal (MCU) or equivalent in your
 * audio software.
 * 
 * Written by PieterP, 2019-02-16  
 * https://github.com/tttapa/Control-Surface
 */
#include <Encoder.h> // Include the encoder library
// This has to be done before including the Control Surface library
#include <Control_Surface.h> // Include the Control Surface library

// Instantiate a MIDI over USB interface.
USBMIDI_Interface midi;

// Instantiate a CCRotaryEncoder object that reads a rotary encoder,
// connected to pins 2 and 3.
// The CC controller is MCU::V_POT_1, the multiplier is 1,
// and the encoder sends out 4 pulses per step/click.
CCRotaryEncoder enc = {
    {2, 3},
    MCU::V_POT_1,
    1,
    4,
};

void setup() {
    // Use the Mackie Control protocol for sending relative MIDI CC messages.
    RelativeCCSender::setMode(MACKIE_CONTROL_RELATIVE);
    Control_Surface.begin(); // Initialize Control Surface
}

void loop() {
    // Update the control surface
    Control_Surface.loop();
}
