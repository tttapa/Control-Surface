#pragma once

#include <AH/Settings/Warnings.hpp>
AH_DIAGNOSTIC_WERROR()

#include <Banks/BankAddresses.hpp>
#include <MIDI_Outputs/Bankable/Abstract/MIDIFilteredAnalog.hpp>
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
 * This version can be banked.
 *
 * @ingroup BankableMIDIOutputElements
 */
class PBPotentiometer
    : public MIDIFilteredAnalog<SingleAddress, PitchBendSender<10>> {
  public:
    /** 
     * @brief   Create a new Bankable PBPotentiometer object with the given 
     *          analog pin and channel.
     * 
     * @param   config
     *          The bank configuration to use: the bank to add this element to,
     *          and whether to change the address, channel or cable number.
     * @param   analogPin
     *          The analog input pin to read from.
     * @param   address
     *          The MIDI channel [CHANNEL_1, CHANNEL_16] and optional Cable
     *          Number [CABLE_1, CABLE_16].
     */
    PBPotentiometer(OutputBankConfig<BankType::CHANGE_CHANNEL> config,
                    pin_t analogPin, MIDIChannelCable address)
        : MIDIFilteredAnalog{{config, address}, analogPin, {}} {}
};

} // namespace Bankable

END_CS_NAMESPACE

AH_DIAGNOSTIC_POP()
