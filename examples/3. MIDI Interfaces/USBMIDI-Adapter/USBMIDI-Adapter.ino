/**
 * Turns an Arduino into a USB to 5-pin DIN MIDI adapter.
 * 
 * @boards  AVR USB, Due, Nano 33 IoT, Nano 33 BLE, Pi Pico, ESP32-S3, Teensy 3.x
 *
 * Connections
 * -----------
 * 
 * - TXD: connected to a MIDI 5-pin DIN output connector
 *     (with series resistor, possibly through a buffer)
 * - RXD: connected to a MIDI 5-pin DIN input connector
 *     (with an optocoupler)
 *
 * See https://midi.org/specifications/midi-transports-specifications/5-pin-din-electrical-specs
 * for the schematic, optocoupler models and resistor values.
 *
 * Behavior
 * --------
 *
 * - The Arduino will show up as a USB MIDI device.
 * - Any MIDI messages sent to the Arduino over USB are sent out to the 5-pin
 *     DIN output connector.
 * - Any MIDI messages sent to the Arduino through the 5-pin DIN input connector
 *     are sent over USB.
 *
 * @see @ref md_pages_MIDI-over-USB
 * @see @ref midi-tutorial
 * 
 * Written by PieterP, 2024-01-21  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h>

// Instantiate a MIDI over USB interface
USBMIDI_Interface midi_usb;
// Instantiate a 5-pin DIN MIDI interface (on the TX and RX pins of Serial1)
HardwareSerialMIDI_Interface midi_ser {Serial1};
// Instantiate the pipe to connect the two interfaces
BidirectionalMIDI_Pipe pipes;

void setup() {
  // Manually route MIDI input from the serial interface to the USB interface,
  // and the MIDI input from the USB interface to the serial interface
  midi_ser | pipes | midi_usb;
  // Initialize the MIDI interfaces
  MIDI_Interface::beginAll();
}

void loop() {
  // Continuously poll all interfaces and route the traffic between them
  MIDI_Interface::updateAll();
}
