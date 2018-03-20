#ifndef MIDI_PARSER_H
#define MIDI_PARSER_H

#ifdef ARDUINO
#include <Arduino.h>
#else 
#include <stdint.h>
#include <cstddef>
#include <cstdlib>
#endif

#include "../Settings/Settings.h"

const uint8_t NOTE_OFF = 0x80;
const uint8_t NOTE_ON = 0x90;
const uint8_t KEY_PRESSURE = 0xA0;
const uint8_t CC = 0xB0, CONTROL_CHANGE = CC;
const uint8_t PROGRAM_CHANGE = 0xC0;
const uint8_t CHANNEL_PRESSURE = 0xD0;
const uint8_t PITCH_BEND = 0xE0;

const uint8_t SysExStart = 0xF0;
const uint8_t SysExEnd = 0xF7;

struct MIDI_message
{
  uint8_t header = 0;
  uint8_t data1 = 0;
  uint8_t data2 = 0;
};

enum MIDI_read_t
{
  NO_MESSAGE = 0,
  CHANNEL_MESSAGE = 1,
  SYSEX_MESSAGE = 2,
};

// ------------------------------------------------------------------------------------------------------------------------

class MIDI_Parser {
  public:
    // virtual MIDI_read_t parse(uint8_t *packet) { return NO_MESSAGE; }
    // virtual MIDI_read_t parse(uint8_t midibyte) { return NO_MESSAGE; }
    MIDI_message getChannelMessage();
    const uint8_t *getSysExBuffer();
    size_t getSysExLength();

  protected:
    const static size_t bufferSize = 128; // max SysEx for Mackie Control Universal is 120 bytes
    uint8_t SysExBuffer[bufferSize];
    size_t SysExLength = 0;
    bool receivingSysEx = false;
    MIDI_message midimsg;
    
    void startSysEx();
    void endSysEx();
    bool addSysExByte(uint8_t data);
    bool hasSpaceLeft();
};

#endif // MIDI_PARSER_H