#pragma once

#include "Selector.hpp"
#include <AH/Hardware/IncrementDecrementButtons.hpp>

BEGIN_CS_NAMESPACE

template <setting_t N, class Callback = EmptySelectorCallback>
class GenericIncrementDecrementSelector : public GenericSelector<N, Callback> {
    using Parent = GenericSelector<N, Callback>;

  public:
    GenericIncrementDecrementSelector(
        Selectable<N> &selectable, const Callback &callback,
        const AH::IncrementDecrementButtons &buttons, Wrap wrap = Wrap::Wrap)
        : GenericSelector<N, Callback>{selectable, callback}, buttons{buttons},
          wrap{wrap} {}

    void begin() override {
        Parent::begin();
        buttons.begin();
    }

    void update() override {
        Parent::update();
        using IncrDecrButtons = AH::IncrementDecrementButtons;
        switch (buttons.update()) {
            case IncrDecrButtons::Increment: this->increment(wrap); break;
            case IncrDecrButtons::Decrement: this->decrement(wrap); break;
            case IncrDecrButtons::Reset: this->reset(); break;
            case IncrDecrButtons::Nothing: break;
            default: break;
        }
    }

#ifdef AH_INDIVIDUAL_BUTTON_INVERT
    void invert() { buttons.invert(); }
#endif

    AH::IncrementDecrementButtons::State getButtonsState() const {
        return buttons.getState();
    }

  private:
    AH::IncrementDecrementButtons buttons;
    Wrap wrap;
};

// -------------------------------------------------------------------------- //

/**
 * @brief   Selector with two buttons (one to increment, one to decrement).
 * 
 * Pressing two buttons simultaneously resets to the default setting.
 * 
 * @htmlonly 
 * <object type="image/svg+xml" data="../../selector-increment-decrement-LED.svg"></object>
 * @endhtmlonly
 * 
 * @ingroup Selectors 
 * 
 * @tparam  N 
 *          The number of settings.
 */
template <setting_t N>
class IncrementDecrementSelector : public GenericIncrementDecrementSelector<N> {
  public:
    IncrementDecrementSelector(Selectable<N> &selectable,
                               const AH::IncrementDecrementButtons &buttons,
                               Wrap wrap = Wrap::Wrap)
        : GenericIncrementDecrementSelector<N>{
              selectable,
              {},
              buttons,
              wrap,
          } {}
};

END_CS_NAMESPACE