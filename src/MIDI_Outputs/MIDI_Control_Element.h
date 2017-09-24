#ifndef MIDI_CONTROL_ELEMENT_h
#define MIDI_CONTROL_ELEMENT_h

#include "Arduino.h"
#include "../Helpers/Linked_List.h"
#include "../Settings/Settings.h"

class MIDI_Control_Element
{
  friend class MIDI_Controller_;

public:
  MIDI_Control_Element() // Constructor
  {
    INSERT_INTO_LINKED_LIST(this, first, last);
  }
  ~MIDI_Control_Element() // Destructor
  {
    DELETE_FROM_LINKED_LIST(this, first, last);
  }

  virtual void map(int (*fn)(int)) {} // Change the function pointer for analogMap to a new function. It will be applied to the raw analog input value in Analog::refresh() and AnalogHiRes::refresh()
  virtual void invert() {}            // Invert the button state (send Note On event when released, Note Off when pressed). It will be applied in Digital::refresh()

  void setChannelOffset(uint8_t offset);     // Set the channel offset
  void setAddressOffset(uint8_t offset);     // Set the address (note or controller number) offset
  void setChannelsPerBank(uint8_t channels); // Set the number of channels per bank

  MIDI_Control_Element *getNext()
  {
    return next;
  }
  static MIDI_Control_Element *getFirst()
  {
    return first;
  }

protected:
  virtual void refresh() {} // Check if the state or value of the control has changed since last refresh, if so, send MIDI event

  uint8_t channelOffset = 0;
  uint8_t addressOffset = 0;
  uint8_t channelsPerBank = 1;

private:
  MIDI_Control_Element *next = nullptr, *previous = nullptr;

  static MIDI_Control_Element *last;
  static MIDI_Control_Element *first;
};

#endif // MIDI_CONTROL_ELEMENT_h