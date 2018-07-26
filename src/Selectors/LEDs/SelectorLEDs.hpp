#pragma once

#include <Def/Def.hpp>
#include <Hardware/ExtendedInputOutput/ExtendedInputOutput.h>
#include <Selectors/Selector.hpp>

template <setting_t NUMBER_OF_LEDS>
class SelectorLEDs : public virtual Selector {
  public:
    SelectorLEDs(const PinList<NUMBER_OF_LEDS> &ledPins) : ledPins(ledPins) {}

    void beginOutput() override {
        for (const pin_t &pin : ledPins)
            ExtIO::pinMode(pin, OUTPUT);
        ExtIO::digitalWrite(ledPins[get()], HIGH);
    }

    void updateOutput(setting_t oldSetting, setting_t newSetting) override {
        ExtIO::digitalWrite(ledPins[oldSetting], LOW);
        ExtIO::digitalWrite(ledPins[newSetting], HIGH);
    }

  private:
    const PinList<NUMBER_OF_LEDS> ledPins;
};