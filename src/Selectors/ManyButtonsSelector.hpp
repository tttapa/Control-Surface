#pragma once

#include "Selector.hpp"
#include <Def/Def.hpp>
#include <Hardware/ExtendedInputOutput/ExtendedInputOutput.h>

template <setting_t NUMBER_OF_BUTTONS>
class ManyButtonsSelector : public virtual Selector {
  public:
    ManyButtonsSelector(Selectable &selectable,
                        const PinList<NUMBER_OF_BUTTONS> &buttonPins)
        : Selector(selectable, NUMBER_OF_BUTTONS), buttonPins(buttonPins) {}

    void beginInput() override {
        for (const pin_t &pin : buttonPins)
            ExtIO::pinMode(pin, INPUT_PULLUP);
    }

    void update() override {
        for (setting_t i = 0; i < NUMBER_OF_BUTTONS; i++) {
            if (ExtIO::digitalRead(pin) == LOW) // TODO: invert?
                set(i);
        }
    }

  private:
    const PinList<NUMBER_OF_BUTTONS> buttonPins;
};