#ifndef MIDI_INPUT_ELEMENT_H_
#define MIDI_INPUT_ELEMENT_H_

#include "Arduino.h"
#include "MIDI_Element.h"

class MIDI_Input_Element : public MIDI_Element
{
public:
  MIDI_Input_Element(uint8_t address, uint8_t channel, size_t nb_addresses, size_t nb_channels)
      : address(address), channel(channel), nb_addresses(nb_addresses), nb_channels(nb_channels)
  {
    if (first == nullptr)
      first = this;
    previous = last;
    if (previous != nullptr)
      previous->next = this;
    last = this;
    next = nullptr;
  }
  ~MIDI_Input_Element()
  {
    if (previous != nullptr)
      previous->next = next;
    if (this == last)
      last = previous;
    if (next != nullptr)
      next->previous = previous;
    if (this == first)
      first = next;
  }
  virtual bool update(uint8_t header, uint8_t data1, uint8_t data2){};
  virtual void refresh(){};

  MIDI_Input_Element *getNext()
  {
    return next;
  }
  static MIDI_Input_Element *getFirst()
  {
    return first;
  }

protected:
  const uint8_t channel, address;
  const size_t nb_channels, nb_addresses;

  MIDI_Input_Element *next = nullptr, *previous = nullptr;

  static MIDI_Input_Element *last;
  static MIDI_Input_Element *first;
};

#endif // MIDI_INPUT_ELEMENT_H_