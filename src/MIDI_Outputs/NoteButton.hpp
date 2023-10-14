#pragma once

#include <MIDI_Outputs/Abstract/MIDIButton.hpp>
#include <MIDI_Senders/DigitalNoteSender.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a **momentary
 *          push button or switch**, and send out MIDI **Note** events.
 * 
 * A Note On event is sent when the button is pressed, and a Note Off
 * event is sent when the button is released.  
 * The button is debounced in software.  
 * This version cannot be banked.  
 *
 * @ingroup MIDIOutputElements
 */
class NoteButton : public MIDIButton<DigitalNoteSender> {
  public:
    /**
     * @brief   Create a new NoteButton object with the given pin, note number
     *          and channel.
     * 
     * @param   pin
     *          The digital input pin to read from.  
     *          The internal pull-up resistor will be enabled.
     * @param   address
     *          The MIDI address containing the note number [0, 127], 
     *          channel [Channel_1, Channel_16], and optional cable number 
     *          [Cable_1, Cable_16].
     * @param   velocity
     *          The velocity of the MIDI Note events.
     */
    NoteButton(pin_t pin, MIDIAddress address, uint8_t velocity = 0x7F)
        : MIDIButton {
              pin,
              address,
              {velocity},
          } {}

    /// Set the velocity of the MIDI Note events.
    void setVelocity(uint8_t velocity) { this->sender.setVelocity(velocity); }
    /// Get the velocity of the MIDI Note events.
    uint8_t getVelocity() const { return this->sender.getVelocity(); }
};

END_CS_NAMESPACE