#include <MIDI_Outputs/Bankable/Abstract/MIDIButtons.hpp>
#include <MIDI_Senders/DigitalCCSender.hpp>

namespace Bankable {

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a collection
 *          of momentary push buttons or switches, and send out MIDI Control
 *          Change events.
 * 
 *          A value of 0x7F is sent when a button is pressed, and a value of
 *          0x00 is sent when a button is released.  
 *          The buttons are debounced in software.  
 *          This version can be banked.  
 *
 * @ingroup MIDIOutputElements
 *
 * @tparam  NUMBER_OF_BUTTONS
 *          The number of buttons in the collection.
 */
template <uint8_t NUMBER_OF_BUTTONS>
class CCButtons : public MIDIButtons<NUMBER_OF_BUTTONS, DigitalCCSender> {
  public:
    /**
     * @brief   Create a new Bankable CCButtons object with the given pins,
     *          the given controller number and channel.
     *
     * @param   buttons
     *          An list of digital input pins with the buttons connected.  
     *          The internal pull-up resistors will be enabled.
     * @param   baseAddress
     *          The MIDI Controller number of the first button. [0, 119]
     * @param   baseChannel
     *          The MIDI Channel of the first button. [1, 16]
     * @param   addressIncrement
     *          The number of addresses to increment for each next button.  
     *          E.g. if `baseAddress` is 8, and `addressIncrement` is 2,
     *          then the first button will send on address 8, the second
     *          button will send on address 10, button three on address 12, etc.
     * @param   channelIncrement
     *          The number of channels to increment for each next button.
     */
    CCButtons(const BankConfigAddressable &config,
              const Array<Button, NUMBER_OF_BUTTONS> &buttons,
              uint8_t baseAddress, uint8_t baseChannel = 1,
              uint8_t addressIncrement = 1, uint8_t channelIncrement = 0)
        : MIDIButtons<NUMBER_OF_BUTTONS, DigitalCCSender>(
              config, buttons, baseAddress, baseChannel, addressIncrement,
              channelIncrement) {}
};

} // namespace Bankable