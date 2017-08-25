#include "MIDI_Input_Element.h"
#include "ExtendedInputOutput/ExtendedInputOutput.h"

using namespace ExtIO;

class VU {
  public:
    VU(pin_t start, pin_t length)
      : start(start), length(length), decayTime(1800 / length) {
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
