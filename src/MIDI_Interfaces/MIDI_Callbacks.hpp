#pragma once

#include <MIDI_Parsers/MIDI_MessageTypes.hpp>

BEGIN_CS_NAMESPACE

class MIDI_Interface;

// LCOV_EXCL_START

/**
 * @brief   A class for callbacks from MIDI input.
 */
class MIDI_Callbacks {
  public:
    /// Callback for incoming MIDI Channel Messages (notes, control change,
    /// pitch bend, etc.)
    virtual void onChannelMessage(MIDI_Interface &, ChannelMessage) {}
    /// Callback for incoming MIDI System Exclusive Messages.
    virtual void onSysExMessage(MIDI_Interface &, SysExMessage) {}
    /// Callback for incoming MIDI Real-Time Messages.
    virtual void onRealTimeMessage(MIDI_Interface &, RealTimeMessage) {}

    /// Destructor.
    virtual ~MIDI_Callbacks() = default;
};

// LCOV_EXCL_STOP

END_CS_NAMESPACE
