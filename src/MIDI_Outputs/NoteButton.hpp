#ifndef CONTROL_SURFACE_MIDI_OUTPUTS_NOTEBUTTON_HPP_
#define CONTROL_SURFACE_MIDI_OUTPUTS_NOTEBUTTON_HPP_

#include "../Hardware/Button.h"
#include "NoteOut.h"

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

#endif // CONTROL_SURFACE_MIDI_OUTPUTS_NOTEBUTTON_HPP_