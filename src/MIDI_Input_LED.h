#ifndef MIDI_INPUT_LED_H_
#define MIDI_INPUT_LED_H_

#include "MIDI_Input_Element.h"
#include "ExtendedInputOutput/ExtendedInputOutput.h"

using namespace ExtIO;

class MIDI_Input_LED : public MIDI_Input_Element {
  public:
    MIDI_Input_LED(pin_t pin, uint8_t address, uint8_t channel, size_t nb_addresses = 1, size_t nb_channels = 1)
      : MIDI_Input_Element(address, channel, nb_addresses, nb_channels), pin(pin) {
      states = (uint8_t*)malloc((nb_channels * nb_addresses + 7) / 8);
      memset(states, 0, (nb_channels * nb_addresses + 7) / 8);
      pinMode(pin, OUTPUT);
    }
    ~MIDI_Input_LED() {
      free (states);
      pinMode(pin, INPUT);
    }
    bool update(uint8_t header, uint8_t data1, uint8_t data2) {
      uint8_t targetChannel = (header & 0b1111) + 1;
      uint8_t targetAddress = data1;
      if (targetAddress < this->address || targetAddress >= this->address + nb_addresses) {
        Serial.println("\t\t\tAddress not found");
        return false;
      }
      if (targetChannel < this->channel || targetChannel >= this->channel + nb_channels) {
        Serial.println("\t\t\tChannel not found");
        return false;
      }
      uint8_t addressIndex = targetAddress - this->address;
      uint8_t channelIndex = targetChannel - this->channel;
      uint8_t ledIndex = channelIndex + nb_channels * addressIndex;
      uint8_t statesIndex = ledIndex >> 3; // ledIndex / 8
      uint8_t statesBit = ledIndex & 0b111; // ledIndex % 8
      if (header == NOTE_OFF || (header == NOTE_ON && data2 == 0)) {
        states[statesIndex] &= ~(1 << statesBit);
      } else if (header == NOTE_ON) {
        states[statesIndex] |= 1 << statesBit;
      } else {
        Serial.println("\t\t\tWrong message type");
        return false; // not a Note On or Note Off event, but channel and address match
      }
      refresh();
      return true;
    }
    void refresh() {
      digitalWrite(pin, states[(channelOffset + nb_channels * addressOffset) / 8] & (1 << ((channelOffset + nb_channels * addressOffset) % 8)));
    }

    void print() {
      Serial.printf("Channels:\t%d - %d\r\n", channel, channel + nb_channels - 1);
      Serial.printf("Addresses:\t%d - %d\r\n", address, address + nb_addresses - 1);
    }
  private:
    pin_t pin;
    uint8_t *states = nullptr;
};

#endif // MIDI_INPUT_LED_H_