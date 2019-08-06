#pragma once

#include <MIDI_Outputs/Bankable/Abstract/MIDIIncrementDecrementButtons.hpp>
#include <MIDI_Senders/RelativeCCSender.hpp>

namespace Bankable {

/**
 * @brief   A class of MIDIOutputElement%s that read the input of **two
 *          momentary push buttons** to increment or decrement a **relative 
 *          setting** using relative MIDI **Control Change** events.  
 *          An optional note number can be provided to reset the setting.
 * 
 *          This version cannot be banked.
 *
 * @ingroup MIDIOutputElements
 */
class CCIncrementDecrementButtons
    : public MIDIIncrementDecrementButtons<RelativeCCSender,
                                           DigitalNoteSender> {
  public:
    /**
     * @brief   Construct a new CCIncrementDecrementButtons object.
     * 
     * @param   buttons
     *          The pins with the increment and decrement buttons connected.  
     *          The internal pull-up resistors will be enabled.
     * @param   controller
     *          The MIDI Controller number for the relative events. [0, 119]
     * @param   channel
     *          The MIDI channel. [1, 16]
     * @param   multiplier
     *          The multiplier for the relative events. When setting it to 2,
     *          it will scroll twice as fast.
     * @param   resetNote
     *          The MIDI note number to use to reset the setting. It will be 
     *          triggered when the increment and decrement buttons are pressed
     *          simultaneously. [0, 127]
     * 
     * @ingroup MIDIOutputElementConstructors
     */
    CCIncrementDecrementButtons(const OutputBankConfig &config,
                                const IncrementDecrementButtons &buttons,
                                const MIDICNChannelAddress &address,
                                uint8_t multiplier = 1,
                                const MIDICNChannelAddress &resetNote = {},
                                const RelativeCCSender &relativeSender = {},
                                const DigitalNoteSender &resetSender = {})
        : MIDIIncrementDecrementButtons<RelativeCCSender, DigitalNoteSender>(
              config, buttons, address, multiplier, resetNote, relativeSender,
              resetSender) {}
};

} // namespace Bankable