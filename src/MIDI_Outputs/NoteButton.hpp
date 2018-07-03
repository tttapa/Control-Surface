#pragma once

#include "../Hardware/Button.h"
#include "Abstract/NoteOut.h"

class NoteButton : public NoteOut {
  public:
    NoteButton(pin_t pin, uint8_t address, uint8_t channel,
               uint8_t velocity = 127)
        : NoteOut(address, channel, velocity), button{pin} {}

  private:
    void refresh() {
        Button::State state = button.getState();
        if (state == Button::Falling)
            sendOn();
        else if (state == Button::Rising)
            sendOff();
    }

    Button button;
};