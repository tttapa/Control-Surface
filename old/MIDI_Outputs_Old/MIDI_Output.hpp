#pragma once

#include "MIDI_Control_Element.h"

class MIDI_Output_Addressable : public MIDI_Control_Element {
  public:
    MIDI_Output_Addressable(uint8_t address, uint8_t channel)
        : baseAddress(address), baseChannel(channel) {}

    uint8_t getBaseChannel() const { return baseChannel; }
    uint8_t getBaseAddress() const { return baseAddress; }

    virtual uint8_t getChannel() const { return getBaseChannel(); }
    virtual uint8_t getAddress() const { return getBaseAddress(); }

  private:
    const uint8_t baseAddress;
    const uint8_t baseChannel;
};