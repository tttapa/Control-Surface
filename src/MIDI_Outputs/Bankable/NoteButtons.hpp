#pragma once

#include <Banks/BankAddresses.hpp>
#include <MIDI_Outputs/Bankable/Abstract/MIDIButtons.hpp>
#include <MIDI_Senders/DigitalNoteSender.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a **collection
 *          of momentary push buttons or switches**, and send out MIDI **Note** 
 *          events.
 * 
 * A Note On event is sent when a button is pressed, and a Note Off
 * event is sent when a button is released.  
 * The buttons are debounced in software.  
 * This version can be banked.
 * 
 * @tparam  NumButtons
 *          The number of buttons in the collection.
 *
 * @ingroup BankableMIDIOutputElements
 */
template <uint8_t NumButtons>
class NoteButtons
    : public MIDIButtons<SingleAddress, DigitalNoteSender, NumButtons> {
  public:
    /**
     * @brief   Create a new Bankable NoteButtons object with the given pins,
     *          the given controller number and channel.
     *
     * @param   config
     *          The bank configuration to use: the bank to add this element to,
     *          and whether to change the address, channel or cable number.
     * @param   buttons
     *          A list of digital input pins with the buttons connected.  
     *          The internal pull-up resistors will be enabled.
     * @param   baseAddress
     *          The MIDI address of the first button, containing the note
     *          number [0, 127], channel [Channel_1, Channel_16], and optional 
     *          cable number [Cable_1, Cable_16].
     * @param   incrementAddress
     *          The number of addresses to increment for each next button.  
     *          E.g. if `baseAddress` is 8, and `incrementAddress` is 2,
     *          then the first button will send on address 8, the second
     *          button will send on address 10, button three on address 12, etc.
     * @param   velocity
     *          The velocity of the MIDI Note events.
     */
    NoteButtons(OutputBankConfig<> config,
                const Array<AH::Button, NumButtons> &buttons,
                MIDIAddress baseAddress, RelativeMIDIAddress incrementAddress,
                uint8_t velocity = 0x7F)
        : MIDIButtons<SingleAddress, DigitalNoteSender, NumButtons> {
              {config, baseAddress},
              buttons,
              incrementAddress,
              {velocity},
          } {}

    /// Set the velocity of the MIDI Note events.
    void setVelocity(uint8_t velocity) { this->sender.setVelocity(velocity); }
    /// Get the velocity of the MIDI Note events.
    uint8_t getVelocity() const { return this->sender.getVelocity(); }
};

} // namespace Bankable

END_CS_NAMESPACE