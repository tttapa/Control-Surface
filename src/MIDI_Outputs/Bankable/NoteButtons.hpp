#pragma once

#include <MIDI_Outputs/Bankable/Abstract/MIDIButtons.hpp>
#include <MIDI_Senders/DigitalNoteSender.hpp>

namespace Bankable {

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a **collection 
 *          of momentary push buttons or switches**, and send out MIDI **Note** 
 *          events.
 * 
 *          A Note On event is sent when a button is pressed, and a Note Off
 *          event is sent when a button is released.  
 *          The buttons are debounced in software.  
 *          This version can be banked.
 *
 * @ingroup MIDIOutputElements
 * 
 * @tparam  NUMBER_OF_BUTTONS
 *          The number of buttons in the collection.
 */
template <uint8_t NUMBER_OF_BUTTONS>
class NoteButtons
    : public MIDIButtons<DigitalNoteSender::sendOn, DigitalNoteSender::sendOff,
                         NUMBER_OF_BUTTONS> {
  public:
    /**
     * @brief   Create a new Bankable NoteButtons object with the given pins,
     *          the given controller number and channel.
     *
     * @param   buttons
     *          An list of digital input pins with the buttons connected.  
     *          The internal pull-up resistors will be enabled.
     * @param   baseNote
     *          The MIDI Note number of the first button. [0, 127]
     * @param   baseChannel
     *          The MIDI Channel of the first button. [1, 16]
     * @param   noteIncrement
     *          The number of notes to increment for each next button.  
     *          E.g. if `baseNote` is 8, and `noteIncrement` is 2,
     *          then the first button will send on note 8, the second
     *          button will send on note 10, button three on note 12, etc.
     * @param   channelIncrement
     *          The number of channels to increment for each next button.
     */
    NoteButtons(const OutputBankConfigAddressable &config,
                const Array<Button, NUMBER_OF_BUTTONS> &buttons,
                uint8_t baseNote, uint8_t baseChannel = 1,
                uint8_t noteIncrement = 1, uint8_t channelIncrement = 0)
        : MIDIButtons<DigitalNoteSender::sendOn, DigitalNoteSender::sendOff,
                      NUMBER_OF_BUTTONS>(config, buttons, baseNote, baseChannel,
                                         noteIncrement, channelIncrement) {}
};

} // namespace Bankable