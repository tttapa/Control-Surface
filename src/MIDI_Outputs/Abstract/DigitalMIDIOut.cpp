#include "DigitalMIDIOut.h"

void DigitalMIDIOut::update() {
    if (!isActive()) {
        addressOffset = newAddressOffset;
        channelOffset = newChannelOffset;
    }
    refresh();
}

void DigitalMIDIOut::setChannelOffset(uint8_t offset) {
    this->newChannelOffset = offset;
}

void DigitalMIDIOut::setAddressOffset(uint8_t offset) {
    this->newAddressOffset = offset;
}