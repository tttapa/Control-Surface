#pragma once

#include "Selector.hpp"
#include <Hardware/Button.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   Selector that selects one of two settings, based on the state of a 
 *          toggle or momentary switch.
 * 
 * @ingroup Selectors
 */
class SwitchSelector : public Selector<2> {
  public:
    SwitchSelector(Selectable<2> &selectable, const Button &button)
        : Selector<2>{selectable}, button{button} {}

  private:
    using Parent = Selector<2>;

  public:
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

END_CS_NAMESPACE