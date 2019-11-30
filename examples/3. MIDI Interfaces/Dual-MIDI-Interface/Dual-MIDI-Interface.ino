/** 
 * This example demonstrates how to group together two or more MIDI interfaces,
 * so you can send the MIDI output over both MIDI USB and 5-pin DIN MIDI, for 
 * example. MIDI input is received from all interfaces as well.
 *
 * @boards  AVR USB, Due, Teensy 3.x
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
 * - When you turn the potentiometer, you should receive MIDI Control Change
 *   events on both the Serial MIDI interface and the USB MIDI interface.
 * - When a MIDI Note event for middle C is received on either MIDI interface,
 *   the state of the built-in LED will change accordingly.
 * 
 * Mapping
 * -------
 * 
 * - None
 * 
 * Written by PieterP, 2019-08-22  
 * https://github.com/tttapa/Control-Surface
 */
#include <Control_Surface.h>

// Create two MIDI interfaces
USBMIDI_Interface usbmidi;
HardwareSerialMIDI_Interface serialmidi = {Serial1, MIDI_BAUD};

// And group them together
MultiMIDI_Interface<2> midi = {{&usbmidi, &serialmidi}};

// The MIDI interface that was instantiated last will be used as the
// default MIDI interface by Control_Surface.
// To override the default, you can use the following line in your setup:
//    `midi.setAsDefault();`
// where `midi` is the MIDI interface you want to set as the default.

// Add some MIDI elements to show that the MIDI interfaces actually work
CCPotentiometer pot = {A0, MIDI_CC::General_Purpose_Controller_1};
NoteValueLED led = {LED_BUILTIN, 0x3C};

void setup() {
  Control_Surface.begin();
}

void loop() {
  Control_Surface.loop();
}
