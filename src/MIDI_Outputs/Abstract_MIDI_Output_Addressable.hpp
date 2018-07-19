#pragma once

#include "Abstract_MIDI_Output.hpp"
#include <Control_Surface/Control_Surface_Class.h>

class Abstract_MIDI_Output_Addressable : public Abstract_MIDI_Output {
  public:
    virtual uint8_t getAddress(uint8_t baseAddress) const {
        return baseAddress;
    }
};