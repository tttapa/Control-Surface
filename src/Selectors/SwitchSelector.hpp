#pragma once

#include "Selector.hpp"
#include <Hardware/Button.hpp>

class SwitchSelector : virtual public Selector<2> {
  public:
    SwitchSelector(Selectable<2> &selectable, const Button &button)
        : Selector<2>(selectable), button(button) {}

    void beginInput() override { button.begin(); }
    void beginOutput() override {}
    void updateOutput(setting_t, setting_t) override {}

    void update() override {
        Button::State state = button.getState();
        if (state == Button::Falling)
            this->set(1);
        else if (state == Button::Rising)
            this->set(0);
    }

  private:
    Button button;
};