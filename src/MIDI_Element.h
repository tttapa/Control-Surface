#ifndef MIDI_ELEMENT_h
#define MIDI_ELEMENT_h

#include <Arduino.h>

class MIDI_Element
{
  public:
    MIDI_Element() // Constructor
    {
        ;
    }
    ~MIDI_Element() // Destructor
    {
        ;
    }
    virtual void reset() {}                // Reset to default value (currently only used for VU meters)
    virtual void refresh() {}              // Check if the state or value of the control has changed since last refresh, if so, send MIDI event, refresh MIDI_Input_Element (write buffer to output etc.)
    virtual void map(int (*fn)(int)) {}    // Change the function pointer for analogMap to a new function. It will be applied to the raw analog input value in Analog::refresh()

    virtual void setChannelOffset(uint8_t offset) // Set the channel offset
    {
        channelOffset = offset;
    }
    virtual void setAddressOffset(uint8_t offset) // Set the address (note or controller number) offset
    {
        addressOffset = offset;
    }
    void setChannelsPerBank(uint8_t channels) // Set the number of channels per bank
    {
        channelsPerBank = channels;
    }

  protected:
    uint8_t channelOffset = 0;
    uint8_t addressOffset = 0;
    uint8_t channelsPerBank = 1;
};

#endif // MIDI_ELEMENT_h