#pragma once

#include <Banks/BankAddresses.hpp>
#include <MIDI_Outputs/Bankable/Abstract/MIDIButtonLatching.hpp>
#include <MIDI_Senders/DigitalNoteSender.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a **latching
 *          push button or toggle switch**, and send out MIDI **Note** events.
 * 
 * When the switch changes state, two MIDI Note events are sent: first
 * a Note On event, followed immediately by a Note Off event.  
 * The switch is debounced in software.  
 * This version can be banked.  
 *
 * @ingroup BankableMIDIOutputElements
 */
class NoteButtonLatching
    : public MIDIButtonLatching<SingleAddress, DigitalNoteSender> {
  public:
    /**
     * @brief   Create a new Bankable NoteButtonLatching object with the given 
     *          pin, note number and channel.
     * 
     * @param   config
     *          The bank configuration to use: the bank to add this element to,
     *          and whether to change the address, channel or cable number.
     * @param   pin
     *          The digital input pin to read from.  
     *          The internal pull-up resistor will be enabled.
     * @param   address
     *          The MIDI address containing the note number [0, 127], 
     *          channel [CHANNEL_1, CHANNEL_16], and optional cable number 
     *          [CABLE_1, CABLE_16].
     * @param   velocity
     *          The velocity of the MIDI Note events.
     */
    NoteButtonLatching(OutputBankConfig<> config, pin_t pin,
                       MIDIAddress address, uint8_t velocity = 0x7F)
        : MIDIButtonLatching<SingleAddress, DigitalNoteSender>{
              {config, address},
              pin,
              {velocity},
          } {}

    /// Set the velocity of the MIDI Note events.
    void setVelocity(uint8_t velocity) { this->sender.setVelocity(velocity); }
    /// Get the velocity of the MIDI Note events.
    uint8_t getVelocity() const { return this->sender.getVelocity(); }
};

} // namespace Bankable

END_CS_NAMESPACE