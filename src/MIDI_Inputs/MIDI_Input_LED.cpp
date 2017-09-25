#include "MIDI_Input_LED.h"
#include "../MIDI_controller.h"

#ifndef NO_MIDI_INPUT

using namespace ExtIO;

MIDI_Input_Note_Buffer::MIDI_Input_Note_Buffer(uint8_t address, uint8_t channel, size_t nb_addresses, size_t nb_channels)
    : MIDI_Input_Element_Note(address, channel, nb_addresses, nb_channels)
{
    states = (uint8_t *)malloc((nb_channels * nb_addresses + 7) / 8);
    memset(states, 0, (nb_channels * nb_addresses + 7) / 8);
}
MIDI_Input_Note_Buffer::~MIDI_Input_Note_Buffer()
{
    free(states);
}
bool MIDI_Input_Note_Buffer::updateImpl(uint8_t header, uint8_t data1)
{
    uint8_t messageType = header & 0xF0;
    uint8_t targetChannel = header & 0x0F;
    uint8_t addressIndex = (data1 - this->address) / channelsPerBank;
    uint8_t channelIndex = (targetChannel - this->channel) / channelsPerBank;
    uint8_t ledIndex = channelIndex + nb_channels * addressIndex;

    uint8_t statesIndex = ledIndex >> 3;  // ledIndex / 8
    uint8_t statesBit = ledIndex & 0b111; // ledIndex % 8
    
    uint8_t velocity = MIDI_Controller.MIDI()->read();
    if ((messageType == NOTE_OFF && velocity != 0) || (messageType == NOTE_ON && velocity == 0))
    {
        states[statesIndex] &= ~(1 << statesBit);
    }
    else if (messageType == NOTE_ON)
    {
        states[statesIndex] |= 1 << statesBit;
    }
    display();
    return true;
}
bool MIDI_Input_Note_Buffer::getState()
{
    uint8_t ledIndex = channelOffset + nb_channels * addressOffset;
    uint8_t statesIndex = ledIndex >> 3;  // ledIndex / 8
    uint8_t statesBit = ledIndex & 0b111; // ledIndex % 8
    return states[statesIndex] & (1 << statesBit);
}

MIDI_LED::MIDI_LED(pin_t pin, uint8_t address, uint8_t channel, size_t nb_addresses, size_t nb_channels)
    : MIDI_Input_Note_Buffer(address, channel, nb_addresses, nb_channels), pin(pin)
{
    pinMode(pin, OUTPUT);
}
MIDI_LED::~MIDI_LED()
{
    pinMode(pin, INPUT);
}
void MIDI_LED::display()
{
    digitalWrite(pin, getState());
}

#endif // #ifndef NO_MIDI_INPUT