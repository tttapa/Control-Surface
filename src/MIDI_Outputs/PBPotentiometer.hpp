#pragma once

#include <AH/Settings/Warnings.hpp>
AH_DIAGNOSTIC_WERROR()

#include <MIDI_Outputs/Abstract/MIDIFilteredAnalog.hpp>
#include <MIDI_Senders/PitchBendSender.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   A class of MIDIOutputElement%s that read the analog input from a
 *          **potentiometer or fader**, and send out 14-bit MIDI **Pitch Bend** 
 *          events.
 * 
 * The analog input is filtered and hysteresis is applied for maximum
 * stability.  
 * The actual precision is "only" 8 bits, because the built-in ADC
 * is pretty noisy.  
 * This version cannot be banked.
 *
 * @ingroup MIDIOutputElements
 */
class PBPotentiometer : public MIDIFilteredAnalog<PitchBendSender<10>> {
  public:
    /** 
     * @brief   Create a new PBPotentiometer object with the given analog pin
     *          and channel.
     * 
     * @param   analogPin
     *          The analog input pin to read from.
     * @param   address
     *          The MIDI channel [CHANNEL_1, CHANNEL_16] and optional Cable
     *          Number [0, 15].
     * @param   sender
     *          The MIDI sender to use.
     */
    PBPotentiometer(pin_t analogPin, const MIDICNChannel &address = CHANNEL_1,
                    const PitchBendSender<10> &sender = {})
        : MIDIFilteredAnalog(analogPin, address, sender) {}
};

END_CS_NAMESPACE

AH_DIAGNOSTIC_POP()