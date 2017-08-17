#ifndef BANK_h_
#define BANK_h_

#include "Arduino.h"
#include "MIDI_Control_Element.h"

struct element_node
{
  MIDI_Control_Element *element;
  element_node *next;
};

class Bank
{
public:
  Bank();  // Constructor
  ~Bank(); // Deconstructor
  void add(MIDI_Control_Element *element);
  void refresh();
  void setChannel(uint8_t channel);

private:
  element_node *firstElement = nullptr;
  element_node *lastElement = nullptr;
};

#endif // BANK_h_