#include "SysExBuffer.hpp"
#include <string.h>

BEGIN_CS_NAMESPACE

void SysExBuffer::start() {
    length = 0; // if the previous message wasn't finished, overwrite it
    receiving = true;
}

void SysExBuffer::end() {
    receiving = false;
}

void SysExBuffer::add(uint8_t data) {
    buffer[length] = data;
    ++length;
}

void SysExBuffer::add(const uint8_t *data, uint8_t len) {
    memcpy(buffer + length, data, len);
    length += len;
}

bool SysExBuffer::hasSpaceLeft(uint8_t amount) const {
    bool avail = length <= SYSEX_BUFFER_SIZE - amount;
    if (!avail)
        DEBUG(F("SysEx: Buffer full (") << amount << ')');
    return avail;
}

bool SysExBuffer::isReceiving() const { return receiving; }

const uint8_t *SysExBuffer::getBuffer() const { return buffer; }

uint16_t SysExBuffer::getLength() const { return length; }

END_CS_NAMESPACE