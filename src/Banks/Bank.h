#ifndef BANK_h_
#define BANK_h_

#include "Arduino.h"
#include "../MIDI_Outputs/MIDI_Control_Element.h"

class Bank
{
public:
  Bank(uint8_t channelsPerBank = 1); // Constructor
  ~Bank();                           // Destructor
  enum bankType
  {
    CHANGE_CHANNEL,
    CHANGE_ADDRESS
  };
  void add(MIDI_Control_Element *element, bankType type = CHANGE_ADDRESS);
  void add(MIDI_Control_Element &element, bankType type = CHANGE_ADDRESS);
  template <class T, size_t N>
  void add(T (&arr)[N], bankType type = CHANGE_ADDRESS)
  {
    for (size_t i = 0; i < N; i++)
    {
      add(arr[i], type);
    }
  }

  void setBankSetting(uint8_t bankSetting);
  void map(int (*fn)(int));

private:
  const uint8_t channelsPerBank;
  struct MIDI_Element_list_node
  {
    MIDI_Control_Element *element;
    MIDI_Element_list_node *next;
    bankType type;
  };
  MIDI_Element_list_node *firstMIDI_Element = nullptr;
  MIDI_Element_list_node *lastMIDI_Element = nullptr;

  void updateMidiInput();
};

#endif // BANK_h_