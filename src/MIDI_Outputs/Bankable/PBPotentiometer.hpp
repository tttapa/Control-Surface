#pragma once

#include <MIDI_Outputs/Bankable/Abstract/MIDIFilteredAnalog.hpp>
#include <MIDI_Senders/PitchBendSender.hpp>

namespace Bankable {

/**
 * @brief   A class of MIDIOutputElement%s that read the analog input from a
 *          **potentiometer or fader**, and send out 14-bit MIDI **Pitch Bend** 
 *          events.
 * 
 *          The analog input is filtered and hysteresis is applied for maximum
 *          stability.  
 *          The actual precision is "only" 8 bits, because the built-in ADC
 *          is pretty noisy.  
 *          This version can be banked.
 *
 * @ingroup MIDIOutputElements
 */
class PBPotentiometer : public MIDIFilteredAnalog<PitchBendSender<8>, 8> {
  public:
    /** 
     * @brief   Create a new Bankable PBPotentiometer object with the given 
     *          analog pin and channel.
     * 
     * @param   analogPin
     *          The analog input pin to read from.
     * @param   channel
     *          The MIDI channel. [1, 16]
     * 
     * @ingroup MIDIOutputElementConstructors
     */
    PBPotentiometer(const OutputBankConfig &bank, pin_t analogPin,
                    const MIDICNChannel &address,
                    const PitchBendSender<8> &sender = {})
        : MIDIFilteredAnalog{bank, analogPin, address, sender} {}
};

} // namespace Bankable