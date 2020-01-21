/**
 * MIDI routing using MIDI_Pipe%s.
 * 
 * Bridges the USB MIDI interface to a Serial MIDI interface, and loops back
 * anything it receives on the USB interface.
 * 
 * @boards  AVR USB, Due, Nano 33, Teensy 3.x, ESP32
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