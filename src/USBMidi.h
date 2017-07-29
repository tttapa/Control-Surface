#ifndef USBMidi_h_
#define USBMidi_h_

#include "Arduino.h"

#if defined (CORE_TEENSY) && ! (defined (USB_MIDI_AUDIO_SERIAL) || defined (USB_MIDI) || defined (USB_MIDI_SERIAL))
#error Please select a MIDI option in the 'Tools > USB Type' menu.
#endif

#define MIDI_BAUD 31250

const uint8_t NOTE_OFF = 0x80;
const uint8_t NOTE_ON = 0x90;
const uint8_t KEY_PRESSURE = 0xA0;
const uint8_t CC = 0xB0;
const uint8_t PROGRAM_CHANGE = 0xC0;
const uint8_t CHANNEL_PRESSURE = 0xD0;
const uint8_t PITCH_BEND = 0xE0;

class USBMidi
{
private:
    bool _blinkEn = false;
    uint8_t _blinkPin;
    int _blinkDelay = 0;
    bool _debug = false;
public:
    USBMidi(); // constructor
    ~USBMidi(); // destructor
    void begin(unsigned long baud = MIDI_BAUD, bool debug = false);
    void send(byte m, byte c, byte d1, byte d2); // message, channel, data 1, data 2
    void send(byte m, byte c, byte d1); // message, channel, data 1
    void setDelay(int d); // delay between messages
    void blink(byte p); // pin
    void noBlink();
};

extern USBMidi USBMidiController;
#endif
