#include "MIDI_Parser.h"
#include "../Helpers/StreamOut.h"

void MIDI_Parser::startSysEx() // start adding a new SysEx message to the buffer
{
  SysExLength = 0; // if the previous message wasn't finished, overwrite it
  receivingSysEx = true;
#ifdef DEBUG
  DEBUG << "Start SysEx" << endl
        << "SysExLength = 0" << endl;
#endif
}

void MIDI_Parser::endSysEx() // stop receiving SysEx message
{
  receivingSysEx = false;
#ifdef DEBUG
  DEBUG << "End SysEx" << endl;
#endif
}

bool MIDI_Parser::addSysExByte(uint8_t data) // add a byte to the current SysEx message
{
  if (!hasSpaceLeft()) // if the buffer is full
    return false;
  SysExBuffer[SysExLength] = data; // add the data to the buffer
  SysExLength++;
#ifdef DEBUG
  DEBUG << "SysEx data byte: " << hex << (int)data << dec << tab << "SysExLength = " << SysExLength << endl; // TODO: BUG: data is not printed as hex
#endif
  return true;
}

bool MIDI_Parser::hasSpaceLeft() // check if the buffer has at least 1 byte of free space available
{
#ifdef DEBUG
  bool avail = SysExLength < bufferSize;
  if (!avail)
    DEBUG << "Buffer full" << endl;
  return avail;
#else
  return SysExLength < bufferSize;
#endif
}

MIDI_message MIDI_Parser::getChannelMessage()
{
  return midimsg;
}
const uint8_t *MIDI_Parser::getSysExBuffer()
{
  return SysExBuffer;
}
size_t MIDI_Parser::getSysExLength()
{
  return SysExLength;
}