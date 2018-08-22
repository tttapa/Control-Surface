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
            this->increment(wrap);
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
    void updateOutput(__attribute__((unused)) setting_t oldSetting,
                      __attribute__((unused)) setting_t newSetting) override {}
};