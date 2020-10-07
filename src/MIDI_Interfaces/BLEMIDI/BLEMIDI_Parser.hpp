#pragma once

#include <AH/Error/Error.hpp>

#include <AH/STL/utility> // std::forward

BEGIN_CS_NAMESPACE

template <class MIDIParser>
class BLEMIDI_Parser_ {
  public:
    BLEMIDI_Parser_(MIDIParser midiparser) 
        : midiparser(std::forward<MIDIParser>(midiparser)) {}

    bool operator()(const uint8_t *data, size_t len) {
        // Need at least two bytes to be useful.
        // Usually, we have header, timestamp and at least one MIDI byte, 
        // but a SysEx continuation could perhaps have only a header and a 
        // single data byte (this is not explicitly allowed by the spec, but
        // handling takes no extra effort)
        if (len < 2)
            return false;
        // First byte should be a header. If it's a data byte, discard packet.
        if (isData(data[0]))
            return false;
        // If the second byte is a data byte, this is a SysEx continuation
        // packet, simply pass on the data to the MIDI parser.
        if (isData(data[1]))
            midiparser(data[1]);
        // Otherwise, it's the LSB of the timestamp (ignored)
        else {}

        // Timestamp bytes are always followed by a MIDI status byte.
        // In this simple parser, time stamps are ignored.
        bool prevWasTimestamp = true;
        for (const uint8_t *d = data + 2; d < data + len; d++) {
            // Simply pass on all normal data bytes to the MIDI parser.
            if (isData(*d)) {
                midiparser(*d);
                prevWasTimestamp = false;
            }
            // If it's not a data byte, it's either a timestamp byte or a 
            // MIDI status byte.
            else {
                // If the previous non-data byte was a timestamp, this one is
                // a MIDI status byte, so send it to the MIDI parser.
                if (prevWasTimestamp)
                    midiparser(*d);
                // Timestamp bytes and MIDI status bytes should alternate.
                prevWasTimestamp = !prevWasTimestamp;
            }
        }
        return true;
    }

  private:
    /// Check if the given byte is a data byte (and not a header, timestamp or
    /// status byte).
    static bool isData(uint8_t data) { return (data & (1 << 7)) == 0; }

  private:
    MIDIParser midiparser;
};

template <class MIDIParser>
auto BLEMIDI_Parser(MIDIParser &&midiparser) -> BLEMIDI_Parser_<MIDIParser> {
    return BLEMIDI_Parser_<MIDIParser>(std::forward<MIDIParser>(midiparser));
}

END_CS_NAMESPACE
