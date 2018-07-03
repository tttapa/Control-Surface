#pragma once

#include "../Hardware/Button.h"
#include "Abstract/CCOut.h"

class CCButtonLatching : public DigitalCCOut {
    CCButtonLatching(pin_t pin, uint8_t address, uint8_t channel,
                  uint8_t offValue = 0, uint8_t onValue = 127)
        : DigitalCCOut(address, channel, offValue, onValue), button{pin} {}

  private:
    void refresh() {
        Button::State state = button.getState();
        if (state == Button::Falling || state == Button::Rising) {
            sendOn();
            sendOff();
        }
    }

    Button button;
};