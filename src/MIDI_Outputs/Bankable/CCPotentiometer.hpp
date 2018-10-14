#pragma once

#include <MIDI_Outputs/Bankable/Abstract/MIDIFilteredAnalog.hpp>
#include <MIDI_Senders/ContinuousCCSender.hpp>

namespace Bankable {

/**
 * @brief   A class of MIDIOutputElement%s that read the analog input from a
 *          **potentiometer or fader**, and send out 7-bit MIDI **Control
 *          Change** events.
 * 
 *          The analog input is filtered and hysteresis is applied for maximum
 *          stability.  
 *          This version can be banked.
 *
 * @ingroup MIDIOutputElements
 */
class CCPotentiometer
    : public MIDIFilteredAnalogAddressable<ContinuousCCSender::send, 7> {
  public:
    /** 
     * @brief   Create a new Bankable CCPotentiometer object with the given 
     *          analog pin, controller number and channel.
     * 
     * @param   analogPin
     *          The analog input pin to read from.
     * @param   controller
     *          The MIDI Controller number. [0, 119]
     * @param   channel
     *          The MIDI channel. [1, 16]
     * 
     * @ingroup MIDIOutputElementConstructors
     */
    CCPotentiometer(const OutputBankConfig &config, pin_t analogPin,
                    const MIDICNChannelAddress &address)
        : MIDIFilteredAnalogAddressable<ContinuousCCSender::send, 7>(
              config, analogPin, address) {}
};

} // namespace Bankable