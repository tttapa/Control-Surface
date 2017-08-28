#include "MIDI_Input_Element.h"
#include "ExtendedInputOutput/ExtendedInputOutput.h"

using namespace ExtIO;

class VU : public MIDI_Input_Element {
  public:
    VU(pin_t start, pin_t length, uint8_t id, uint8_t nb_ids, bool decay = true)
      : MIDI_Input_Element(id, 1, nb_ids, 1), start(start), overloadpin(0), length(length), overload(false),
        decayTime(1800 / length), decay(decay) {
      for (pin_t pin = 0; pin < length; pin++) {
        pinMode(start + pin, OUTPUT);
      }
    }
    VU(pin_t start, pin_t length, pin_t overloadpin, uint8_t id, uint8_t nb_ids, bool decay = true)
      : MIDI_Input_Element(id, 1, nb_ids, 1), start(start), length(length), overloadpin(overloadpin),
        overload(true), decayTime(1800 / length), decay(decay) {
      for (pin_t pin = 0; pin < length; pin++) {
        pinMode(start + pin, OUTPUT);
      }
      pinMode(overloadpin, OUTPUT);
    }
    void setValue(uint8_t value) {
      this->value = value <= length ? value : length;
      prevDecayTime = millis();
      display();
    }
    void refresh() {
      if (decay && ((millis() - prevDecayTime) > decayTime) && (value > 0)) {
        value--;
        prevDecayTime = millis();
        display();
      }
    }
    bool update(uint8_t header, uint8_t data1, uint8_t data2) {
      uint8_t targetChannel = (header & 0b1111) + 1;
      uint8_t targetAddress = data1 >> 4;
      if (targetAddress != this->address + addressOffset) {
        // Serial.println("\t\t\tAddress not found");
        return false;
      }
      if (targetChannel != this->channel) {
        // Serial.println("\t\t\tChannel not found");
        return false;
      }
      if ((header & 0xF0) == CHANNEL_PRESSURE) {
        // Serial.println("\tMatch (VU)");
        uint8_t data = data1 & 0xF;
        if (data == 0xF) { // clear overload
          if (overload)
            digitalWrite(overloadpin, LOW);
        }
        else if (data == 0xE) {
          if (overload)
            digitalWrite(overloadpin, HIGH);
        }
        else if (data == 0xD)
          ; // not implemented
        else
          setValue(data * length / 12);
      } else {
        // Serial.println("\t\t\tWrong message type");
        return false; // not a Note On or Note Off event, but channel and address match
      }
      refresh();
      return true;
    }
  private:
    const pin_t start, length, overloadpin;
    const bool decay;
    uint8_t value;
    const bool overload;
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
