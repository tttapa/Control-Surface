#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <Def/Def.hpp>
#include <Settings/SettingsWrapper.hpp>

#include "MIDIReadEvent.hpp"
#include "MIDI_MessageTypes.hpp"

BEGIN_CS_NAMESPACE

/// Base class for MIDI parsers.
class MIDI_Parser {
  public:
    /// Get the latest MIDI channel voice message.
    ChannelMessage getChannelMessage() const { return ChannelMessage(midimsg); }
    /// Get the latest MIDI system common message.
    SysCommonMessage getSysCommonMessage() const {
        return SysCommonMessage(midimsg);
    }
    /// Get the latest MIDI real-time message.
    RealTimeMessage getRealTimeMessage() const { return rtmsg; }
#if IGNORE_SYSEX
    /// Get the latest SysEx message.
    SysExMessage getSysExMessage() const { return {nullptr, 0, Cable_1}; }
#endif

  protected:
    MIDIMessage midimsg = {0x00, 0x00, 0x00};
    RealTimeMessage rtmsg = {0x00};

  public:
    /// Check if the given byte is a MIDI header/status byte.
    static bool isStatus(uint8_t data) { return data & (1 << 7); }
    /// Check if the given byte is a MIDI data byte.
    static bool isData(uint8_t data) { return (data & (1 << 7)) == 0; }
};

END_CS_NAMESPACE