#pragma once

#include "Selector.hpp"
#include <Hardware/Button.hpp>

BEGIN_CS_NAMESPACE

template <class Callback = EmptySelectorCallback>
class GenericSwitchSelector : public GenericSelector<2, Callback> {
    using Parent = GenericSelector<2, Callback>;

  public:
    GenericSwitchSelector(Selectable<2> &selectable, const Callback &callback,
                          const Button &button)
        : GenericSelector<2, Callback>{selectable, callback}, button{button} {}

    void begin() override {
        Parent::begin();
        button.begin();
    }

    void update() override {
        Parent::update();
        Button::State state = button.getState();
        if (state == Button::Falling)
            this->set(1);
        else if (state == Button::Rising)
            this->set(0);
    }

#ifdef INDIVIDUAL_BUTTON_INVERT
    void invert() { button.invert(); }
#endif

  private:
    Button button;
};

/**
 * @brief   Selector that selects one of two settings, based on the state of a 
 *          toggle or momentary switch.
 * 
 * @ingroup Selectors
 */
class SwitchSelector : public GenericSwitchSelector<> {
  public:
    SwitchSelector(Selectable<2> &selectable, const Button &button)
        : GenericSwitchSelector<>{
              selectable,
              {},
              button,
          } {}
};

END_CS_NAMESPACE