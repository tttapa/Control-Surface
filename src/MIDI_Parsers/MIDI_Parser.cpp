#include "MIDI_Parser.h"

#ifndef IGNORE_SYSEX

void MIDI_Parser::startSysEx() {
    SysExLength = 0; // if the previous message wasn't finished, overwrite it
    receivingSysEx = true;
    DEBUG("Start SysEx");
}

void MIDI_Parser::endSysEx() {
    receivingSysEx = false;
    DEBUG("End SysEx");
}

bool MIDI_Parser::addSysExByte(uint8_t data) {
    if (!hasSpaceLeft()) // if the buffer is full
        return false;
    SysExBuffer[SysExLength] = data; // add the data to the buffer
    SysExLength++;
    DEBUG("SysEx data byte: " << hex << +data << dec << '\t'
                              << "SysExLength = " << SysExLength);
    return true;
}

bool MIDI_Parser::hasSpaceLeft() {
    bool avail = SysExLength < bufferSize;
    if (!avail)
        DEBUG("SysEx buffer full");
    return avail;
}

const uint8_t *MIDI_Parser::getSysExBuffer() { return SysExBuffer; }
size_t MIDI_Parser::getSysExLength() { return SysExLength; }
MIDI_Parser::SysExMessage MIDI_Parser::getSysExMessage() {
    return {getSysExBuffer(), getSysExLength()};
}

#endif

bool MIDI_Parser::isStatus(uint8_t data) {
    return data & (1 << 7); // TODO
}

bool MIDI_Parser::isData(uint8_t data) { return (data & (1 << 7)) == 0; }

MIDI_message MIDI_Parser::getChannelMessage() { return midimsg; }