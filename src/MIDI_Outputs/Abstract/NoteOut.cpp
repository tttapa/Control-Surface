#include "NoteOut.h"
#include "../../Control_Surface/Control_Surface_Class.h"

NoteOut::NoteOut(uint8_t address, uint8_t channel, uint8_t velocity)
    : address(address), channel(channel), velocity(velocity) {}

void NoteOut::setVelocity(uint8_t velocity) { this->velocity = velocity; }

void NoteOut::sendOn() {
    Control_Surface.MIDI()->send(
        NOTE_ON, channel + channelOffset * tracksPerBank,
        address + addressOffset * tracksPerBank, velocity);
    this->active = true;
}

void NoteOut::sendOff() {
    Control_Surface.MIDI()->send(
        NOTE_OFF, channel + channelOffset * tracksPerBank,
        address + addressOffset * tracksPerBank, velocity);
    this->active = false;
}

bool NoteOut::isActive() const { return this->active; }