#pragma once

#include "Selector.hpp"
#include <Hardware/IncrementDecrementButtons.hpp>

class IncrementDecrementSelector_Base : public virtual Selector {
  public:
    IncrementDecrementSelector_Base(const IncrementDecrementButtons &buttons,
                                    bool wrap = true)
        : buttons(buttons), wrap(wrap) {}

    void beginInput() override { buttons.begin(); }

    void update() override {
        switch (buttons.getState()) {
            case IncrementDecrementButtons::Increment: increment(); break;
            case IncrementDecrementButtons::Decrement: decrement(); break;
            case IncrementDecrementButtons::Reset: reset(); break;
            default: break;
        }
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

    void decrement() {
        setting_t setting = get();
        if (setting == 0) {
            if (wrap) {
                setting = getNumberOfSettings();
            } else {
                return;
            }
        }
        setting--;
        set(setting);
    }

  private:
    IncrementDecrementButtons buttons;
    bool wrap;
};

// -------------------------------------------------------------------------- //

class IncrementDecrementSelector : public IncrementDecrementSelector_Base {
  public:
    IncrementDecrementSelector(Selectable &selectable,
                               setting_t numberOfSettings,
                               const IncrementDecrementButtons &buttons,
                               bool wrap = true)
        : Selector(selectable, numberOfSettings),
          IncrementDecrementSelector_Base(buttons, wrap) {}

    void beginOutput() override {}
    void updateOutput(setting_t oldSetting, setting_t newSetting) override {}
};