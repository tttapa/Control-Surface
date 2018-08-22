#pragma once

#include "Selector.hpp"
#include <Def/Def.hpp>
#include <Hardware/ExtendedInputOutput/ExtendedInputOutput.h>

template <setting_t N>
class ManyButtonsSelector_Base : public virtual Selector<N> {
  protected:
    ManyButtonsSelector_Base(const PinList<N> &buttonPins)
        : buttonPins(buttonPins) {}

  public:
    void beginInput() override {
        for (const pin_t &pin : buttonPins)
            ExtIO::pinMode(pin, INPUT_PULLUP);
    }

    void update() override {
        for (setting_t i = 0; i < N; i++) {
            if (ExtIO::digitalRead(buttonPins[i]) == LOW) // TODO: invert?
                this->set(i);
        }
    }

  private:
    const PinList<N> buttonPins;
};

// -------------------------------------------------------------------------- //

template <setting_t N>
class ManyButtonsSelector : public ManyButtonsSelector_Base<N> {
  public:
    ManyButtonsSelector(Selectable<N> &selectable, const PinList<N> &buttonPins)
        : Selector<N>(selectable), ManyButtonsSelector_Base<N>(buttonPins) {}

    void beginOutput() override {}
    void updateOutput(__attribute__((unused)) setting_t oldSetting,
                      __attribute__((unused)) setting_t newSetting) override {}
};