#ifndef BANK_h_
#define BANK_h_

#include "Arduino.h"
#include "MIDI_Control_Element.h"

class Bank
{
public:
  Bank();  // Constructor
  ~Bank(); // Destructor
  enum bankType
  {
    CHANGE_CHANNEL,
    CHANGE_ADDRESS
  };
  void add(MIDI_Control_Element *element, bankType type = CHANGE_CHANNEL);
  void add(MIDI_Control_Element &element, bankType type = CHANGE_CHANNEL);
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