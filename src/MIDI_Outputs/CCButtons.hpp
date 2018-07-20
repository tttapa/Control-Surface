#pragma once

#include <MIDI_Outputs/Abstract/MIDIButtons.hpp>
#include <MIDI_Senders/DigitalCCSender.hpp>

/**
 * @brief   A class for collections of non-bankable momentary buttons that send
 *          MIDI Control Change events.
 *
 * @tparam  NUMBER_OF_BUTTONS
 *          The number of buttons in the collection.
 */
template <uint8_t NUMBER_OF_BUTTONS>
class CCButtons : public MIDIButtons<NUMBER_OF_BUTTONS, DigitalCCSender> {
  public:
    /**
     * @brief   Create a new CCButton with the given pin,
     *          the given controller number and channel.
     *
     * @param   buttons
     *          An array of digital input pins with the buttons connected.
     *          The internal pull-up resistors will be enabled.
     * @param   controller
     *          The MIDI Controller number of the first button. [0, 119]
     * @param   channel
     *          The MIDI Channel of the first button. [1, 16]
     * @param   addressIncrement
     *          The number of addresses to increment for each next button.
     *          E.g. if `baseAddress` is 8, and `addressIncrement` is 2,
     *          then the first button will send on address 8, the second
     *          button will send on address 10, button three on address 12, etc.
     * @param   channelIncrement
     *          The number of channels to increment for each next button.
     */
    CCButtons(const Array<Button, NUMBER_OF_BUTTONS> &buttons,
              uint8_t baseAddress, uint8_t baseChannel,
              uint8_t addressIncrement, uint8_t channelIncrement)
        : MIDIButtons<NUMBER_OF_BUTTONS, DigitalCCSender>(
              buttons, baseAddress, baseChannel, addressIncrement,
              channelIncrement) {}
};