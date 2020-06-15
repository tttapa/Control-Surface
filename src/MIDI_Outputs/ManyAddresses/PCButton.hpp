#pragma once

#include <Banks/BankAddresses.hpp>
#include <MIDI_Outputs/Bankable/Abstract/MIDIButton.hpp>
#include <MIDI_Senders/ProgramChangeSender.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {
namespace ManyAddresses {

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a **momentary
 *          push button or switch**, and send out MIDI **Program Change** 
 *          events.
 * 
 * A Program Change event is sent when the button is pressed.  
 * The button is debounced in software.  
 * This version can be banked using an arbitrary list of alternative
 * addresses.
 *          
 * @tparam  NumBanks
 *          The number of variants/alternative addresses the element has.
 *
 * @ingroup ManyAddressesMIDIOutputElements
 */
template <setting_t NumBanks>
class PCButton : public Bankable::MIDIButton<ManyAddresses<NumBanks>,
                                             ProgramChangeSender> {
  public:
    /**
     * @brief   Create a new PCButton object with the given pin, program number
     *          and channel.
     * 
     * @param   bank
     *          The bank that selects the address to use.
     * @param   pin
     *          The digital input pin to read from.  
     *          The internal pull-up resistor will be enabled.
     * @param   addresses
     *          The list of MIDI addresses containing the program number 
     *          [0, 127], channel [CHANNEL_1, CHANNEL_16], and optional cable 
     *          number [CABLE_1, CABLE_16].
     */
    PCButton(const Bank<NumBanks> &bank, pin_t pin,
             const Array<MIDIAddress, NumBanks> &addresses)
        : MIDIButton<ManyAddresses<NumBanks>, ProgramChangeSender>{
              {bank, addresses}, pin, {}} {}
};

} // namespace ManyAddresses
} // namespace Bankable

END_CS_NAMESPACE