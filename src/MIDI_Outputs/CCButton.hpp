#pragma once

#include <MIDI_Outputs/Abstract/MIDIButton.hpp>
#include <MIDI_Senders/DigitalCCSender.hpp>

/**
 * @brief   A class for non-bankable momentary buttons that send MIDI Control
 *          Change events.
 */
class CCButton : public MIDIButton<DigitalCCSender> {
  public:
    /**
     * @brief   Create a new CCButton with the given pin,
     *          the given controller number and channel.
     *
     * @param   pin
     *          The digital input pin with the button connected.
     *          The internal pull-up resistor will be enabled.
     * @param   controller
     *          The MIDI Controller number. [0, 119]
     * @param   channel
     *          The MIDI Channel. [1, 16]
     */
    CCButton(pin_t pin, uint8_t controller, uint8_t channel)
        : MIDIButton<DigitalCCSender>(pin, controller, channel) {}
};