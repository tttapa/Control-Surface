#include "MIDI_Input_Element.h"
#include "ExtendedInputOutput/ExtendedInputOutput.h"

using namespace ExtIO;

class VU : public MIDI_Input_Element {
  public:
    VU(pin_t start, pin_t length, uint8_t id, uint8_t nb_ids)
      : MIDI_Input_Element(id, 1, nb_ids, 1), start(start), length(length), decayTime(1800 / length) {
      for (pin_t pin = 0; pin < length; pin++) {
        pinMode(start + pin, OUTPUT);
      }
    }
    void setValue(uint8_t value) {
      this->value = value <= length ? value : length;
      prevDecayTime = millis();
      display();
    }
    void refresh() {
      if (millis() - prevDecayTime > decayTime && value > 0) {
        value--;
        prevDecayTime = millis();
        display();
      }
    }
    bool update(uint8_t header, uint8_t data1, uint8_t data2) {
      uint8_t targetChannel = (header & 0b1111) + 1;
      uint8_t targetAddress = data1 >> 4;
      if (targetAddress < this->address || targetAddress >= this->address + nb_addresses) {
        Serial.println("\t\t\tAddress not found");
        return false;
      }
      if (targetChannel != this->channel) {
        Serial.println("\t\t\tChannel not found");
        return false;
      }
      if ((header & 0xF0) == CHANNEL_PRESSURE) {
        Serial.println("\tMatch (VU)");
        uint8_t data = data1 & 0xF;
        if (data == 0xF)
          ; // clear overload
        else if (data == 0xE)
          ; // set overload
        else if (data == 0xD)
          ; // not implemented
        else
          setValue(data * length / 12);
        Serial.println(value);
      } else {
        Serial.println("\t\t\tWrong message type");
        return false; // not a Note On or Note Off event, but channel and address match
      }
      refresh();
      return true;
    }
  private:
    pin_t start, length;
    uint8_t value;
    const unsigned long decayTime;
    unsigned long prevDecayTime = 0;

    void display() {
      for (pin_t pin = 0; pin < value; pin++) {
        digitalWrite(start + pin, HIGH);
      }
      for (pin_t pin = value; pin < length; pin++) {
        digitalWrite(start + pin, LOW);
      }
    }
};
