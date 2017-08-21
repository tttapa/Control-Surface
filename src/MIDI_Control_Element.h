#ifndef MIDI_CONTROL_ELEMENT_h
#define MIDI_CONTROL_ELEMENT_h

class MIDI_Control_Element
{
public:
  MIDI_Control_Element() {}              // Constructor
  virtual void refresh() {}              // Check if the state or value of the control has changed since last refresh, if so, send MIDI event
  virtual void average(size_t length) {} // Use the average of multiple samples of analog readings
  virtual void map(int (*fn)(int)) {}    // Change the function pointer for analogMap to a new function. It will be applied to the raw analog input value in Analog::refresh()

  void setChannelOffset(uint8_t offset) // Set the channel offset
  {
    channelOffset = offset;
  }
  void setAddressOffset(uint8_t offset) // Set the address (note or controller number) offset
  {
    addressOffset = offset;
  }

protected:
  uint8_t channelOffset = 0;
  uint8_t addressOffset = 0;
};

#endif // MIDI_CONTROL_ELEMENT_h