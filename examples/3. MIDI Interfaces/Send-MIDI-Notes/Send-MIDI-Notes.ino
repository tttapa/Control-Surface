/**
 * Example showing how to send MIDI Note messages.
 * 
 * @boards  AVR, AVR USB, Due, Nano 33, Teensy 3.x, ESP32
 */

#include <Control_Surface.h>

// Instantiate the MIDI over USB interface
USBMIDI_Interface midi;

// Push button connected between pin 2 and ground.
// Note message is sent when pressed.
Button pushbutton = {2};

using namespace MIDI_Notes;

// MIDI address of the note to send
const MIDICNChannelAddress noteAddress = {note(C, 4), CHANNEL_1};
// The velocity of the note events
const uint8_t velocity = 0x7F;

void setup() {
  pushbutton.begin(); // enables internal pull-up
  midi.begin();
}

void loop() {
  midi.update();
  pushbutton.update();
  if (pushbutton.getState() == Button::Falling)
    midi.sendNoteOn(noteAddress, velocity);
  else if (pushbutton.getState() == Button::Rising)
    midi.sendNoteOff(noteAddress, velocity);
}
