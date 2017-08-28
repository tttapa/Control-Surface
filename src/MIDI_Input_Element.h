#ifndef MIDI_INPUT_ELEMENT_H_
#define MIDI_INPUT_ELEMENT_H_

#include "Arduino.h"
#include "MIDI_Element.h"
#include "Linked_List.h"

class MIDI_Input_Element_CC : public MIDI_Element
{
public:
  MIDI_Input_Element_CC(uint8_t address, uint8_t channel, size_t nb_addresses, size_t nb_channels);
  ~MIDI_Input_Element_CC();

  bool update(uint8_t targetChannel, uint8_t targetAddress);

  virtual void refresh(){}

  static MIDI_Input_Element_CC *getFirst();
  MIDI_Input_Element_CC *getNext();

protected:
  inline bool match(uint8_t targetAddress, uint8_t targetChannel);

  virtual bool updateImpl(uint8_t targetAddress, uint8_t targetChannel) { return false; }

  const uint8_t channel, address;
  const size_t nb_channels, nb_addresses;

  MIDI_Input_Element_CC *next = nullptr, *previous = nullptr;

  static MIDI_Input_Element_CC *last;
  static MIDI_Input_Element_CC *first;
};

//----------------------------------------------------------------------------------------------------------------------------------------//

class MIDI_Input_Element_Note : public MIDI_Element
{
public:
  MIDI_Input_Element_Note(uint8_t address, uint8_t channel, size_t nb_addresses, size_t nb_channels);
  ~MIDI_Input_Element_Note();

  bool update(uint8_t messageType, uint8_t targetAddress, uint8_t targetChannel);

  virtual void refresh(){}

  static MIDI_Input_Element_Note *getFirst();
  MIDI_Input_Element_Note *getNext();

protected:
  inline bool match(uint8_t targetAddress, uint8_t targetChannel);

  virtual bool updateImpl(uint8_t messageType, uint8_t targetAddress, uint8_t targetChannel) { return false; }

  const uint8_t channel, address;
  const size_t nb_channels, nb_addresses;

  MIDI_Input_Element_Note *next = nullptr, *previous = nullptr;

  static MIDI_Input_Element_Note *last;
  static MIDI_Input_Element_Note *first;
};

//----------------------------------------------------------------------------------------------------------------------------------------//

#endif // MIDI_INPUT_ELEMENT_H_