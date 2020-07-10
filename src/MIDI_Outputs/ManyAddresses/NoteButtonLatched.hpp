#pragma once

#include <Banks/BankAddresses.hpp>
#include <MIDI_Outputs/Bankable/Abstract/MIDIButtonLatched.hpp>
#include <MIDI_Senders/DigitalNoteSender.hpp>

BEGIN_CS_NAMESPACE
namespace Bankable {
namespace ManyAddresses {

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a **momentary
 *          push button or switch**, and send out MIDI **Note** event.
 *
 * A Note On event is sent when the button is pressed
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
class NoteButtonLatched
    : public Bankable::MIDIButtonLatched<ManyAddresses<NumBanks>, SingleAddress, DigitalNoteSender> {
  public:
    /**
     * @brief   Create a new Bankable NoteButtonLatched object with the given bank
     *          configuration, button pin, and address.
     *
     * @param   bank
     *          The bank that selects the address to use.
     * @param   pin
     *          The digital input pin with the button connected.
     *          The internal pull-up resistor will be enabled.
     * @param   addresses
     *          The list of MIDI addresses containing the note number
     *          [0, 127], channel [CHANNEL_1, CHANNEL_16], and optional cable
     *          number [0, 15].
     * @param   velocity
     *          The velocity of the MIDI Note events.
     *
     * @ingroup MIDIOutputElementConstructors
     */
    NoteButtonLatched(const Bank<NumBanks> &bank, pin_t pin,
               const Array<MIDIAddress, NumBanks> &addresses,
               uint8_t velocity = 0x7F)
        : MIDIButtonLatched<ManyAddresses<NumBanks>, SingleAddress, DigitalNoteSender>{
              {bank, addresses}, pin, {velocity}} {}
};

} // namespace ManyAddresses
} // namespace Bankable

END_CS_NAMESPACE
