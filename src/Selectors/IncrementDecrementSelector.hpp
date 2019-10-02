#pragma once

#include "Selector.hpp"
#include <Hardware/IncrementDecrementButtons.hpp>

BEGIN_CS_NAMESPACE

template <setting_t N>
class IncrementDecrementSelector_Base : virtual public Selector<N> {
  public:
    IncrementDecrementSelector_Base(const IncrementDecrementButtons &buttons,
                                    Wrap wrap = Wrap::Wrap)
        : buttons(buttons), wrap(wrap) {}

    void beginInput() override { buttons.begin(); }

    void update() override {
        using IncrDecrButtons = IncrementDecrementButtons;
        switch (buttons.getState()) {
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

  private:
    IncrementDecrementButtons buttons;
    Wrap wrap;
};

// -------------------------------------------------------------------------- //

/**
 * @brief   Selector with two buttons (one to increment, one to decrement).
 * 
 * Pressing two buttons simultaneously resets to the default setting.
 * 
 * @ingroup Selectors 
 * 
 * @tparam  N 
 *          The number of settings.
 */
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

END_CS_NAMESPACE