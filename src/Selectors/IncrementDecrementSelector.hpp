#pragma once

#include "Selector.hpp"
#include <Hardware/IncrementDecrementButtons.hpp>

template <setting_t N>
class IncrementDecrementSelector_Base : virtual public Selector<N> {
  public:
    IncrementDecrementSelector_Base(const IncrementDecrementButtons &buttons,
                                    bool wrap = true)
        : buttons(buttons), wrap(wrap) {}

    void beginInput() override { buttons.begin(); }

    void update() override {
        using IncrDecrButtons = IncrementDecrementButtons;
        switch (buttons.getState()) {
            case IncrDecrButtons::Increment: this->increment(wrap); break;
            case IncrDecrButtons::Decrement: this->decrement(wrap); break;
            case IncrDecrButtons::Reset: this->reset(); break;
            default: break;
        }
    }

  private:
    IncrementDecrementButtons buttons;
    bool wrap;
};

// -------------------------------------------------------------------------- //

template <setting_t N>
class IncrementDecrementSelector : public IncrementDecrementSelector_Base<N> {
  public:
    IncrementDecrementSelector(Selectable<N> &selectable,
                               const IncrementDecrementButtons &buttons,
                               bool wrap = true)
        : Selector<N>(selectable), IncrementDecrementSelector_Base<N>(buttons,
                                                                      wrap) {}

    void beginOutput() override {}
    void updateOutput(UNUSED_PARAM setting_t oldSetting,
                      UNUSED_PARAM setting_t newSetting) override {}
};