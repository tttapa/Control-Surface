#include "MIDI_Input_Element.h"
#include "../Helpers/StreamOut.h"

//----------------------------------------------------------------------------------------------------------------------------------------//

MIDI_Input_Element::MIDI_Input_Element(uint8_t address, uint8_t channel, uint8_t nb_addresses, uint8_t nb_channels)
    : address(address), channel(channel - 1), nb_addresses(nb_addresses), nb_channels(nb_channels) {}

bool MIDI_Input_Element::update(const MIDI_message_matcher &midimsg)
{
    if (!match(midimsg))
        return false;
    if (!updateImpl(midimsg))
        return false;
#ifdef DEBUG
    DEBUG_OUT << "Match: " << endl
          << tab << "address = " << hex << this->address << endl
          << tab << "channel = " << this->channel << dec << endl;
#endif
    moveDown(); // Most frequently updated inputs sink to the bottom for performance improvements
    return true;
}

inline bool MIDI_Input_Element::match(const MIDI_message_matcher &midimsg)
{
    return matchAddress(midimsg.data1) && matchChannel(midimsg.channel);
}

bool MIDI_Input_Element::matchAddress(uint8_t targetAddress)
{
    int8_t addressDiff = targetAddress - this->address;
    return (addressDiff >= 0) && (addressDiff < nb_addresses * channelsPerBank) && (addressDiff % channelsPerBank == 0);
}
bool MIDI_Input_Element::matchChannel(uint8_t targetChannel)
{
    int8_t channelDiff = targetChannel - this->channel;
    return (channelDiff >= 0) && (channelDiff < nb_channels * channelsPerBank) && (channelDiff % channelsPerBank == 0);
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
    LinkedList::append(this, first, last);
}
MIDI_Input_Element_CC::~MIDI_Input_Element_CC()
{
    LinkedList::remove(this, first, last);
}

MIDI_Input_Element_CC *MIDI_Input_Element_CC::getFirst()
{
    return first;
}
MIDI_Input_Element_CC *MIDI_Input_Element_CC::getLast()
{
    return last;
}
MIDI_Input_Element_CC *MIDI_Input_Element_CC::getNext()
{
    return next;
}
void MIDI_Input_Element_CC::moveDown() {
    LinkedList::moveDown(this, first, last);
}

MIDI_Input_Element_CC *MIDI_Input_Element_CC::last = nullptr;
MIDI_Input_Element_CC *MIDI_Input_Element_CC::first = nullptr;

//----------------------------------------------------------------------------------------------------------------------------------------//

MIDI_Input_Element_Note::MIDI_Input_Element_Note(uint8_t address, uint8_t channel, uint8_t nb_addresses, uint8_t nb_channels)
    : MIDI_Input_Element(address, channel, nb_addresses, nb_channels)
{
    LinkedList::append(this, first, last);
}
MIDI_Input_Element_Note::~MIDI_Input_Element_Note()
{
    LinkedList::remove(this, first, last);
}

MIDI_Input_Element_Note *MIDI_Input_Element_Note::getFirst()
{
    return first;
}
MIDI_Input_Element_Note *MIDI_Input_Element_Note::getLast()
{
    return last;
}
MIDI_Input_Element_Note *MIDI_Input_Element_Note::getNext()
{
    return next;
}
void MIDI_Input_Element_Note::moveDown() {
    LinkedList::moveDown(this, first, last);
}

MIDI_Input_Element_Note *MIDI_Input_Element_Note::last = nullptr;
MIDI_Input_Element_Note *MIDI_Input_Element_Note::first = nullptr;

//----------------------------------------------------------------------------------------------------------------------------------------//

MIDI_Input_Element_ChannelPressure::MIDI_Input_Element_ChannelPressure(uint8_t address, uint8_t channel, uint8_t nb_addresses, uint8_t nb_channels)
    : MIDI_Input_Element(address, channel, nb_addresses, nb_channels)
{
    LinkedList::append(this, first, last);
}
MIDI_Input_Element_ChannelPressure::~MIDI_Input_Element_ChannelPressure()
{
    LinkedList::remove(this, first, last);
}

inline bool MIDI_Input_Element_ChannelPressure::match(const MIDI_message_matcher &midimsg)
{
    return matchChannel(midimsg.channel);
}

MIDI_Input_Element_ChannelPressure *MIDI_Input_Element_ChannelPressure::getFirst()
{
    return first;
}
MIDI_Input_Element_ChannelPressure *MIDI_Input_Element_ChannelPressure::getLast()
{
    return last;
}
MIDI_Input_Element_ChannelPressure *MIDI_Input_Element_ChannelPressure::getNext()
{
    return next;
}
void MIDI_Input_Element_ChannelPressure::moveDown() {
    LinkedList::moveDown(this, first, last);
}

MIDI_Input_Element_ChannelPressure *MIDI_Input_Element_ChannelPressure::last = nullptr;
MIDI_Input_Element_ChannelPressure *MIDI_Input_Element_ChannelPressure::first = nullptr;

//----------------------------------------------------------------------------------------------------------------------------------------//