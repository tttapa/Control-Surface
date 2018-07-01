#ifndef CONTROL_SURFACE_MIDI_OUTPUTS_CCBUTTONLATCHING_HPP_
#define CONTROL_SURFACE_MIDI_OUTPUTS_CCBUTTONLATCHING_HPP_

#include "CCOut.h"
#include "Button.h"

class CCButtonLatch : public DigitalCCOut
{
    CCButtonLatch(pin_t pin, uint8_t address, uint8_t channel, uint8_t offValue = 0, uint8_t onValue = 127)
        : DigitalCCOut(address, channel, offValue, onValue), button{pin}
    {
    }

  private:
    void refresh()
    {
        Button::State state = button.getState();
        if (state == Button::Falling || state == Button::Rising)
        {
            sendOn();
            sendOff();
        }
    }

    Button button;
};

#endif // CONTROL_SURFACE_MIDI_OUTPUTS_CCBUTTONLATCHING_HPP_