#ifndef MIDI_INTERFACE_H_
#define MIDI_INTERFACE_H_

#include "../MIDI_Parsers/MIDI_Parser.h"

#define MIDI_BAUD 31250

class MIDI_Interface
{
public:
  MIDI_Interface(MIDI_Parser &parser); // Constructor (make this the default MIDI interface)

  virtual void begin() {}
  void send(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2); // Send a 3-byte MIDI packet
  void send(uint8_t m, uint8_t c, uint8_t d1);             // Send a 2-byte MIDI packet

  /* virtual bool ignoreInput() = 0; // Virtual function that is implemented by child classes,
  // if MIDI input is disabled, it should just ignore all incoming data, but clear the USB buffer, otherwise the
  // software on the computer will wait for the messages to arrive, causing it to hang */

  virtual MIDI_read_t read() = 0; // try to read a MIDI message and return what type of message is ready to read

  static MIDI_Interface *getDefault(); // static function that returns the default interface (doesn't depend on a particular MIDI interface instance )
  void setDefault();                   // Set this MIDI interface as the default interface

protected:
  virtual void sendImpl(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2) = 0;
  virtual void sendImpl(uint8_t m, uint8_t c, uint8_t d1) = 0;

  MIDI_Parser &parser;

  static MIDI_Interface *DefaultMIDI_Interface; // static pointer that points to the default MIDI interface instance

public:
  MIDI_message getChannelMessage();
  size_t getSysExLength();
  const uint8_t *getSysExBuffer();
};

#endif // MIDI_INTERFACE_H_