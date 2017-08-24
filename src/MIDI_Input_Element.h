#ifndef MIDI_INPUT_ELEMENT_H_
#define MIDI_INPUT_ELEMENT_H_

#include "Arduino.h"

class MIDI_Input_Element {
  public:
    MIDI_Input_Element(uint8_t address, uint8_t channel, size_t nb_addresses, size_t nb_channels)
      : address(address), channel(channel), nb_addresses(nb_addresses), nb_channels(nb_channels) {};
    virtual bool update(uint8_t header, uint8_t data1, uint8_t data2) {};
    virtual void refresh() {};

    void setChannelOffset(uint8_t offset) // Set the channel offset
    {
      channelOffset = offset % nb_channels;
      refresh();
    }
    void setAddressOffset(uint8_t offset) // Set the address (note or controller number) offset
    {
      addressOffset = offset % nb_addresses;
      refresh();
    }

  protected:
    uint8_t channelOffset = 0;
    uint8_t addressOffset = 0;
    uint8_t channel, address;
    const size_t nb_channels, nb_addresses;
};

#endif // MIDI_INPUT_ELEMENT_H_