#pragma once

#include "Selector.hpp"
#include <Def/Def.hpp>
#include <Hardware/ExtendedInputOutput/ExtendedInputOutput.h>

template <setting_t NUMBER_OF_BUTTONS>
class ManyButtonsSelector_Base : public virtual Selector {
  public:
    ManyButtonsSelector_Base(const PinList<NUMBER_OF_BUTTONS> &buttonPins)
        : buttonPins(buttonPins) {}

    void beginInput() override {
        for (const pin_t &pin : buttonPins)
            ExtIO::pinMode(pin, INPUT_PULLUP);
    }

    void update() override {
        for (setting_t i = 0; i < NUMBER_OF_BUTTONS; i++) {
            if (ExtIO::digitalRead(buttonPins[i]) == LOW) // TODO: invert?
                set(i);
        }
    }

  private:
    const PinList<NUMBER_OF_BUTTONS> buttonPins;
};

// -------------------------------------------------------------------------- //

template <setting_t NUMBER_OF_BUTTONS>
class ManyButtonsSelector : public ManyButtonsSelector_Base<NUMBER_OF_BUTTONS> {
  public:
    ManyButtonsSelector(Selectable &selectable,
                        const PinList<NUMBER_OF_BUTTONS> &buttonPins)
        : Selector(selectable, NUMBER_OF_BUTTONS),
          ManyButtonsSelector_Base<NUMBER_OF_BUTTONS>(buttonPins) {}

    void beginOutput() override {}
    void updateOutput(setting_t oldSetting, setting_t newSetting) override {}
};