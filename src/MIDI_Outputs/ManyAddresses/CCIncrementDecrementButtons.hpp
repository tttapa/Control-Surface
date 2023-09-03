#pragma once

#include <Banks/BankAddresses.hpp>
#include <MIDI_Outputs/Bankable/Abstract/MIDIIncrementDecrementButtons.hpp>
#include <MIDI_Senders/RelativeCCSender.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {
namespace ManyAddresses {
/**
 * @brief   A class of MIDIOutputElement%s that read the input of **two
 *          momentary push buttons** to increment or decrement a **relative 
 *          setting** using relative MIDI **Control Change** events.  
 *          An optional note number can be provided to reset the setting.
 * 
 * This version can be banked using an arbitrary list of alternative
 * addresses.
 *          
 * @tparam  NumBanks
 *          The number of variants/alternative addresses the element has.
 *
 * @ingroup ManyAddressesMIDIOutputElements
 */
template <setting_t NumBanks>
class CCIncrementDecrementButtons
    : public MIDIIncrementDecrementButtons<
          DualManyAddresses<NumBanks>, RelativeCCSender, DigitalNoteSender> {
  public:
    /**
     * @brief   Construct a new Bankable CCIncrementDecrementButtons object.
     * 
     * @param   bank
     *          The bank to add this element to.
     * @param   buttons
     *          The pins with the increment and decrement buttons connected.  
     *          The internal pull-up resistors will be enabled.
     * @param   addresses
     *          The list of MIDI addresses to use for the relative events, 
     *          containing the controller number [0, 119], channel [Channel_1, 
     *          Channel_16], and optional cable number [Cable_1, Cable_16].
     * @param   multiplier
     *          The multiplier for the relative events. When setting it to 2,
     *          it will scroll twice as fast, for example.
     * @param   resetNotes
     *          The list of MIDI addresses to use to reset the setting, 
     *          containing the note number [0, 127], channel [Channel_1, 
     *          Channel_16], and optional cable number [Cable_1, Cable_16]. 
     *          It will be triggered when the increment and decrement buttons
     *          are pressed simultaneously.
     * @param   relativeSender
     *          The MIDI sender to use for the relative events.
     * @param   resetSender
     *          The MIDI sender to use for reset events.
     */
    CCIncrementDecrementButtons(
        const Bank<NumBanks> &bank,
        const AH::IncrementDecrementButtons &buttons,
        const Array<MIDIAddress, NumBanks> &addresses, uint8_t multiplier = 1,
        const Array<MIDIAddress, NumBanks> &resetNotes = {},
        const RelativeCCSender &relativeSender = {},
        const DigitalNoteSender &resetSender = {})
        : MIDIIncrementDecrementButtons<DualManyAddresses<NumBanks>,
                                        RelativeCCSender, DigitalNoteSender>(
              {bank, {addresses, resetNotes}}, buttons, multiplier,
              relativeSender, resetSender) {}
};
} // namespace ManyAddresses
} // namespace Bankable

END_CS_NAMESPACE