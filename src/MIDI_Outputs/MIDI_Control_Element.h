#ifndef MIDI_CONTROL_ELEMENT_h
#define MIDI_CONTROL_ELEMENT_h

#include "../MIDI_Element.h"
#include "../Helpers/Linked_List.h"

class MIDI_Control_Element : public MIDI_Element
{
  friend class Control_Surface_;

public:
  MIDI_Control_Element() // Constructor
  {
    INSERT_INTO_LINKED_LIST(this, first, last);
  }
  ~MIDI_Control_Element()
  {
    DELETE_FROM_LINKED_LIST(this, first, last);
  }
  
  MIDI_Control_Element *getNext()
  {
    return next;
  }
  static MIDI_Control_Element *getFirst()
  {
    return first;
  }

private:
  MIDI_Control_Element *next = nullptr, *previous = nullptr;

  static MIDI_Control_Element *last;
  static MIDI_Control_Element *first;
};

#endif // MIDI_CONTROL_ELEMENT_h