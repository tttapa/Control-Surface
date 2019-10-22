#pragma once

#include <Banks/BankAddresses.hpp>
#include <MIDI_Outputs/Bankable/Abstract/MIDIFilteredAnalog.hpp>
#include <MIDI_Senders/ContinuousCCSender.hpp>

BEGIN_CS_NAMESPACE
namespace Bankable {
namespace ManyAddresses {

/**
 * @brief   A class of MIDIOutputElement%s that read the analog input from a
 *          **potentiometer or fader**, and send out 7-bit MIDI **Control
 *          Change** events.
 * 
 * The analog input is filtered and hysteresis is applied for maximum
 * stability.  
 * This version can be banked using an arbitrary list of alternative
 * addresses.
 *          
 * @tparam  N
 *          The number of variants/alternative addresses the element has.
 *
 * @ingroup ManyAddressesMIDIOutputElements
 */
template <setting_t N>
class CCPotentiometer
    : public Bankable::MIDIFilteredAnalogAddressable<ManyAddresses<N>,
                                                     ContinuousCCSender> {
  public:
    /** 
     * @brief   Create a new CCPotentiometer object with the given 
     *          analog pin, and address list.
     * 
     * @param   bank
     *          The bank that selects the address to use.
     * @param   analogPin
     *          The analog input pin to read from.
     * @param   addresses
     *          The list of MIDI addresses containing the controller number 
     *          [0, 119], channel [CHANNEL_1, CHANNEL_16], and optional cable 
     *          number [0, 15].
     * @param   sender
     *          The MIDI sender to use.
     * 
     * @ingroup MIDIOutputElementConstructors
     */
    CCPotentiometer(const Bank<N> &bank, pin_t analogPin,
                    const Array<MIDICNChannelAddress, N> &addresses,
                    const ContinuousCCSender &sender = {})
        : MIDIFilteredAnalogAddressable<ManyAddresses<N>, ContinuousCCSender>{
              {bank, addresses}, analogPin, sender} {}
};

} // namespace ManyAddresses
} // namespace Bankable

END_CS_NAMESPACE