/**
 * MIDI routing using MIDI_Pipe%s.
 * 
 * Bridges the USB MIDI interface to a Serial MIDI interface, and loops back
 * anything it receives on the USB interface.
 * 
 * You can also use the MIDI pipes to route the input and output of Control 
 * Surface itself, not just the input and output of MIDI interfaces. This is
 * demonstrated in the @ref Dual-MIDI-Interface.ino example.
 * 
 * @boards  AVR USB, Due, Nano 33 IoT, Nano 33 BLE, Teensy 3.x, ESP32
 */

#include <Control_Surface.h>

USBMIDI_Interface midiusb;
HardwareSerialMIDI_Interface midiser = Serial1;

MIDI_PipeFactory<3> pipes;

void setup() {
  midiusb >> pipes >> midiser; // all incoming midi from USB is sent to serial
  midiusb << pipes << midiser; // all incoming midi from Serial is sent to USB
  midiusb >> pipes >> midiusb; // all incoming midi from USB is looped back

  midiusb.begin();
  midiser.begin();
}

void loop() {
  midiusb.update();
  midiser.update();
}