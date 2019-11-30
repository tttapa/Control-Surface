#pragma once

#include <Banks/BankAddresses.hpp>
#include <MIDI_Outputs/Bankable/Abstract/MIDIIncrementDecrementButtons.hpp>
#include <MIDI_Senders/RelativeCCSender.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {

/**
 * @brief   A class of MIDIOutputElement%s that read the input of **two
 *          momentary push buttons** to increment or decrement a **relative 
 *          setting** using relative MIDI **Control Change** events.  
 *          An optional note number can be provided to reset the setting.
 * 
 * This version can be banked.
 *
 * @ingroup BankableMIDIOutputElements
 */
class CCIncrementDecrementButtons
    : public MIDIIncrementDecrementButtons<TwoSingleAddresses, RelativeCCSender,
                                           DigitalNoteSender> {
  public:
    /**
     * @brief   Construct a new Bankable CCIncrementDecrementButtons object.
     * 
     * @param   config
     *          The bank configuration to use: the bank to add this element to,
     *          and whether to change the address, channel or cable number.
     * @param   buttons
     *          The pins with the increment and decrement buttons connected.  
     *          The internal pull-up resistors will be enabled.
     * @param   address
     *          The address to use for the relative events, containing the 
     *          controller number [0, 119], channel [CHANNEL_1, CHANNEL_16], and
     *          optional cable number [0, 15].
     * @param   multiplier
     *          The multiplier for the relative events. When setting it to 2,
     *          it will scroll twice as fast, for example.
     * @param   resetNote
     *          The MIDI address to use to reset the setting, containing the 
     *          note number [0, 119], channel [CHANNEL_1, CHANNEL_16], and 
     *          optional cable number [0, 15]. It will be triggered when the 
     *          increment and decrement buttons are pressed simultaneously.
     * @param   relativeSender
     *          The MIDI sender to use for the relative events.
     * @param   resetSender
     *          The MIDI sender to use for reset events.
     */
    CCIncrementDecrementButtons(const OutputBankConfig &config,
                                const AH::IncrementDecrementButtons &buttons,
                                const MIDICNChannelAddress &address,
                                uint8_t multiplier = 1,
                                const MIDICNChannelAddress &resetNote = {},
                                const RelativeCCSender &relativeSender = {},
                                const DigitalNoteSender &resetSender = {})
        : MIDIIncrementDecrementButtons<TwoSingleAddresses, RelativeCCSender,
                                        DigitalNoteSender>(
              {config, {address, resetNote}}, buttons, multiplier,
              relativeSender, resetSender) {}
};

} // namespace Bankable

END_CS_NAMESPACE