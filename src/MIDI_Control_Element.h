#ifndef MIDI_CONTROL_ELEMENT_h
#define MIDI_CONTROL_ELEMENT_h

class MIDI_Control_Element
{
  friend class Bank;

public:
  MIDI_Control_Element() {}                   // Constructor
  virtual void refresh() {}                   // Check if the state or value of the control has changed since last refresh, if so, send MIDI event
  virtual void average(size_t length) {}      // Use the average of multiple samples of analog readings
  virtual void map(int (*fn)(int)) {}         // Change the function pointer for analogMap to a new function. It will be applied to the raw analog input value in Analog::refresh()
  virtual void setChannel(uint8_t channel) {} // Set the channel
};

#endif // MIDI_CONTROL_ELEMENT_h