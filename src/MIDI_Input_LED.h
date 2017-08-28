#ifndef MIDI_INPUT_LED_H_
#define MIDI_INPUT_LED_H_

#include "MIDI_Input_Element.h"
#include "ExtendedInputOutput/ExtendedInputOutput.h"

using namespace ExtIO;

class MIDI_Input_LED : public MIDI_Input_Element_Note {
  public:
    MIDI_Input_LED(pin_t pin, uint8_t address, uint8_t channel, size_t nb_addresses = 1, size_t nb_channels = 1)
      : MIDI_Input_Element_Note(address, channel, nb_addresses, nb_channels), pin(pin) {
      states = (uint8_t*)malloc((nb_channels * nb_addresses + 7) / 8);
      memset(states, 0, (nb_channels * nb_addresses + 7) / 8);
      pinMode(pin, OUTPUT);
    }
    ~MIDI_Input_LED() {
      free (states);
      pinMode(pin, INPUT);
    }
    bool updateImpl(uint8_t messageType, uint8_t targetAddress, uint8_t targetChannel) {
      uint8_t addressIndex = targetAddress - this->address;
      uint8_t channelIndex = targetChannel - this->channel;
      uint8_t ledIndex = channelIndex + nb_channels * addressIndex;

      uint8_t statesIndex = ledIndex >> 3; // ledIndex / 8
      uint8_t statesBit = ledIndex & 0b111; // ledIndex % 8

      uint8_t velocity = MIDI_Interface::getDefault()->read();
      if (messageType == NOTE_OFF || (messageType == NOTE_ON && velocity == 0)) {
        states[statesIndex] &= ~(1 << statesBit);
      } else if (messageType == NOTE_ON) {
        states[statesIndex] |= 1 << statesBit;
      }
      refresh();
      return true;
    }
    void refresh() {
      digitalWrite(pin, states[(channelOffset + nb_channels * addressOffset) / 8] & (1 << ((channelOffset + nb_channels * addressOffset) % 8)));
    }

  private:
    pin_t pin;
    uint8_t *states = nullptr;
};

#endif // MIDI_INPUT_LED_H_