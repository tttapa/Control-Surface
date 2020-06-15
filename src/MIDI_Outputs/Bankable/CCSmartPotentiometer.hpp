#pragma once

#include <Banks/BankAddresses.hpp>
#include <MIDI_Outputs/Bankable/Abstract/SmartMIDIFilteredAnalog.hpp>
#include <MIDI_Senders/ContinuousCCSender.hpp>

AH_DIAGNOSTIC_WERROR()

BEGIN_CS_NAMESPACE

namespace Bankable {

/**
 * @brief   A class of MIDIOutputElement%s that read the analog input from a
 *          **potentiometer or fader**, and send out 7-bit MIDI **Control
 *          Change** events.
 * 
 * The analog input is filtered and hysteresis is applied for maximum
 * stability.  
 * 
 * This version can be banked, and when changing banks, it disables the 
 * potentiometer until you move it to the position where it was last time the
 * current bank was active. This prevents the values changing when changing 
 * banks.
 *
 * @ingroup BankableMIDIOutputElements
 * 
 * @tparam  NumBanks
 *          The number of banks.
 */
template <uint8_t NumBanks>
class CCSmartPotentiometer
    : public SmartMIDIFilteredAnalog<NumBanks, SingleAddress,
                                     ContinuousCCSender> {
  public:
    /** 
     * @brief   Create a new Bankable CCSmartPotentiometer object with the given
     *          analog pin, controller number and channel.
     * 
     * @param   config
     *          The bank configuration to use: the bank to add this element to,
     *          and whether to change the address, channel or cable number.
     * @param   analogPin
     *          The analog input pin to read from.
     * @param   address
     *          The MIDI address containing the controller number [0, 119], 
     *          channel [CHANNEL_1, CHANNEL_16], and optional cable number 
     *          [CABLE_1, CABLE_16].
     */
    CCSmartPotentiometer(BankConfig<NumBanks> config, pin_t analogPin,
                         MIDIAddress address)
        : SmartMIDIFilteredAnalog<NumBanks, SingleAddress, ContinuousCCSender>(
              SingleAddress{config, address}, analogPin, {}) {}
};

} // namespace Bankable

END_CS_NAMESPACE

AH_DIAGNOSTIC_POP()
