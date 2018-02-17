#ifndef MIDI_INTERFACE_H_
#define MIDI_INTERFACE_H_

#include "Arduino.h"

#include "../Settings/Settings.h"

#define MIDI_BAUD 31250
#define HAIRLESS_BAUD 115200

// #define IGNORE_SYSEX

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

class MIDI_Interface
{
public:
  MIDI_Interface(); // Constructor (make this the default MIDI interface)

  virtual void begin() {}
  void send(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2); // Send a 3-byte MIDI packet
  void send(uint8_t m, uint8_t c, uint8_t d1);             // Send a 2-byte MIDI packet

  /* virtual bool ignoreInput() = 0; // Virtual function that is implemented by child classes,
  // if MIDI input is disabled, it should just ignore all incoming data, but clear the USB buffer, otherwise the
  // software on the computer will wait for the messages to arrive, causing it to hang */

#ifndef NO_MIDI_INPUT
  virtual MIDI_read_t read() = 0; // try to read a MIDI message and return what type of message is ready to read
  void startSysEx();
  bool addSysExByte(uint8_t data);
  bool hasSpaceLeft();
#endif                            // #ifndef NO_MIDI_INPUT

  static MIDI_Interface *getDefault(); // static function that returns the default interface (doesn't depend on a particular MIDI interface instance )
  void setDefault();                   // Set this MIDI interface as the default interface

protected:
  virtual void sendImpl(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2) = 0;
  virtual void sendImpl(uint8_t m, uint8_t c, uint8_t d1) = 0;

#ifndef NO_MIDI_INPUT
  const static size_t bufferSize = MIDI_BUFFER_SIZE; // max SysEx for Mackie Control Universal is 120 bytes
  uint8_t SysExBuffer[bufferSize];
  size_t SysExLength = 0;
  MIDI_message midimsg;
#endif // #ifndef NO_MIDI_INPUT

  static MIDI_Interface *DefaultMIDI_Interface; // static pointer that points to the default MIDI interface instance

public:
  const uint8_t *SysEx = &SysExBuffer[0];
  const MIDI_message *ChannelMessage = &midimsg;
};

#endif // MIDI_INTERFACE_H_