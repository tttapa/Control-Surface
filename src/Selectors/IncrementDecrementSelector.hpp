#pragma once

#include "Selector.hpp"
#include <Hardware/IncrementDecrementButtons.hpp>

class IncrementDecrementSelector : public virtual Selector {
  public:
    IncrementDecrementSelector(Selectable &selectable,
                               setting_t numberOfSettings,
                               const IncrementDecrementButtons &buttons,
                               bool wrap = true)
        : Selector(selectable, numberOfSettings), buttons(buttons), wrap(wrap) {
    }

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
        if (setting == getNumberOfSettings())
            if (wrap)
                setting = 0;
            else
                return;
        set(setting);
    }

    void decrement() {
        setting_t setting = get();
        if (setting == 0)
            if (wrap)
                getNumberOfSettings();
            else
                return;
        setting--;
        set(setting);
    }

  private:
    IncrementDecrementButtons buttons;
    bool wrap;
};