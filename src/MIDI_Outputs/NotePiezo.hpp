#pragma once

#include <MIDI_Outputs/Abstract/MIDIPiezo.hpp>
#include <MIDI_Senders/DigitalNoteSender.hpp>

BEGIN_CS_NAMESPACE

/**
 * @file NotePiezo.hpp
 * @brief   A class of MIDIOutputElement%s that read the input of a
 *          piezo or analog element**, and send out MIDI **Note** events.
 * 
 * A Note On event is sent when the piezo is struck, and a Note Off
 * event is sent after a set amount of time.
 * Example Usage:
 * NotePiezo drumpad = { A0, {43, CHANNEL_10} };
 * drumpad.setHitThreshold(piezo1.sampleSilence(1000)+1);
 * 
 * Put a piezo between ground and A0
 * Put a resistor between ground and A0
 * Put a diode (preferably zener) between ground and A0 with the line facing A0. This drains backwards current from the piezo.
 * 
 * 
 * The piezo is debounced in software.  
 * This version cannot be banked.  
 *
 * @ingroup MIDIOutputElements
 */
class NotePiezo : public MIDIPiezo<DigitalNoteSender> {
  public:
    /**
     * @brief   Create a new NotePiezo object with the given pin, note number
     *          and channel.
     * 
     * @param   pin
     *          The digital input pin to read from.  
     *          The internal pull-up resistor will be enabled.
     * @param   address
     *          The MIDI address containing the note number [0, 127], 
     *          channel [CHANNEL_1, CHANNEL_16], and optional cable number 
     *          [CABLE_1, CABLE_16].
     * @param   velocity
     *          The velocity of the MIDI Note events. (0 - 127)
     */
    NotePiezo(pin_t pin, const MIDIAddress &address,
               uint8_t velocity = 0x7F)
        : MIDIPiezo{
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