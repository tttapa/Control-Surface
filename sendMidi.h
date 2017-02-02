#ifndef sendMidi_h_
#define sendMidi_h_

#include "Arduino.h"
#include "debug.h"

const static byte NO_BLINK = 255;
const static byte NO_DELAY = 0;

const static byte NOTE_OFF = 0x80;
const static byte NOTE_ON = 0x90;

const static byte CC = 0xB0;
const static byte PROGRAM_CHANGE = 0xC0;
const static byte CHANNEL_PRESSURE = 0xD0;
const static byte PITCH_BEND = 0xE0;

void sendMidi(byte m, byte c, byte d1, byte d2);
void sendMidi(byte m, byte c, int d1);
void setupMidi(byte p, int d, bool db = false);

#endif
