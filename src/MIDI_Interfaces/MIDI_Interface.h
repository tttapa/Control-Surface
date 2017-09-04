#ifndef MIDI_INTERFACE_H_
#define MIDI_INTERFACE_H_

#include "Arduino.h"

#include "Settings/Settings.h"

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

  virtual bool refresh(); // Virtual function that is implemented by child classes,
                          // it should discard MIDI data from the UART or USB buffer
                          // returns true as long as there's data to read, returns false if UART/USB buffer is empty 
                          // it should just ignore all incoming data, but clear the USB buffer, otherwise the
                          // software on the computer will wait for the messages to arrive, causing it to hang

    static MIDI_Interface *getDefault(); // static function that returns the default interface (doesn't depend on a particular MIDI interface instance )
    void setDefault();                   // Set this MIDI interface as the default interface

  protected:
    virtual void sendImpl(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2) {}
    virtual void sendImpl(uint8_t m, uint8_t c, uint8_t d1) {}

    static MIDI_Interface *DefaultMIDI_Interface; // static pointer that points to the default MIDI interface instance
  };

#endif // MIDI_INTERFACE_H_