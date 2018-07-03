#include "MIDI_Parser.h"
#include "../Settings/SettingsWrapper.h"

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
    DEBUG("SysEx data byte: " << hex << data << dec << '\t'
                              << "SysExLength = " << SysExLength);
    return true;
}

bool MIDI_Parser::hasSpaceLeft() {
    bool avail = SysExLength < bufferSize;
    if (!avail)
        DEBUG("SysEx buffer full");
    return avail;
}

MIDI_message MIDI_Parser::getChannelMessage() { return midimsg; }
const uint8_t *MIDI_Parser::getSysExBuffer() { return SysExBuffer; }
size_t MIDI_Parser::getSysExLength() { return SysExLength; }

bool MIDI_Parser::isHeader(uint8_t data) { return data & (1 << 7); }