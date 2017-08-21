#ifndef BANK_h_
#define BANK_h_

#include "Arduino.h"
#include "MIDI_Control_Element.h"

class Bank
{
public:
  Bank();  // Constructor
  ~Bank(); // Deconstructor
  enum bankType
  {
    BANK_CHANNEL,
    BANK_ADDRESS
  };
  void add(MIDI_Control_Element *element, bankType type = BANK_CHANNEL);
  void refresh();
  void setBankSetting(uint8_t bankSetting);
  void average(size_t length);

private:
  struct bank_element
  {
    MIDI_Control_Element *element;
    bank_element *next;
    bankType type;
  };
  bank_element *firstElement = nullptr;
  bank_element *lastElement = nullptr;
};

#endif // BANK_h_