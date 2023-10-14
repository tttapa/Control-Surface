#pragma once

#include <Banks/BankAddresses.hpp>
#include <MIDI_Outputs/Bankable/Abstract/MIDIFilteredAnalog.hpp>
#include <MIDI_Senders/ContinuousCCSender.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {

/**
 * @brief   A class of MIDIOutputElement%s that read the analog input from a
 *          **potentiometer or fader**, and send out 7-bit MIDI **Control
 *          Change** events.
 * 
 * The analog input is filtered and hysteresis is applied for maximum
 * stability.  
 * This version can be banked.
 *
 * @ingroup BankableMIDIOutputElements
 */
class CCPotentiometer
    : public MIDIFilteredAnalog<SingleAddress, ContinuousCCSender> {
  public:
    /** 
     * @brief   Create a new Bankable CCPotentiometer object with the given 
     *          analog pin, controller number and channel.
     * 
     * @param   config
     *          The bank configuration to use: the bank to add this element to,
     *          and whether to change the address, channel or cable number.
     * @param   analogPin
     *          The analog input pin to read from.
     * @param   address
     *          The MIDI address containing the controller number [0, 119], 
     *          channel [Channel_1, Channel_16], and optional cable number 
     *          [Cable_1, Cable_16].
     */
    CCPotentiometer(OutputBankConfig<> config, pin_t analogPin,
                    MIDIAddress address)
        : MIDIFilteredAnalog(SingleAddress {config, address}, analogPin, {}) {}
};

} // namespace Bankable

END_CS_NAMESPACE