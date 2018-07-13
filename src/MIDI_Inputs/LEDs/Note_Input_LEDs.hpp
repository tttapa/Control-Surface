#pragma once

#include "../../Hardware/ExtendedInputOutput/ExtendedInputOutput.h"
#include "../Note_Input.hpp"

class Note_Input_LED_Base : public virtual Note_Input_Base {
  public:
    Note_Input_LED_Base(pin_t ledPin) : ledPin(ledPin) {
        ExtIO::pinMode(ledPin, OUTPUT);
    }

  private:
    void display() const override {
        DEBUGFN("");
        ExtIO::digitalWrite(ledPin, getValue());
    }

  private:
    const pin_t ledPin;
};

// -------------------------------------------------------------------------- //

class Note_Input_LED : public Note_Input, public Note_Input_LED_Base {
  public:
    Note_Input_LED(uint8_t note, uint8_t channel, pin_t ledPin)
        : Note_Input_Base(note, channel), Note_Input(note, channel),
          Note_Input_LED_Base(ledPin) {}
};

// -------------------------------------------------------------------------- //

template <uint8_t NUMBER_OF_BANKS>
class Note_Input_Bankable_LED : public Note_Input_Bankable<NUMBER_OF_BANKS>,
                                public Note_Input_LED_Base {
  public:
    Note_Input_Bankable_LED(uint8_t note, uint8_t channel, pin_t ledPin)
        : Note_Input_Base(note, channel), Note_Input_Bankable<NUMBER_OF_BANKS>(
                                              note, channel),
          Note_Input_LED_Base(ledPin) {}
};