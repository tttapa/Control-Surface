#include "Abstract/MIDIFilteredAnalog.hpp"
#include <MIDI_Senders/ContinuousCCSender.hpp>

namespace ManyAddresses {

/**
 * @brief   A class of MIDIOutputElement%s that read the analog input from a
 *          **potentiometer or fader**, and send out 7-bit MIDI **Control
 *          Change** events.
 * 
 *          The analog input is filtered and hysteresis is applied for maximum
 *          stability.  
 *          This version can be banked.
 *          
 * @tparam  N
 *          The number of variants/alternative addresses the element has.
 *
 * @ingroup MIDIOutputElements
 */
template <setting_t N>
class CCPotentiometer
    : public MIDIFilteredAnalogAddressable<N, ContinuousCCSender::send, 7> {
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
     *          The list of addresses.
     * 
     * @ingroup MIDIOutputElementConstructors
     */
    CCPotentiometer(const Bank<N> &bank, pin_t analogPin,
                    const Array<MIDICNChannelAddress, N> &addresses)
        : MIDIFilteredAnalogAddressable<N, ContinuousCCSender::send, 7>(
              bank, analogPin, addresses) {}
};

} // namespace ManyAddresses