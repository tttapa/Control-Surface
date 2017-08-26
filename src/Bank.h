#ifndef BANK_h_
#define BANK_h_

#include "Arduino.h"
#include "MIDI_Control_Element.h"
#include "MIDI_Input_Element.h"

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
  void add(MIDI_Element *element, bankType type = CHANGE_CHANNEL);
  void add(MIDI_Element &element, bankType type = CHANGE_CHANNEL);
  void setBankSetting(uint8_t bankSetting);
  void average(size_t length);
  void map(int (*fn)(int));

private:
  struct MIDI_Element_list_node
  {
    MIDI_Element *element;
    MIDI_Element_list_node *next;
    bankType type;
  };
  MIDI_Element_list_node *firstMIDI_Element = nullptr;
  MIDI_Element_list_node *lastMIDI_Element = nullptr;

  void updateMidiInput();
};

#endif // BANK_h_