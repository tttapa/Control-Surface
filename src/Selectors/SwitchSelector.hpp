#pragma once

#include "Selector.hpp"
#include <AH/Hardware/Button.hpp>

BEGIN_CS_NAMESPACE

template <class Callback = EmptySelectorCallback>
class GenericSwitchSelector : public GenericSelector<2, Callback> {
    using Parent = GenericSelector<2, Callback>;

  public:
    GenericSwitchSelector(Selectable<2> &selectable, const Callback &callback,
                          const AH::Button &button)
        : GenericSelector<2, Callback>(selectable, callback), button(button) {}

    void begin() override {
        Parent::begin();
        button.begin();
    }

    void update() override {
        Parent::update();
        AH::Button::State state = button.update();
        if (state == AH::Button::Falling)
            this->set(1);
        else if (state == AH::Button::Rising)
            this->set(0);
    }

    AH::Button::State getButtonState() const { return button.getState(); }

    /// @see @ref AH::Button::invert()
    void invert() { button.invert(); }

  private:
    AH::Button button;
};

/**
 * @brief   Selector that selects one of two settings, based on the state of a 
 *          toggle or momentary switch.
 * 
 * @htmlonly 
 * <object type="image/svg+xml" data="../../selector-one-toggle-switch-LED.svg"></object>
 * @endhtmlonly
 * 
 * @ingroup Selectors
 */
class SwitchSelector : public GenericSwitchSelector<> {
  public:
    SwitchSelector(Selectable<2> &selectable, const AH::Button &button)
        : GenericSwitchSelector<> {
              selectable,
              {},
              button,
          } {}
};

END_CS_NAMESPACE