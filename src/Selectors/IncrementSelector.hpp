#pragma once

#include "Selector.hpp"
#include <AH/Hardware/IncrementButton.hpp>

BEGIN_CS_NAMESPACE

template <setting_t N, class Callback = EmptySelectorCallback>
class GenericIncrementSelector : public GenericSelector<N, Callback> {
    using Parent = GenericSelector<N, Callback>;

  public:
    GenericIncrementSelector(Selectable<N> &selectable,
                             const Callback &callback,
                             const AH::IncrementButton &button)
        : GenericSelector<N, Callback>{selectable, callback}, button{button} {}

    void begin() override {
        Parent::begin();
        button.begin();
    }

    void update() override {
        if (button.update() != AH::IncrementButton::Nothing)
            this->increment(Wrap::Wrap);
    }

    AH::IncrementButton::State getButtonState() const {
        return button.getState();
    }

#ifdef AH_INDIVIDUAL_BUTTON_INVERT
    void invert() { button.invert(); }
#endif

  private:
    AH::IncrementButton button;
};

// -------------------------------------------------------------------------- //

/**
 * @brief   Selector with one button that increments the selection.
 * 
 * @htmlonly 
 * <object type="image/svg+xml" data="../../selector-increment-LED.svg"></object>
 * @endhtmlonly
 * 
 * @ingroup Selectors 
 * 
 * @tparam  N 
 *          The number of settings.
 */
template <setting_t N>
class IncrementSelector : virtual public GenericIncrementSelector<N> {
  public:
    IncrementSelector(Selectable<N> &selectable,
                      const AH::IncrementButton &button)
        : GenericIncrementSelector<N>{selectable, {}, button} {}

    IncrementSelector(Selectable<N> &selectable, const AH::Button &button)
        : GenericIncrementSelector<N>{selectable, {}, button} {}
};

END_CS_NAMESPACE