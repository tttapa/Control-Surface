#pragma once

#include <MIDI_Outputs/Abstract/MIDIIncrementDecrementButtons.hpp>
#include <MIDI_Senders/RelativeCCSender.hpp>

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a two
 *          momentary push buttons to increment or decrement a relative setting 
 *          using relative MIDI Control Change events.  
 *          An optional note number can be provided to reset the setting.
 * 
 *          This version cannot be banked.
 *
 * @ingroup MIDIOutputElements
 */
class CCIncrementDecrementButtons
    : public MIDIIncrementDecrementButtons<RelativeCCSender::send> {
  public:
    /**
     * @brief   
     * @todo    Documentation
     */
    CCIncrementDecrementButtons(const IncrementDecrementButtons &buttons,
                                uint8_t address, uint8_t channel = 1,
                                uint8_t speedMultiply = 1,
                                uint8_t resetAddress = NO_ADDRESS)
        : MIDIIncrementDecrementButtons(buttons, address, channel,
                                        speedMultiply, resetAddress) {}
};