#include "USBMIDI_Parser.h"
#include "../Settings/SettingsWrapper.h"
#include "../Helpers/StreamOut.h"

MIDI_read_t USBMIDI_Parser::parse(uint8_t *packet)
{
#ifdef DEBUG
  DEBUG_OUT << "MIDIUSB packet:\t" << hex
        << packet[0] << ' ' << packet[1] << ' ' << packet[2] << ' ' << packet[3] << dec << endl;
#endif

  uint8_t CIN = (uint8_t)packet[0] << 4; // MIDI USB cable index number

  if (CIN >= NOTE_OFF && CIN <= PITCH_BEND) // 2- or 3-byte MIDI event
  {
    // uint8_t type = packet[1] & 0xF0;
    // if (CIN != type) // invalid MIDI USB packet
    //    return NO_MESSAGE;
    midimsg.header = packet[1];
    midimsg.data1 = packet[2];
    midimsg.data2 = packet[3];
    return CHANNEL_MESSAGE;
  }

#ifndef IGNORE_SYSEX
  else if (CIN == 0x40) // SysEx starts or continues (3 bytes)
  {
    if (packet[1] == SysExStart)
      startSysEx(); // start a new message (overwrite previous unfinished message)
    else if (!receivingSysEx) // If we haven't received a SysExStart
    {
#ifdef DEBUG
      DEBUG_OUT << "Error: No SysExStart received" << endl;
#endif
      return NO_MESSAGE; // ignore the data
    }
    addSysExByte(packet[1]) && // add three data bytes to buffer
    addSysExByte(packet[2]) && 
    addSysExByte(packet[3]);
    return NO_MESSAGE; // SysEx is not finished yet
  }

  else if (CIN == 0x50) // SysEx ends with following single byte (or Single-byte System Common Message, not implemented)
  {
    if (packet[1] != SysExEnd) // System Common (not implemented)
    {
      return NO_MESSAGE;
    }
    else if (!receivingSysEx) // If we haven't received a SysExStart
    {
#ifdef DEBUG
      DEBUG_OUT << "Error: No SysExStart received" << endl;
#endif
      return NO_MESSAGE; // ignore the data
    }
    if (
      addSysExByte(SysExEnd)
    ) {
      endSysEx();
      return SYSEX_MESSAGE;
    } else 
      return NO_MESSAGE;
  }

  else if (CIN == 0x60) // SysEx ends with following two bytes
  {
    if (packet[1] == SysExStart)
      startSysEx(); // start a new message (overwrite previous unfinished message)
    else if (!receivingSysEx) // If we haven't received a SysExStart
    {
#ifdef DEBUG
      DEBUG_OUT << "Error: No SysExStart received" << endl;
#endif
      return NO_MESSAGE; // ignore the data
    }
    if (  // add two data bytes to buffer
      addSysExByte(packet[1]) &&
      addSysExByte(SysExEnd)
    ) {
      endSysEx();
      return SYSEX_MESSAGE;
    } else 
      return NO_MESSAGE;
  }

  else if (CIN == 0x70) // SysEx ends with following three bytes
  {
    if (packet[1] == SysExStart)
      startSysEx(); // start a new message (overwrite previous unfinished message)
    else if (!receivingSysEx) // If we haven't received a SysExStart
    {
#ifdef DEBUG
      DEBUG_OUT << "Error: No SysExStart received" << endl;
#endif
      return NO_MESSAGE; // ignore the data
    }
    if ( // add three data bytes to buffer
      addSysExByte(packet[1]) && 
      addSysExByte(packet[2]) &&
      addSysExByte(SysExEnd)
    ) {
      endSysEx();
      return SYSEX_MESSAGE;
    }
    else
      return NO_MESSAGE;
  }
#endif // IGNORE_SYSEX

  /*
    else if (CIN == 0x00) // Miscellaneous function codes. Reserved for future extensions. (not implemented)
      ;
    else if (CIN == 0x10) // Cable events. Reserved for future expansion. (not implemented)
      ;
    else if (CIN == 0x20) // Two-byte System Common message (not implemented)
      ;
    else if (CIN == 0x30) // Three-byte System Common message (not implemented)
      ;
    else if (CIN == 0xF0) // Single Byte (not implemented)
      ;
  */

  return NO_MESSAGE;
}