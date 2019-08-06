/* ✔ */

#pragma once

#include <MIDI_Outputs/Bankable/Abstract/MIDIButtonLatched.hpp>
#include <MIDI_Senders/DigitalCCSender.hpp>

namespace Bankable {

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a **momentary
 *          push button**, and send out MIDI **Controller Change** events.
 * 
 *          It latches the input, so press once to enable, press again to 
 *          disable (toggle).
 * 
 *          The switch is debounced in software.  
 *          This version can be banked.  
 *
 * @ingroup MIDIOutputElements
 */
class CCButtonLatched : public MIDIButtonLatched<DigitalCCSender> {
  public:
    /**
     * @brief   Create a new bankable CCButtonLatched object on the given pin 
     *          and with address.
     * 
     * @param   config
     *          The bank to add this element to.
     * @param   pin
     *          The digital input pin to read from.  
     *          The internal pull-up resistor will be enabled.
     * @param   address
     *          The MIDI address containing the controller number [0, 119], 
     *          channel [1, 16], and optional cable number.
     * 
     * @ingroup MIDIOutputElementConstructors
     */
    CCButtonLatched(const OutputBankConfig &config, pin_t pin,
                    const MIDICNChannelAddress &address,
                    const DigitalCCSender &sender = {})
        : MIDIButtonLatched{config, pin, address, sender} {}
};

} // namespace Bankable