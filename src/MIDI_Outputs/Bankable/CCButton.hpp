#pragma once

#include <Banks/BankAddresses.hpp>
#include <MIDI_Outputs/Bankable/Abstract/MIDIButton.hpp>
#include <MIDI_Senders/DigitalCCSender.hpp>

BEGIN_CS_NAMESPACE

/** @brief  A namespace for MIDI elements that can be added to a Bank,
 *          to change their address or channel.
 */
namespace Bankable {

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a **momentary
 *          push button or switch**, and send out MIDI **Control Change**
 *          events.
 * 
 * A value of 0x7F is sent when the button is pressed, and a value of
 * 0x00 is sent when the button is released.  
 * The button is debounced in software.  
 * This version can be banked.
 *
 * @ingroup BankableMIDIOutputElements
 */
class CCButton : public MIDIButton<SingleAddress, DigitalCCSender> {
  public:
    /**
     * @brief   Create a new Bankable CCButton object with the given bank 
     *          configuration, button pin, and address.
     *
     * @param   config
     *          The bank configuration to use: the bank to add this element to,
     *          and whether to change the address, channel or cable number.
     * @param   pin
     *          The digital input pin with the button connected.  
     *          The internal pull-up resistor will be enabled.
     * @param   address
     *          The MIDI address containing the controller number [0, 119], 
     *          channel [CHANNEL_1, CHANNEL_16], and optional cable number 
     *          [CABLE_1, CABLE_16].
     * @param   sender
     *          The MIDI sender to use.
     */
    CCButton(OutputBankConfig<> config, pin_t pin, MIDIAddress address,
             const DigitalCCSender &sender = {})
        : MIDIButton{{config, address}, pin, sender} {}
};

} // namespace Bankable

END_CS_NAMESPACE