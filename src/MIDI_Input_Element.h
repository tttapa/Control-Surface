#ifndef MIDI_INPUT_ELEMENT_H_
#define MIDI_INPUT_ELEMENT_H_

#include "Arduino.h"
#include "MIDI_Element.h"
#include "Linked_List.h"

class MIDI_Input_Element_CC : public MIDI_Element
{
public:
  MIDI_Input_Element_CC(uint8_t address, uint8_t channel, uint8_t nb_addresses, uint8_t nb_channels);
  ~MIDI_Input_Element_CC();

  bool update(uint8_t targetChannel, uint8_t targetAddress);

  static MIDI_Input_Element_CC *getFirst();
  MIDI_Input_Element_CC *getNext();

protected:
  inline bool match(uint8_t targetAddress, uint8_t targetChannel);

  virtual bool updateImpl(uint8_t targetAddress, uint8_t targetChannel) { return false; }

  const uint8_t channel, address;
  const uint8_t nb_channels, nb_addresses;

  MIDI_Input_Element_CC *next = nullptr, *previous = nullptr;

  static MIDI_Input_Element_CC *last;
  static MIDI_Input_Element_CC *first;
};

//----------------------------------------------------------------------------------------------------------------------------------------//

class MIDI_Input_Element_Note : public MIDI_Element
{
public:
  MIDI_Input_Element_Note(uint8_t address, uint8_t channel, uint8_t nb_addresses, uint8_t nb_channels);
  ~MIDI_Input_Element_Note();

  bool update(uint8_t messageType, uint8_t targetAddress, uint8_t targetChannel);

  static MIDI_Input_Element_Note *getFirst();
  MIDI_Input_Element_Note *getNext();

protected:
  inline bool match(uint8_t targetAddress, uint8_t targetChannel);

  virtual bool updateImpl(uint8_t messageType, uint8_t targetAddress, uint8_t targetChannel) { return false; }

  const uint8_t channel, address;
  const uint8_t nb_channels, nb_addresses;

  MIDI_Input_Element_Note *next = nullptr, *previous = nullptr;

  static MIDI_Input_Element_Note *last;
  static MIDI_Input_Element_Note *first;
};

//----------------------------------------------------------------------------------------------------------------------------------------//

class MIDI_Input_Element_ChannelPressure : public MIDI_Element
{
public:
  MIDI_Input_Element_ChannelPressure(uint8_t channel, uint8_t nb_channels);
  ~MIDI_Input_Element_ChannelPressure();

  bool update(uint8_t targetChannel, uint8_t data1);

  static MIDI_Input_Element_ChannelPressure *getFirst();
  MIDI_Input_Element_ChannelPressure *getNext();

protected:
  inline bool match(uint8_t targetChannel);

  virtual bool updateImpl(uint8_t targetChannel, uint8_t data1) { return false; }

  const uint8_t channel;
  const uint8_t nb_channels;

  MIDI_Input_Element_ChannelPressure *next = nullptr, *previous = nullptr;

  static MIDI_Input_Element_ChannelPressure *last;
  static MIDI_Input_Element_ChannelPressure *first;
};

//----------------------------------------------------------------------------------------------------------------------------------------//

#endif // MIDI_INPUT_ELEMENT_H_