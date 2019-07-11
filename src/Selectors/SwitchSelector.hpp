#pragma once

#include "Selector.hpp"
#include <Hardware/Button.hpp>

/// A class for selectors that select one of two settings, based on the state
/// of a toggle switch.
class SwitchSelector : virtual public Selector<2> {
  public:
    SwitchSelector(Selectable<2> &selectable, const Button &button)
        : Selector<2>(selectable), button(button) {}

    void beginInput() override { button.begin(); }
    void beginOutput() override {}
    void updateOutput(setting_t, setting_t) override {}

    void update() override {
        Button::State state = button.getState();
        if (state == Button::Falling)
            this->set(1);
        else if (state == Button::Rising)
            this->set(0);
    }

  private:
    Button button;
};

/// @todo   Move to correct file
class SwitchSelectorLEDs : public SwitchSelector {
  public:
    SwitchSelectorLEDs(Selectable<2> &selectable, pin_t ledPin,
                       const Button &button)
        : SwitchSelector(selectable, button), ledPin(ledPin) {}

    void beginOutput() override { pinMode(ledPin, OUTPUT); }

    void updateOutput(setting_t oldSetting, setting_t newSetting) override {
        (void)oldSetting;
        digitalWrite(ledPin, newSetting);
    }

  private:
    const pin_t ledPin;
};