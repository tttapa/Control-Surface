#pragma once

#include "Selector.hpp"
#include <Hardware/IncrementDecrementButtons.hpp>

template <setting_t N>
class IncrementDecrementSelector_Base : virtual public Selector<N> {
  public:
    IncrementDecrementSelector_Base(const IncrementDecrementButtons &buttons,
                                    Wrap wrap = Wrap::Wrap)
        : buttons(buttons), wrap(wrap) {}

    void beginInput() override { buttons.begin(); }

    void update() override {
        using IncrDecrButtons = IncrementDecrementButtons;
        switch (buttons.update()) {
            case IncrDecrButtons::Increment: this->increment(wrap); break;
            case IncrDecrButtons::Decrement: this->decrement(wrap); break;
            case IncrDecrButtons::Reset: this->reset(); break;
            case IncrDecrButtons::Nothing: break;
            default: break;
        }
    }

#ifdef INDIVIDUAL_BUTTON_INVERT
    void invert() { buttons.invert(); }
#endif

    IncrementDecrementButtons::State getButtonsState() const {
        return buttons.getState();
    }

  private:
    IncrementDecrementButtons buttons;
    Wrap wrap;
};

// -------------------------------------------------------------------------- //

template <setting_t N>
class IncrementDecrementSelector : public IncrementDecrementSelector_Base<N> {
  public:
    IncrementDecrementSelector(Selectable<N> &selectable,
                               const IncrementDecrementButtons &buttons,
                               Wrap wrap = Wrap::Wrap)
        : Selector<N>(selectable), IncrementDecrementSelector_Base<N>(buttons,
                                                                      wrap) {}

    void beginOutput() override {}
    void updateOutput(UNUSED_PARAM setting_t oldSetting,
                      UNUSED_PARAM setting_t newSetting) override {}
};