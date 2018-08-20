#pragma once

#include <MIDI_Outputs/Abstract/MIDIFilteredAnalog.hpp>
#include <MIDI_Senders/ContinuousCCSender.hpp>

/**
 * @brief   A class of MIDIOutputElement%s that read the analog input from a
 *          **potentiometer or fader**, and send out 7-bit MIDI 
 *          **Control Change** events.
 * 
 *          The analog input is filtered and hysteresis is applied for maximum
 *          stability.  
 *          This version cannot be banked.
 *
 * @ingroup MIDIOutputElements
 */
class CCPotentiometer
    : public MIDIFilteredAnalogAddressable<ContinuousCCSender::send, 7> {
  public:
    /** 
     * @brief   Create a new CCPotentiometer object with the given analog pin, 
     *          controller number and channel.
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
    CCPotentiometer(pin_t analogPin, uint8_t controller, uint8_t channel = 1)
        : MIDIFilteredAnalogAddressable<ContinuousCCSender::send, 7>(
              analogPin, controller, channel) {}
};