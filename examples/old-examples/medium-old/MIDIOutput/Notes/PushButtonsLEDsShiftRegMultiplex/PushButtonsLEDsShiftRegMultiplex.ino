/**
 * @brief   This example demonstrates the use of push buttons and LEDs and how 
 *          to use shift registers and analog multiplexers to save pins.  
 * 
 * Connections:  
 * 
 *   - 10: 74HC595 ST_CP
 *   - 12: 74HC595 DS
 *   - 13: 74HC595 SH_CP
 *   - 18: 74HC4051 A (COM OUT/IN)
 *   - 19: 74HC4051 S0
 *   - 20: 74HC4051 S1
 *   - 21: 74HC4051 S2
 * 
 * Connect eight momentary push buttons between the input pins of the 
 * multiplexer and ground.  
 * The internal pull-up resistor for the buttons will be enabled automatically.
 * 
 * Connect eight LEDs (and series resistors) between the eight outputs of the
 * shift register and ground. 
 * 
 * Remember to connect the enable pins of both the multiplexer and the shift
 * register to ground in order to enable them.
 *  
 * Pressing the first button will mute the first track, and will turn on the 
 * first LED.  
 * Pressing the second button will mute the second track, and will turn on the 
 * second LED, and so on.
 * 
 * Map the Arduino as a Mackie Control Universal (MCU) or equivalent in your
 * audio software.
 * 
 * Written by PieterP, 2018-08-28
 * https://github.com/tttapa/Control-Surface
 */

// Include the Control Surface library.
#include <Control_Surface.h>

// Instantiate a MIDI over USB interface.
USBMIDI_Interface midi;

// Instantiate a multiplexer with three address lines.
AnalogMultiplex<3> mux = { 18, {19, 20, 21} };

const pin_t latchPin = 10;  // Pin connected to ST_CP of 74HC595
const pin_t dataPin  = 12;  // Pin connected to DS of 74HC595
const pin_t clockPin = 13;  // Pin connected to SH_CP of 74HC595

// Instantiate a shift register.
ShiftRegisterOut<8> sreg = { dataPin, clockPin, latchPin, MSBFIRST };

// Instantiate the momentary push buttons that send out MIDI Note events.
NoteButton buttons[] = {
    { mux.pin(0), MCU::MUTE_1 }, // digital input pin, note number, [channel]
    { mux.pin(1), MCU::MUTE_2 },
    { mux.pin(2), MCU::MUTE_3 },
    { mux.pin(3), MCU::MUTE_4 },
    { mux.pin(4), MCU::MUTE_5 },
    { mux.pin(5), MCU::MUTE_6 },
    { mux.pin(6), MCU::MUTE_7 },
    { mux.pin(7), MCU::MUTE_8 },
};

// Instantiate the LEDs that will display the state of the incoming MIDI Note
// events.
MIDINoteLED leds[] = {
    { sreg.pin(0), MCU::MUTE_1 }, // digital input pin, note number, [channel]
    { sreg.pin(1), MCU::MUTE_2 },
    { sreg.pin(2), MCU::MUTE_3 },
    { sreg.pin(3), MCU::MUTE_4 },
    { sreg.pin(4), MCU::MUTE_5 },
    { sreg.pin(5), MCU::MUTE_6 },
    { sreg.pin(6), MCU::MUTE_7 },
    { sreg.pin(7), MCU::MUTE_8 },
};

void setup() {
    // Initialize everything.
    Control_Surface.begin();
}

void loop() {
    // Update the control surface: read new MIDI events, and read all inputs.
    Control_Surface.loop();
}