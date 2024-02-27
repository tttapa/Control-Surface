#pragma once

#include <Banks/BankAddresses.hpp>
#include <MIDI_Outputs/Bankable/Abstract/SmartMIDIFilteredAnalog.hpp>
#include <MIDI_Senders/PitchBendSender.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {

/**
 * @brief   A class of MIDIOutputElement%s that read the analog input from a
 *          **potentiometer or fader**, and send out 14-bit MIDI **Pitch Bend** 
 *          events.
 * 
 * The analog input is filtered and hysteresis is applied for maximum
 * stability.  
 * The actual precision is "only" 10 bits, because this is the resolution of the
 * built-in ADC, and this is the default resolution used by the Mackie Control
 * Universal protocol.  
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
class PBSmartPotentiometer
    : public SmartMIDIFilteredAnalog<NumBanks, SingleAddress,
                                     PitchBendSender<10>> {
  public:
    /** 
     * @brief   Create a new Bankable PBSmartPotentiometer object with the given 
     *          analog pin and channel.
     * 
     * @param   config
     *          The bank configuration to use: the bank to add this element to,
     *          and whether to the channel or cable number.
     * @param   analogPin
     *          The analog input pin to read from.
     * @param   address
     *          The MIDI address containing the channel [Channel_1, Channel_16],
     *          and optional cable number [Cable_1, Cable_16].
     */
    PBSmartPotentiometer(BankConfig<NumBanks> config, pin_t analogPin,
                         MIDIChannelCable address)
        : SmartMIDIFilteredAnalog<NumBanks, SingleAddress, PitchBendSender<10>>(
              SingleAddress {config, address}, analogPin, {}) {}
};

} // namespace Bankable

END_CS_NAMESPACE
