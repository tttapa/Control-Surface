#ifndef MIDI_INPUT_LED_H_
#define MIDI_INPUT_LED_H_

#include "MIDI_Input_Element.h"
#include "ExtendedInputOutput/ExtendedInputOutput.h"

using namespace ExtIO;

class MIDI_Input_Note_Buffer : public MIDI_Input_Element_Note
{
public:
  MIDI_Input_Note_Buffer(uint8_t address, uint8_t channel, size_t nb_addresses = 1, size_t nb_channels = 1)
      : MIDI_Input_Element_Note(address, channel, nb_addresses, nb_channels)
  {
    states = (uint8_t *)malloc((nb_channels * nb_addresses + 7) / 8);
    memset(states, 0, (nb_channels * nb_addresses + 7) / 8);
  }
  ~MIDI_Input_Note_Buffer()
  {
    free(states);
  }
  bool updateImpl(uint8_t messageType, uint8_t targetAddress, uint8_t targetChannel)
  {
    uint8_t addressIndex = targetAddress - this->address;
    uint8_t channelIndex = targetChannel - this->channel;
    uint8_t ledIndex = channelIndex + nb_channels * addressIndex;

    uint8_t statesIndex = ledIndex >> 3;  // ledIndex / 8
    uint8_t statesBit = ledIndex & 0b111; // ledIndex % 8

    uint8_t velocity = MIDI_Interface::getDefault()->read();
    if ((messageType == NOTE_OFF  && velocity != 0) || (messageType == NOTE_ON && velocity == 0))
    {
      states[statesIndex] &= ~(1 << statesBit);
      if (targetAddress == 0x10)
        Serial.println("\t\t\t\t\t\t\t\t\t\tTurned off MUTE channel 1");
    }
    else if (messageType == NOTE_ON)
    {
      states[statesIndex] |= 1 << statesBit;
    }
    display();
    return true;
  }
  bool getState()
  {
    uint8_t ledIndex = channelOffset + nb_channels * addressOffset;
    uint8_t statesIndex = ledIndex >> 3;  // ledIndex / 8
    uint8_t statesBit = ledIndex & 0b111; // ledIndex % 8
    return states[statesIndex] & (1 << statesBit);
  }

protected:
  uint8_t *states = nullptr;
};

class MIDI_LED : public MIDI_Input_Note_Buffer
{
public:
  MIDI_LED(pin_t pin, uint8_t address, uint8_t channel, size_t nb_addresses = 1, size_t nb_channels = 1)
      : MIDI_Input_Note_Buffer(address, channel, nb_addresses, nb_channels), pin(pin)
  {
    pinMode(pin, OUTPUT);
  }
  ~MIDI_LED()
  {
    pinMode(pin, INPUT);
  }
  void display()
  {
    digitalWrite(pin, getState());
  }

protected:
  pin_t pin;
};
#endif // MIDI_INPUT_LED_H_