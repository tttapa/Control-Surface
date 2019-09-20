#pragma once

#include <MIDI_Outputs/Bankable/Abstract/MIDIFilteredAnalog.hpp>
#include <MIDI_Senders/PitchBendSender.hpp>

namespace Bankable {

/**
 * @brief   A class of MIDIOutputElement%s that read the analog input from a
 *          **potentiometer or fader**, and send out 14-bit MIDI **Pitch Bend** 
 *          events.
 * 
 * The analog input is filtered and hysteresis is applied for maximum
 * stability.  
 * The actual precision is "only" 8 bits, because the built-in ADC
 * is pretty noisy.  
 * This version can be banked.
 *
 * @ingroup BankableMIDIOutputElements
 */
class PBPotentiometer : public MIDIFilteredAnalog<PitchBendSender<10>, 10> {
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
     *          Number [0, 15].
     * @param   sender
     *          The MIDI sender to use.
     */
    PBPotentiometer(const OutputBankConfig &bank, pin_t analogPin,
                    const MIDICNChannel &address,
                    const PitchBendSender<10> &sender = {})
        : MIDIFilteredAnalog{bank, analogPin, address, sender} {}
};

} // namespace Bankable