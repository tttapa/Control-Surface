#pragma once

#include <MIDI_Outputs/Abstract/MIDIButtons.hpp>
#include <MIDI_Senders/DigitalNoteSender.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a **collection
 *          of momentary push buttons or switches**, and send out MIDI **Note** 
 *          events.
 * 
 * A Note On event is sent when a button is pressed, and a Note Off event is
 * sent when a button is released.  
 * The buttons are debounced in software.  
 * This version cannot be banked.
 * 
 * @tparam  NumButtons
 *          The number of buttons in the collection.
 * 
 * @ingroup MIDIOutputElements
 */
template <uint8_t NumButtons>
class NoteButtons : public MIDIButtons<DigitalNoteSender, NumButtons> {
  public:
    /**
     * @brief   Create a new NoteButtons object with the given pins,
     *          the given controller number and channel.
     *
     * @param   buttons
     *          A list of digital input pins with the buttons connected.  
     *          The internal pull-up resistors will be enabled.
     * @param   baseAddress
     *          The MIDI address of the first button, containing the note
     *          number [0, 127], channel [CHANNEL_1, CHANNEL_16], and optional 
     *          cable number [CABLE_1, CABLE_16].
     * @param   incrementAddress
     *          The number of addresses to increment for each next button.  
     *          E.g. if `baseAddress` is 8, and `incrementAddress` is 2,
     *          then the first button will send on address 8, the second
     *          button will send on address 10, button three on address 12, etc.
     * @param   velocity
     *          The velocity of the MIDI Note events.
     */
    NoteButtons(const Array<AH::Button, NumButtons> &buttons,
                MIDIAddress baseAddress, RelativeMIDIAddress incrementAddress,
                uint8_t velocity = 0x7F)
        : MIDIButtons<DigitalNoteSender, NumButtons>{
              buttons,
              baseAddress,
              incrementAddress,
              {velocity},
          } {}

    /// Set the velocity of the MIDI Note events.
    void setVelocity(uint8_t velocity) { this->sender.setVelocity(velocity); }
    /// Get the velocity of the MIDI Note events.
    uint8_t getVelocity() const { return this->sender.getVelocity(); }
};

END_CS_NAMESPACE