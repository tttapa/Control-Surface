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
  void add(MIDI_Control_Element *element, bankType type = CHANGE_CHANNEL);
  void add(MIDI_Control_Element &element, bankType type = CHANGE_CHANNEL);
  void add(MIDI_Input_Element *element, bankType type = CHANGE_CHANNEL);
  void add(MIDI_Input_Element &element, bankType type = CHANGE_CHANNEL);
  void refresh();
  void setBankSetting(uint8_t bankSetting);
  void average(size_t length);

private:
  struct control_element
  {
    MIDI_Control_Element *element;
    control_element *next = nullptr;
    bankType type;
  };
  struct input_element
  {
    MIDI_Input_Element *element;
    input_element *next = nullptr;
    bankType type;
  };
  control_element *firstControlElement = nullptr;
  control_element *lastControlElement = nullptr;
  input_element *firstInputElement = nullptr;
  input_element *lastInputElement = nullptr;

  void updateMidiInput();
};

#endif // BANK_h_