#pragma once

#include <Def/Def.hpp>
#include <Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>
#include <Selectors/Selector.hpp>

BEGIN_CS_NAMESPACE

template <setting_t N>
class SelectorLEDs : virtual public Selector<N> {
  public:
    SelectorLEDs(const PinList<N> &ledPins) : ledPins(ledPins) {}

    void beginOutput() override {
        for (const pin_t &pin : ledPins)
            ExtIO::pinMode(pin, OUTPUT);
        ExtIO::digitalWrite(ledPins[this->get()], HIGH);
    }

    void updateOutput(setting_t oldSetting, setting_t newSetting) override {
        ExtIO::digitalWrite(ledPins[oldSetting], LOW);
        ExtIO::digitalWrite(ledPins[newSetting], HIGH);
    }

  private:
    const PinList<N> ledPins;
};

END_CS_NAMESPACE