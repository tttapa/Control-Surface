#ifndef sendMidi_h_
#define sendMidi_h_

#include "Arduino.h"

#if defined (CORE_TEENSY) && ! (defined (USB_MIDI_AUDIO_SERIAL) || defined (USB_MIDI) || defined (USB_MIDI_SERIAL))
#error Please select a MIDI option in the 'Tools > USB Type' menu.
#endif

#define MIDI_BAUD 31250

const static byte NOTE_OFF = 0x80;
const static byte NOTE_ON = 0x90;

const static byte CC = 0xB0;
const static byte PROGRAM_CHANGE = 0xC0;
const static byte CHANNEL_PRESSURE = 0xD0;
const static byte PITCH_BEND = 0xE0;

class USBMidi
{    
public:
    USBMidi();
    void send(byte m, byte c, byte d1, byte d2);
    void send(byte m, byte c, byte d1);
};

extern USBMidi USB;
#endif