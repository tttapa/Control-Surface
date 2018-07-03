#include "CCOut.h"
#include "../Control_Surface/Control_Surface_Class.h"

DigitalCCOut::DigitalCCOut(uint8_t address, uint8_t channel, uint8_t offValue,
                           uint8_t onValue)
    : address(address), channel(channel), offValue(offValue), onValue(onValue) {
}

void DigitalCCOut::update() {
    if (!active) {
        addressOffset = newAddressOffset;
        channelOffset = newChannelOffset;
    }
    refresh();
}

void DigitalCCOut::sendOn() {
    Control_Surface.MIDI()->send(
        CONTROL_CHANGE, channel + channelOffset * tracksPerBank,
        address + addressOffset * tracksPerBank, onValue);
    this->active = true;
}
void DigitalCCOut::sendOff() {
    Control_Surface.MIDI()->send(
        CONTROL_CHANGE, channel + channelOffset * tracksPerBank,
        address + addressOffset * tracksPerBank, offValue);
    this->active = false;
}

// Set the channel offset
void DigitalCCOut::setChannelOffset(uint8_t offset) {
    this->newChannelOffset = offset;
}

// Set the address (note or controller number) offset
void DigitalCCOut::setAddressOffset(uint8_t offset) {
    this->newAddressOffset = offset;
}

// ------------------------------------------------------------------------- //

CCOut::CCOut(uint8_t address, uint8_t channel)
    : address(address), channel(channel) {}

void CCOut::update() { refresh(); }

void CCOut::send(uint8_t value) {
    Control_Surface.MIDI()->send(
        CONTROL_CHANGE, channel + channelOffset * tracksPerBank,
        address + addressOffset * tracksPerBank, value);
}