#pragma once

#include <MIDI_Outputs/Abstract/MIDIButtons.hpp>
#include <MIDI_Senders/DigitalCCSender.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a **collection
 *          of momentary push buttons or switches**, and send out MIDI **Control
 *          Change** events.
 * 
 * A value of 0x7F is sent when a button is pressed, and a value of 0x00 is sent
 * when a button is released.  
 * The buttons are debounced in software.  
 * This version cannot be banked.  
 *
 * @tparam  NumButtons
 *          The number of buttons in the collection.
 *
 * @ingroup MIDIOutputElements
 */
template <uint8_t NumButtons>
class CCButtons : public MIDIButtons<DigitalCCSender, NumButtons> {
  public:
    /**
     * @brief   Create a new CCButtons object with the given pins,
     *          the given controller number and channel.
     *
     * @param   buttons
     *          A list of digital input pins with the buttons connected.  
     *          The internal pull-up resistors will be enabled.
     * @param   baseAddress
     *          The MIDI address of the first button, containing the controller
     *          number [0, 119], channel [CHANNEL_1, CHANNEL_16], and optional 
     *          cable number [CABLE_1, CABLE_16].
     * @param   incrementAddress
     *          The number of addresses to increment for each next button.  
     *          E.g. if `baseAddress` is 8, and `incrementAddress` is 2,
     *          then the first button will send on address 8, the second
     *          button will send on address 10, button three on address 12, etc.
     * @param   sender
     *          The MIDI sender to use.
     */
    CCButtons(const Array<AH::Button, NumButtons> &buttons,
              MIDIAddress baseAddress, RelativeMIDIAddress incrementAddress,
              const DigitalCCSender &sender = {})
        : MIDIButtons<DigitalCCSender, NumButtons>(buttons, baseAddress,
                                                   incrementAddress, sender) {}
};

END_CS_NAMESPACE