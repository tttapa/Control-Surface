#include "NoteOut.h"
#include "../Control_Surface/Control_Surface_Class.h"

NoteOut::NoteOut(uint8_t address, uint8_t channel, uint8_t velocity) // Constructor
    : address(address), channel(channel), velocity(velocity)
{
}

void NoteOut::setVelocity(uint8_t velocity)
{
    this->velocity = velocity;
}

void NoteOut::update() // Check if the button state changed, and send a MIDI event accordingly
{
    if (!active)
    {
        addressOffset = newAddressOffset;
        channelOffset = newChannelOffset;
    }
    refresh();
}

void NoteOut::sendOn()
{
    Control_Surface.MIDI()->send(NOTE_ON,
                                 channel + channelOffset * tracksPerBank,
                                 address + addressOffset * tracksPerBank,
                                 velocity);
    this->active = true;
}
void NoteOut::sendOff()
{
    Control_Surface.MIDI()->send(NOTE_ON,
                                 channel + channelOffset * tracksPerBank,
                                 address + addressOffset * tracksPerBank,
                                 velocity);
    this->active = false;
}

void NoteOut::setChannelOffset(uint8_t offset) // Set the channel offset
{
    this->newChannelOffset = offset;
}

void NoteOut::setAddressOffset(uint8_t offset) // Set the address (note or controller number) offset
{
    this->newAddressOffset = offset;
}