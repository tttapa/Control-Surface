#pragma once

#include "Selector.hpp"
#include <Hardware/IncrementButton.hpp>

class IncrementSelector_Base : public virtual Selector {
  public:
    IncrementSelector_Base(const IncrementButton &button, bool wrap = true)
        : button(button), wrap(wrap) {}

    IncrementSelector_Base(const Button &button, bool wrap = true)
        : button{button}, wrap(wrap) {}

    void beginInput() override { button.begin(); }

    void update() override {
        if (button.getState() == IncrementButton::Increment)
            increment();
    }

    void increment() {
        setting_t setting = get();
        setting++;
        if (setting == getNumberOfSettings()) {
            if (wrap) {
                setting = 0;
            } else {
                return;
            }
        }
        set(setting);
    }

  private:
    IncrementButton button;
    bool wrap;
};

// -------------------------------------------------------------------------- //

class IncrementSelector : public virtual IncrementSelector_Base {
  public:
    IncrementSelector(Selectable &selectable, setting_t numberOfSettings,
                      const IncrementButton &button, bool wrap = true)
        : Selector(selectable, numberOfSettings),
          IncrementSelector_Base(button, wrap) {}

    IncrementSelector(Selectable &selectable, setting_t numberOfSettings,
                      const Button &button, bool wrap = true)
        : Selector(selectable, numberOfSettings),
          IncrementSelector_Base(button, wrap) {}

    void beginOutput() override {}
    void updateOutput(setting_t oldSetting, setting_t newSetting) override {}
};