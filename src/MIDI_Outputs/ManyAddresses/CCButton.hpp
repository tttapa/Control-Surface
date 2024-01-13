#pragma once

#include <Banks/BankAddresses.hpp>
#include <MIDI_Outputs/Bankable/Abstract/MIDIButton.hpp>
#include <MIDI_Senders/DigitalCCSender.hpp>

BEGIN_CS_NAMESPACE
namespace Bankable {
namespace ManyAddresses {

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a **momentary
 *          push button or switch**, and send out MIDI **Control Change**
 *          events.
 * 
 * A value of 0x7F is sent when the button is pressed, and a value of
 * 0x00 is sent when the button is released.  
 * The button is debounced in software.  
 * This version can be banked using an arbitrary list of alternative
 * addresses.
 *          
 * @tparam  NumBanks
 *          The number of variants/alternative addresses the element has.
 *
 * @ingroup ManyAddressesMIDIOutputElements
 */
template <setting_t NumBanks>
class CCButton
    : public Bankable::MIDIButton<ManyAddresses<NumBanks>, DigitalCCSender> {
  public:
    /**
     * @brief   Create a new Bankable CCButton object with the given bank 
     *          configuration, button pin, and address.
     *
     * @param   bank
     *          The bank that selects the address to use.
     * @param   pin
     *          The digital input pin with the button connected.  
     *          The internal pull-up resistor will be enabled.
     * @param   addresses
     *          The list of MIDI addresses containing the controller number 
     *          [0, 119], channel [Channel_1, Channel_16], and optional cable 
     *          number [Cable_1, Cable_16].
     * @param   sender
     *          The MIDI sender to use.
     */
    CCButton(const Bank<NumBanks> &bank, pin_t pin,
             const Array<MIDIAddress, NumBanks> &addresses,
             const DigitalCCSender &sender = {})
        : MIDIButton<ManyAddresses<NumBanks>, DigitalCCSender> {
              {bank, addresses}, pin, sender} {}
};

} // namespace ManyAddresses
} // namespace Bankable

END_CS_NAMESPACE