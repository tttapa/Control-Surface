/** 
 * This example demonstrates how to use the USBHostMIDI_Interface to send and
 * receive MIDI data to and from USB MIDI devices connected to a Teensy's USB
 * Host port.
 *
 * @boards  Teensy 3.6, Teensy 4.1
 * 
 * Connections
 * -----------
 * 
 * - Use a [USB Host cable](https://www.pjrc.com/store/cable_usb_host_t36.html)
 *   to connect a MIDI USB device to the Teensy. Connect the primary USB port of
 *   the Teensy to a computer.
 * 
 * Behavior
 * --------
 * 
 * - All messages sent from the computer to the Teensy are forwarded to the
 *   USB MIDI device.
 * - All messages sent by the USB MIDI device are forwarded to the computer.
 * 
 * Mapping
 * -------
 * 
 * - None
 * 
 * Written by PieterP, 2021-01-31  
 * https://github.com/tttapa/Control-Surface
 */
#include <Control_Surface.h>
#include <MIDI_Interfaces/USBHostMIDI_Interface.hpp>

// Create the ports for USB devices plugged into Teensy's 2nd USB port (via hubs)
USBHost usb;
USBHub hub {usb};

// USB Device MIDI (primary USB port)
USBMIDI_Interface devicemidi;
// USB Host MIDI (second USB port)
USBHostMIDI_Interface hostmidi {usb};
// Alternatively, use `USBHostMIDI_Interface_BigBuffer`

// Pipe to route MIDI data between two USB ports
BidirectionalMIDI_Pipe p;

void setup() {
  // Wait 1.5 seconds before turning on USB Host. If connected USB devices
  // use too much power, Teensy at least completes USB enumeration, which
  // makes isolating the power issue easier.
  delay(1500);

  hostmidi | p | devicemidi; // Connect the interfaces using a MIDI pipe
  usb.begin();               // Initialize the USB Host
  hostmidi.begin();          // Initialize the MIDI interfaces
  devicemidi.begin();
}

void loop() {
  // Continuously read incoming MIDI data and route it between the interfaces
  hostmidi.update();
  devicemidi.update();
}
