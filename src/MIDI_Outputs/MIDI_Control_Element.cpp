#include "MIDI_Control_Element.h"

void MIDI_Control_Element::setChannelOffset(uint8_t offset) // Set the channel offset
{
    channelOffset = offset;
}
void MIDI_Control_Element::setAddressOffset(uint8_t offset) // Set the address (note or controller number) offset
{
    addressOffset = offset;
}
void MIDI_Control_Element::setChannelsPerBank(uint8_t channels) // Set the number of channels per bank
{
    channelsPerBank = channels;
}

MIDI_Control_Element *MIDI_Control_Element::last = nullptr;
MIDI_Control_Element *MIDI_Control_Element::first = nullptr;