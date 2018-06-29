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
    LinkedList::append(this, first, last);
  }
  ~MIDI_Control_Element()
  {
    LinkedList::remove(this, first, last);
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
  template <class Node>
  friend void LinkedList::append(Node *, Node *&, Node *&);
  template <class Node>
  friend void LinkedList::moveDown(Node *, Node *&, Node *&);
  template <class Node>
  friend void LinkedList::remove(Node *, Node *&, Node *&);
};

#endif // MIDI_CONTROL_ELEMENT_h