/* ✔ */

#pragma once

#include <Banks/BankAddresses.hpp>
#include <MIDI_Outputs/Bankable/Abstract/MIDIButtonLatched.hpp>
#include <MIDI_Senders/DigitalCCSender.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a **momentary
 *          push button**, and send out MIDI **Controller Change** events.
 * 
 * It latches the input, so press once to enable, press again to 
 * disable (toggle).
 * 
 * The switch is debounced in software.  
 * This version can be banked.  
 *
 * @ingroup BankableMIDIOutputElements
 */
class CCButtonLatched
    : public MIDIButtonLatched<SingleAddress, DigitalCCSender> {
  public:
    /**
     * @brief   Create a new bankable CCButtonLatched object on the given pin 
     *          and with address.
     * 
     * @param   config
     *          The bank configuration to use: the bank to add this element to,
     *          and whether to change the address, channel or cable number.
     * @param   pin
     *          The digital input pin to read from.  
     *          The internal pull-up resistor will be enabled.
     * @param   address
     *          The MIDI address containing the controller number [0, 119], 
     *          channel [CHANNEL_1, CHANNEL_16], and optional cable number 
     *          [0, 15].
     * @param   sender
     *          The MIDI sender to use.
     */
    CCButtonLatched(const OutputBankConfig &config, pin_t pin,
                    const MIDICNChannelAddress &address,
                    const DigitalCCSender &sender = {})
        : MIDIButtonLatched{{config, address}, pin, sender} {}
};

} // namespace Bankable

END_CS_NAMESPACE