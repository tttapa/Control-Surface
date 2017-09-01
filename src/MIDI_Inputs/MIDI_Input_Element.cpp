#ifndef NO_MIDI_INPUT

#include "MIDI_Input_Element.h"

//----------------------------------------------------------------------------------------------------------------------------------------//

MIDI_Input_Element::MIDI_Input_Element(uint8_t address, uint8_t channel, uint8_t nb_addresses, uint8_t nb_channels)
: address(address), channel(channel - 1), nb_addresses(nb_addresses), nb_channels(nb_channels) { }

bool MIDI_Input_Element::update(uint8_t header, uint8_t data1)
{
    uint8_t targetChannel = header & 0x0F;
    uint8_t targetAddress = data1;
    if (!match(targetAddress, targetChannel))
        return false;
    if (!updateImpl(header, data1))
        return false;
    return true;
}

inline bool MIDI_Input_Element::match(uint8_t targetAddress, uint8_t targetChannel)
{
    return matchAddress(targetAddress) && matchChannel(targetChannel);
}

bool MIDI_Input_Element::matchAddress(uint8_t targetAddress)
{
    for (uint8_t address = this->address; address < this->address + nb_addresses * channelsPerBank; address += channelsPerBank)
    {
        if (address == targetAddress)
            return true;
    }
    return false;
}
bool MIDI_Input_Element::matchChannel(uint8_t targetChannel)
{
    for (uint8_t channel = this->channel; channel < this->channel + nb_channels * channelsPerBank; channel += channelsPerBank)
    {
        if (channel == targetChannel)
            return true;
    }
    return false;
}

void MIDI_Input_Element::setChannelOffset(uint8_t offset) // Set the channel offset
{
    channelOffset = offset % nb_channels;
    display();
}
void MIDI_Input_Element::setAddressOffset(uint8_t offset) // Set the address (note or controller number) offset
{
    addressOffset = offset % nb_addresses;
    display();
}

//----------------------------------------------------------------------------------------------------------------------------------------//

MIDI_Input_Element_CC::MIDI_Input_Element_CC(uint8_t address, uint8_t channel, uint8_t nb_addresses, uint8_t nb_channels)
    : MIDI_Input_Element(address, channel, nb_addresses, nb_channels)
{
    INSERT_INTO_LINKED_LIST(this, first, last);
}
MIDI_Input_Element_CC::~MIDI_Input_Element_CC()
{
    DELETE_FROM_LINKED_LIST(this, first, last);
}

MIDI_Input_Element_CC *MIDI_Input_Element_CC::getFirst()
{
    return first;
}
MIDI_Input_Element_CC *MIDI_Input_Element_CC::getNext()
{
    return next;
}

MIDI_Input_Element_CC *MIDI_Input_Element_CC::last = nullptr;
MIDI_Input_Element_CC *MIDI_Input_Element_CC::first = nullptr;

//----------------------------------------------------------------------------------------------------------------------------------------//

MIDI_Input_Element_Note::MIDI_Input_Element_Note(uint8_t address, uint8_t channel, uint8_t nb_addresses, uint8_t nb_channels)
    : MIDI_Input_Element(address, channel, nb_addresses, nb_channels)
{
    INSERT_INTO_LINKED_LIST(this, first, last);
}
MIDI_Input_Element_Note::~MIDI_Input_Element_Note()
{
    DELETE_FROM_LINKED_LIST(this, first, last);
}

MIDI_Input_Element_Note *MIDI_Input_Element_Note::getFirst()
{
    return first;
}
MIDI_Input_Element_Note *MIDI_Input_Element_Note::getNext()
{
    return next;
}

MIDI_Input_Element_Note *MIDI_Input_Element_Note::last = nullptr;
MIDI_Input_Element_Note *MIDI_Input_Element_Note::first = nullptr;

//----------------------------------------------------------------------------------------------------------------------------------------//

MIDI_Input_Element_ChannelPressure::MIDI_Input_Element_ChannelPressure(uint8_t channel, uint8_t nb_channels)
    : MIDI_Input_Element(0, channel, 1, nb_channels)
{
    INSERT_INTO_LINKED_LIST(this, first, last);
}
MIDI_Input_Element_ChannelPressure::~MIDI_Input_Element_ChannelPressure()
{
    DELETE_FROM_LINKED_LIST(this, first, last);
}

inline bool MIDI_Input_Element_ChannelPressure::match(uint8_t targetAddress, uint8_t targetChannel)
{
    return matchChannel(targetChannel);
}

MIDI_Input_Element_ChannelPressure *MIDI_Input_Element_ChannelPressure::getFirst()
{
    return first;
}
MIDI_Input_Element_ChannelPressure *MIDI_Input_Element_ChannelPressure::getNext()
{
    return next;
}

MIDI_Input_Element_ChannelPressure *MIDI_Input_Element_ChannelPressure::last = nullptr;
MIDI_Input_Element_ChannelPressure *MIDI_Input_Element_ChannelPressure::first = nullptr;

//----------------------------------------------------------------------------------------------------------------------------------------//

#endif // #ifndef NO_MIDI_INPUT