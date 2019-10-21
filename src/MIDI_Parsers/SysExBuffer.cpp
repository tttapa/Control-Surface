#include "SysExBuffer.hpp"

BEGIN_CS_NAMESPACE

void SysExBuffer::start() {
    SysExLength = 0; // if the previous message wasn't finished, overwrite it
    receiving = true;
    DEBUG("Start SysEx");
}

void SysExBuffer::end() {
    receiving = false;
    DEBUG("End SysEx");
}

bool SysExBuffer::add(uint8_t data) {
    if (!hasSpaceLeft()) // if the buffer is full
        return false;
    SysExBuffer[SysExLength] = data; // add the data to the buffer
    ++SysExLength;
    return true;
}

bool SysExBuffer::hasSpaceLeft() const {
    bool avail = SysExLength < SYSEX_BUFFER_SIZE;
    if (!avail)
        DEBUG("SysEx buffer full");
    return avail;
}

bool SysExBuffer::isReceiving() const { return receiving; }

const uint8_t *SysExBuffer::getBuffer() const { return SysExBuffer; }

size_t SysExBuffer::getLength() const { return SysExLength; }

END_CS_NAMESPACE