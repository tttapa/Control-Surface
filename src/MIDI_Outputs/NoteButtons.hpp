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
 * @tparam  NUMBER_OF_BUTTONS
 *          The number of buttons in the collection.
 * 
 * @ingroup MIDIOutputElements
 */
template <uint8_t NUMBER_OF_BUTTONS>
class NoteButtons : public MIDIButtons<DigitalNoteSender, NUMBER_OF_BUTTONS> {
  public:
    /**
     * @brief   Create a new NoteButtons object with the given pins,
     *          the given controller number and channel.
     *
     * @param   buttons
     *          An list of digital input pins with the buttons connected.  
     *          The internal pull-up resistors will be enabled.
     * @param   baseAddress
     *          The MIDI address of the first button, containing the note
     *          number [0, 127], channel [CHANNEL_1, CHANNEL_16], and optional 
     *          cable number [0, 15].
     * @param   incrementAddress
     *          The number of addresses to increment for each next button.  
     *          E.g. if `baseAddress` is 8, and `incrementAddress` is 2,
     *          then the first button will send on address 8, the second
     *          button will send on address 10, button three on address 12, etc.
     * @param   sender
     *          The MIDI sender to use.
     */
    NoteButtons(const Array<Button, NUMBER_OF_BUTTONS> &buttons,
                const MIDICNChannelAddress &baseAddress,
                const RelativeMIDICNChannelAddress &incrementAddress,
                const DigitalNoteSender &sender = {})
        : MIDIButtons<DigitalNoteSender, NUMBER_OF_BUTTONS>(
              buttons, baseAddress, incrementAddress, sender) {}
};

END_CS_NAMESPACE