#include "MIDI_Parser.h"
#include "../Settings/SettingsWrapper.h"

void MIDI_Parser::startSysEx() // start adding a new SysEx message to the buffer
{
  SysExLength = 0; // if the previous message wasn't finished, overwrite it
  receivingSysEx = true;
  DEBUG("Start SysEx");
}

void MIDI_Parser::endSysEx() // stop receiving SysEx message
{
  receivingSysEx = false;
#ifdef DEBUG
  DEBUG("End SysEx");
#endif
}

bool MIDI_Parser::addSysExByte(uint8_t data) // add a byte to the current SysEx message
{
  if (!hasSpaceLeft()) // if the buffer is full
    return false;
  SysExBuffer[SysExLength] = data; // add the data to the buffer
  SysExLength++;
  DEBUG("SysEx data byte: " << hex << data << dec << '\t' << "SysExLength = " << SysExLength);
  return true;
}

bool MIDI_Parser::hasSpaceLeft() // check if the buffer has at least 1 byte of free space available
{
  bool avail = SysExLength < bufferSize;
  if (!avail)
    DEBUG("SysEx buffer full");
  return avail;
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