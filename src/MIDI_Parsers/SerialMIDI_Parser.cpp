#include "SerialMIDI_Parser.h"
#include "../Helpers/StreamOut.h"

MIDI_read_t SerialMIDI_Parser::parse(uint8_t midiByte)
{
  if (midiByte & (1 << 7)) // If it's a header byte (first byte)
  {
    if ((midiByte & 0xF8) == 0xF8) // If it's a Real-Time message (not implemented)
    {
      ; // Handle Real-Time stuff
#ifdef DEBUG
      DEBUG << "Real-Time" << endl;
#endif
    }
    else // Normal header
    {
#ifndef IGNORE_SYSEX
      if (midimsg.header == SysExStart) // if we're currently receiving a SysEx message
      {
#ifdef DEBUG
        DEBUG << "SysExEnd" << endl;
#endif
        midimsg.header = SysExEnd;
        if (addSysExByte(SysExEnd)) // add SysExEnd byte to buffer
          return SYSEX_MESSAGE;
      }
#endif
#ifdef DEBUG
      DEBUG << "Header" << endl;
#endif
      midimsg.header = midiByte;
      thirdByte = false;

#ifndef IGNORE_SYSEX
      if (midimsg.header == SysExStart) // if the first byte of a SysEx message
      {
#ifdef DEBUG
        DEBUG << "SysExStart" << endl;
#endif
        startSysEx();
        addSysExByte(SysExStart);
      }
#endif // IGNORE_SYSEX
    }
  }
  else // If it's a data byte
  {
    if (midimsg.header == 0)
    {
#ifdef DEBUG
      DEBUG << "Error: No header" << endl;
#endif
      ; // Ignore
    }
    else if (thirdByte) // third byte of three
    {
#ifdef DEBUG
      DEBUG << "Second data byte" << endl;
#endif
      midimsg.data2 = midiByte;
#ifdef DEBUG
      DEBUG << "Message finished" << endl;
#endif
      thirdByte = false;
      return CHANNEL_MESSAGE;
    }
    else // second byte or SysEx data
    {
      if (midimsg.header < 0xC0 || midimsg.header == 0xE0) // Note, Aftertouch, CC or Pitch Bend
      {
#ifdef DEBUG
        DEBUG << "First data byte (of two)" << endl;
#endif
        midimsg.data1 = midiByte;
        thirdByte = true;
      }
      else if (midimsg.header < 0xE0) // Program Change or Channel Pressure
      {
#ifdef DEBUG
        DEBUG << "First data byte" << endl;
#endif
        midimsg.data1 = midiByte;
        return CHANNEL_MESSAGE;
      }
#ifndef IGNORE_SYSEX
      else if (midimsg.header == SysExStart) // SysEx data byte
      {
#ifdef DEBUG
        DEBUG << "SysEx data byte" << endl;
#endif
        addSysExByte(midiByte);
      }
#endif // IGNORE_SYSEX
#ifdef DEBUG
      else
      {
        DEBUG << "Data byte ignored" << endl;
      }
#endif
    }
  }
  return NO_MESSAGE;
}