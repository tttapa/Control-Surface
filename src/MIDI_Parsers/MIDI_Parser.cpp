#include "MIDI_Parser.hpp"

BEGIN_CS_NAMESPACE

#if !IGNORE_SYSEX

// TODO

#endif

bool MIDI_Parser::isStatus(uint8_t data) {
    return data & (1 << 7); // TODO
}

bool MIDI_Parser::isData(uint8_t data) { return (data & (1 << 7)) == 0; }

ChannelMessage MIDI_Parser::getChannelMessage() { return midimsg; }

END_CS_NAMESPACE