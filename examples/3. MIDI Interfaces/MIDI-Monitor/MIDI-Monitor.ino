/** 
 * This example prints all MIDI messages that arrive over USB to the Serial
 * monitor so you can use the Arduino as a MIDI monitor. It also allows you to
 * enter MIDI messages as text in the Serial monitor, and then sends these
 * messages as MIDI over USB.
 *
 * @boards  AVR USB, Due, Nano 33 IoT, Nano 33 BLE, UNO R4, Pi Pico, Teensy 3.x
 *
 * This example requires an Arduino board with native MIDI over USB support.
 *
 * To send a MIDI message from the Serial monitor, select “Newline” as the line 
 * ending, enter the MIDI message in hexadecimal, and press enter.  
 * For example, if you enter `90 3C 7F`, a MIDI Note On message for middle C 
 * (0x3C) with a velocity of 127 (0x7F) will be sent over USB.  
 * Spaces between bytes are not required (`903C7F` will be interpreted as 
 * `90 3C 7F`), but they are recommended to prevent framing errors. 
 *
 * @see @ref MIDI_Pipes-Routing.ino
 * @see @ref Debug-MIDI-Interface.ino
 * @see @ref midi_md-routing "MIDI Tutorial: Routing MIDI messages"
 * 
 * Written by PieterP, 2021-06-28  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h> // Include the Control Surface library

// Instantiate the two MIDI interfaces
USBMIDI_Interface midiusb;
USBDebugMIDI_Interface mididbg;

// Instantiate a MIDI pipe to connect the two interfaces
BidirectionalMIDI_Pipe mpipe;

void setup() {
  // Create a bidirectional route between the interfaces:
  // all input to one interface is sent to the output of the other
  midiusb | mpipe | mididbg;
  // Initialize all MIDI interfaces
  MIDI_Interface::beginAll();
}

void loop() {
  // Continuously handle MIDI input
  MIDI_Interface::updateAll();
}
