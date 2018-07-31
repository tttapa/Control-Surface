#pragma once

#include "Selector.hpp"
#include <Hardware/IncrementButton.hpp>

template <setting_t N>
class IncrementSelector_Base : public virtual Selector<N> {
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
        setting_t setting = this->get();
        setting++;
        if (setting == N) {
            if (wrap) {
                setting = 0;
            } else {
                return;
            }
        }
        this->set(setting);
    }

  private:
    IncrementButton button;
    bool wrap;
};

// -------------------------------------------------------------------------- //

template <setting_t N>
class IncrementSelector : public virtual IncrementSelector_Base<N> {
  public:
    IncrementSelector(Selectable<N> &selectable, const IncrementButton &button,
                      bool wrap = true)
        : Selector<N>(selectable), IncrementSelector_Base<N>(button, wrap) {}

    IncrementSelector(Selectable<N> &selectable, const Button &button,
                      bool wrap = true)
        : Selector<N>(selectable), IncrementSelector_Base<N>(button, wrap) {}

    void beginOutput() override {}
    void updateOutput(setting_t oldSetting, setting_t newSetting) override {}
};