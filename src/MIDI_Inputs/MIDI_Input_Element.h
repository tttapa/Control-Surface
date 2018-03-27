#ifndef MIDI_INPUT_ELEMENT_H_
#define MIDI_INPUT_ELEMENT_H_

#include "../MIDI_Element.h"
#include "../Helpers/Linked_List.h"
#include "./MIDI_message_matcher.h"

//----------------------------------------------------------------------------------------------------------------------------------------//

class MIDI_Input_Element : public MIDI_Element
{
public:
  MIDI_Input_Element(uint8_t address, uint8_t channel, uint8_t nb_addresses = 1, uint8_t nb_channels = 1);

  virtual bool update(const MIDI_message_matcher &midimsg);
  virtual void display() {}

  void setChannelOffset(uint8_t offset); // Set the channel offset
  void setAddressOffset(uint8_t offset); // Set the address (note or controller number) offset
  virtual inline bool match(const MIDI_message_matcher &midimsg);

protected:
  virtual bool updateImpl(const MIDI_message_matcher &midimsg) {return true;}
  bool matchAddress(uint8_t targetAddress);
  bool matchChannel(uint8_t targetChannel);

  const uint8_t address, channel;
  const uint8_t nb_addresses, nb_channels;

  virtual void moveDown() {}
  
  template <class Node>
  friend void LinkedList::append(Node *, Node *&, Node *&);
  template <class Node>
  friend void LinkedList::moveDown(Node *, Node *&, Node *&);
  template <class Node>
  friend void LinkedList::remove(Node *, Node *&, Node *&);
};

//----------------------------------------------------------------------------------------------------------------------------------------//

class MIDI_Input_Element_CC : public MIDI_Input_Element
{
public:
  MIDI_Input_Element_CC(uint8_t address, uint8_t channel, uint8_t nb_addresses, uint8_t nb_channels);
  ~MIDI_Input_Element_CC();

  static MIDI_Input_Element_CC *getFirst();
  static MIDI_Input_Element_CC *getLast();
  MIDI_Input_Element_CC *getNext();

protected:
  MIDI_Input_Element_CC *next = nullptr, *previous = nullptr;
  static MIDI_Input_Element_CC *last;
  static MIDI_Input_Element_CC *first;

  void moveDown();

  template <class Node>
  friend void LinkedList::append(Node *, Node *&, Node *&);
  template <class Node>
  friend void LinkedList::moveDown(Node *, Node *&, Node *&);
  template <class Node>
  friend void LinkedList::remove(Node *, Node *&, Node *&);
};

//----------------------------------------------------------------------------------------------------------------------------------------//

class MIDI_Input_Element_Note : public MIDI_Input_Element
{
public:
  MIDI_Input_Element_Note(uint8_t address, uint8_t channel, uint8_t nb_addresses, uint8_t nb_channels);
  ~MIDI_Input_Element_Note();

  static MIDI_Input_Element_Note *getFirst();
  static MIDI_Input_Element_Note *getLast();
  MIDI_Input_Element_Note *getNext();

protected:
  MIDI_Input_Element_Note *next = nullptr, *previous = nullptr;
  static MIDI_Input_Element_Note *last;
  static MIDI_Input_Element_Note *first;

  void moveDown();

  template <class Node>
  friend void LinkedList::append(Node *, Node *&, Node *&);
  template <class Node>
  friend void LinkedList::moveDown(Node *, Node *&, Node *&);
  template <class Node>
  friend void LinkedList::remove(Node *, Node *&, Node *&);
};

//----------------------------------------------------------------------------------------------------------------------------------------//

class MIDI_Input_Element_ChannelPressure : public MIDI_Input_Element
{
public:
  MIDI_Input_Element_ChannelPressure(uint8_t address, uint8_t channel, uint8_t nb_addresses, uint8_t nb_channels);
  ~MIDI_Input_Element_ChannelPressure();

  static MIDI_Input_Element_ChannelPressure *getFirst();
  static MIDI_Input_Element_ChannelPressure *getLast();
  MIDI_Input_Element_ChannelPressure *getNext();

  inline bool match(const MIDI_message_matcher &midimsg);

protected:
  MIDI_Input_Element_ChannelPressure *next = nullptr, *previous = nullptr;
  static MIDI_Input_Element_ChannelPressure *last;
  static MIDI_Input_Element_ChannelPressure *first;

  void moveDown();
  
  template <class Node>
  friend void LinkedList::append(Node *, Node *&, Node *&);
  template <class Node>
  friend void LinkedList::moveDown(Node *, Node *&, Node *&);
  template <class Node>
  friend void LinkedList::remove(Node *, Node *&, Node *&);
};

//----------------------------------------------------------------------------------------------------------------------------------------//

#endif // MIDI_INPUT_ELEMENT_H_