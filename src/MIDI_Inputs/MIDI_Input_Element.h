#ifndef MIDI_INPUT_ELEMENT_H_
#define MIDI_INPUT_ELEMENT_H_

#include "Arduino.h"
#include "../MIDI_Element.h"
#include "../Control_Surface/Control_Surface_Class.h"
#include "../Helpers/Linked_List.h"

//----------------------------------------------------------------------------------------------------------------------------------------//

class MIDI_Input_Element : public MIDI_Element
{
public:
  MIDI_Input_Element(uint8_t address, uint8_t channel, uint8_t nb_addresses = 1, uint8_t nb_channels = 1);

  virtual bool update(uint8_t targetChannel, uint8_t targetAddress);
  virtual void display() {}

  void setChannelOffset(uint8_t offset); // Set the channel offset
  void setAddressOffset(uint8_t offset); // Set the address (note or controller number) offset

protected:
  virtual bool updateImpl(MIDI_message *midimsg)=0;
  virtual inline bool match(uint8_t targetAddress, uint8_t targetChannel);
  bool matchAddress(uint8_t targetAddress);
  bool matchChannel(uint8_t targetChannel);

  const uint8_t channel, address;
  const uint8_t nb_channels, nb_addresses;
};

//----------------------------------------------------------------------------------------------------------------------------------------//

class MIDI_Input_Element_CC : public MIDI_Input_Element
{
public:
  MIDI_Input_Element_CC(uint8_t address, uint8_t channel, uint8_t nb_addresses, uint8_t nb_channels);
  ~MIDI_Input_Element_CC();

  static MIDI_Input_Element_CC *getFirst();
  MIDI_Input_Element_CC *getNext();

protected:
  MIDI_Input_Element_CC *next = nullptr, *previous = nullptr;

  static MIDI_Input_Element_CC *last;
  static MIDI_Input_Element_CC *first;
};

//----------------------------------------------------------------------------------------------------------------------------------------//

class MIDI_Input_Element_Note : public MIDI_Input_Element
{
public:
  MIDI_Input_Element_Note(uint8_t address, uint8_t channel, uint8_t nb_addresses, uint8_t nb_channels);
  ~MIDI_Input_Element_Note();

  static MIDI_Input_Element_Note *getFirst();
  MIDI_Input_Element_Note *getNext();

protected:
  MIDI_Input_Element_Note *next = nullptr, *previous = nullptr;

  static MIDI_Input_Element_Note *last;
  static MIDI_Input_Element_Note *first;
};

//----------------------------------------------------------------------------------------------------------------------------------------//

class MIDI_Input_Element_ChannelPressure : public MIDI_Input_Element
{
public:
  MIDI_Input_Element_ChannelPressure(uint8_t address, uint8_t channel, uint8_t nb_addresses, uint8_t nb_channels);
  ~MIDI_Input_Element_ChannelPressure();

  static MIDI_Input_Element_ChannelPressure *getFirst();
  MIDI_Input_Element_ChannelPressure *getNext();

protected:
  inline bool match(uint8_t targetAddress, uint8_t targetChannel);

  MIDI_Input_Element_ChannelPressure *next = nullptr, *previous = nullptr;

  static MIDI_Input_Element_ChannelPressure *last;
  static MIDI_Input_Element_ChannelPressure *first;
};

//----------------------------------------------------------------------------------------------------------------------------------------//

#endif // MIDI_INPUT_ELEMENT_H_