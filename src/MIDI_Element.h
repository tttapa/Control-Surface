#ifndef MIDI_ELEMENT_h
#define MIDI_ELEMENT_h

#include <stdint.h> // uint8_t
#include <stddef.h> // size_t

#include "./Settings/SettingsWrapper.h"

class MIDI_Element
{
  friend class Control_Surface_;

  public:
    MIDI_Element() // Constructor
    {
        ;
    }
    ~MIDI_Element() // Destructor
    {
        ;
    }
  protected:
    virtual void reset() {}                // Reset to default value (currently only used for VU meters)
    virtual void update() = 0;             // Check if the state or value of the control has changed since last update, if so, send MIDI event, update MIDI_Input_Element (write buffer to output etc.)
    
  public:
    virtual void map(int (*fn)(int)) {}    // Change the function pointer for analogMap to a new function. It will be applied to the raw analog input value in Analog::update()

    virtual void setChannelOffset(uint8_t offset) // Set the channel offset
    {
        channelOffset = offset;
    }
    virtual void setAddressOffset(uint8_t offset) // Set the address (note or controller number) offset
    {
        addressOffset = offset;
    }
    virtual void settracksPerBank(uint8_t channels) // Set the number of channels per bank
    {
        tracksPerBank = channels;
    }

  protected:
    uint8_t channelOffset = 0;
    uint8_t addressOffset = 0;
    uint8_t tracksPerBank = 1;
};

#endif // MIDI_ELEMENT_h