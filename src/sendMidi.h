#ifndef sendMidi_h_
#define sendMidi_h_

#include "Arduino.h"

#if defined (CORE_TEENSY) && ! (defined (USB_MIDI_AUDIO_SERIAL) || defined (USB_MIDI) || defined (USB_MIDI_SERIAL))
#error Please select a MIDI option in the 'Tools > USB Type' menu.
#endif

const static byte NO_BLINK = 255;
const static byte NO_DELAY = 0;

const static byte NOTE_OFF = 0x80;
const static byte NOTE_ON = 0x90;

const static byte CC = 0xB0;
const static byte PROGRAM_CHANGE = 0xC0;
const static byte CHANNEL_PRESSURE = 0xD0;
const static byte PITCH_BEND = 0xE0;

class MIDISender
{
private:
    int delay;
    byte pin;
    bool blink;
    Stream midiStream;
public:
    MIDISender(byte p, int d, Stream s = null); // blink pin, delay, MIDI stream, debug stream, debug baud
    void sendMidi(byte m, byte c, byte d1, byte d2); // message, channel, data 1, data 2
    void sendMidi(byte m, byte c, byte d1); // message, channel, data 1
    void serialMidi(byte m, byte c, byte d1, byte d2, Stream s);
    void serialMidi(byte m, byte c, byte d1, Stream s);
};

class MIDIDebug
{
private:
    int delay;
    byte pin;
    bool blink;
    Stream debugStream;
public:
    MIDIDebug(byte p, int d, Stream ds = null, unsigned long baud = 9600); // blink pin, delay, MIDI stream, debug stream, debug baud
    void sendMidi(byte m, byte c, byte d1, byte d2); // message, channel, data 1, data 2
    void sendMidi(byte m, byte c, byte d1); // message, channel, data 1
};

#endif
