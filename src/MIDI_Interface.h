#ifndef MIDI_INTERFACE_H_
#define MIDI_INTERFACE_H_

#include "Arduino.h"

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

class MIDI_Interface
{
public:
  MIDI_Interface(); // Constructor (make this the default MIDI interface)

  virtual void begin() {}
  void send(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2); // Send a 3-byte MIDI packet
  void send(uint8_t m, uint8_t c, uint8_t d1);             // Send a 2-byte MIDI packet

  virtual bool refresh();              // Virtual function that is implemented by child classes,
                                       // it should take MIDI data from the UART or USB buffer and add it to the MIDI buffer,
                                       // returns true as long as there's data to read, returns false if UART/USB buffer is empty or if MIDI buffer is full
  void startMessage();                 // start adding a new message to the buffer
  bool addToMessage(uint8_t data);     // add a byte to the current message (and increment write index)
  bool hasSpaceLeft(size_t bytes = 1); // check if the buffer has at least 'bytes' bytes of free space available
  void finishMessage();                // finish the message

  uint8_t read();          // read a byte and move the read index to the next byte
  uint8_t peek();          // read a byte without incrementing the read index
  uint8_t getNextHeader(); // read until the first header byte and return it if it exists, otherwise, return zero
  size_t available();      // returns the number of MIDI messages in the buffer

  static MIDI_Interface *getDefault(); // static function that returns the default interface (doesn't depend on a particular MIDI interface instance )

  void setDefault(); // Set this MIDI interface as the default interface

protected:
  virtual void sendImpl(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2) {}
  virtual void sendImpl(uint8_t m, uint8_t c, uint8_t d1) {}

  const static size_t bufferSize = 128; // max SysEx for Mackie Control Universal is 120 bytes
  uint8_t buffer[bufferSize];
  size_t writeIndex = 0, readIndex = 0, messageStartIndex = 0;
  size_t messages = 0;
  size_t SysExLength = 0;

  size_t mod(int a, int b); // modulo operator that works on negative numbers
  void incrementWriteIndex();
  void incrementReadIndex();
  inline bool isHeader(); // returns true if the read index points to a MIDI header byte and not to a SysExEnd byte or data byte

  static MIDI_Interface *DefaultMIDI_Interface; // static pointer that points to the default MIDI interface instance
};

extern void sendMIDI(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2) __attribute__((deprecated));
extern void sendMIDI(uint8_t m, uint8_t c, uint8_t d1) __attribute__((deprecated));
extern void startMIDI() __attribute__((deprecated));
extern bool refreshMIDI() __attribute__((deprecated));
extern size_t availableMIDI() __attribute__((deprecated));
extern uint8_t readMIDI() __attribute__((deprecated));

#endif // MIDI_INTERFACE_H_