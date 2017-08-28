#ifndef MIDI_INTERFACE_H_
#define MIDI_INTERFACE_H_

#include "Arduino.h"

#define MIDI_BAUD 31250
#define HAIRLESS_BAUD 115200

const uint8_t NOTE_OFF = 0x80;
const uint8_t NOTE_ON = 0x90;
const uint8_t KEY_PRESSURE = 0xA0;
const uint8_t CC = 0xB0;
const uint8_t PROGRAM_CHANGE = 0xC0;
const uint8_t CHANNEL_PRESSURE = 0xD0;
const uint8_t PITCH_BEND = 0xE0;

const uint8_t SysExEnd = 0xF0;

struct MIDI_event
{
  uint8_t header;
  uint8_t data1;
  uint8_t data2;
};

class MIDI_Interface
{
public:
  MIDI_Interface()
  {
    DefaultMIDI_Interface = this;
  }
  virtual void begin();
  virtual bool refresh();
  void send(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2); // Send a 3-byte MIDI packet
  void send(uint8_t m, uint8_t c, uint8_t d1);             // Send a 2-byte MIDI packet
  uint8_t read();
  uint8_t peek();
  uint8_t getNextHeader();
  size_t available();

  static MIDI_Interface *getDefault()
  {
    return DefaultMIDI_Interface;
  }

  void setDefault() {
    DefaultMIDI_Interface = this;
  }

protected:
  virtual void sendImpl(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2);
  virtual void sendImpl(uint8_t m, uint8_t c, uint8_t d1);
  size_t mod(size_t a, size_t b); // modulo operator that works on negative numbers
  static MIDI_Interface *DefaultMIDI_Interface;
  const static size_t bufferSize = 64;
  uint8_t ringbuffer[bufferSize];
  size_t writeIndex = 0, readIndex = 0, availableMIDIevents = 0;
  
  void incrementWriteIndex(size_t incr);
  void incrementReadIndex(size_t incr);
  inline bool isHeader(uint8_t data);
};

extern void sendMIDI(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2);
extern void sendMIDI(uint8_t m, uint8_t c, uint8_t d1);
extern void startMIDI();
extern bool refreshMIDI();
extern size_t availableMIDI();
extern uint8_t readMIDI();

#endif // MIDI_INTERFACE_H_