#include <MIDI_Outputs/Bankable/Abstract/MIDIButtons.hpp>
#include <MIDI_Senders/DigitalCCSender.hpp>

namespace Bankable {

/**
 * @brief   A class of MIDIOutputElement%s that read the input of **a collection
 *          of momentary push buttons or switches**, and send out MIDI **Control
 *          Change** events.
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
class CCButtons
    : public MIDIButtons<DigitalCCSender::sendOn, DigitalCCSender::sendOff,
                         NUMBER_OF_BUTTONS> {
  public:
    /**
     * @brief   Create a new Bankable CCButtons object with the given pins,
     *          the given controller number and channel.
     *
     * @param   buttons
     *          An list of digital input pins with the buttons connected.  
     *          The internal pull-up resistors will be enabled.
     * @param   baseController
     *          The MIDI Controller number of the first button. [0, 119]
     * @param   baseChannel
     *          The MIDI Channel of the first button. [1, 16]
     * @param   controllerIncrement
     *          The number of addresses to increment for each next button.  
     *          E.g. if `baseController` is 8, and `controllerIncrement` is 2,
     *          then the first button will send on address 8, the second
     *          button will send on address 10, button three on address 12, etc.
     * @param   channelIncrement
     *          The number of channels to increment for each next button.
     * 
     * @ingroup MIDIOutputElementConstructors
     */
    CCButtons(const OutputBankConfig &config,
              const Array<Button, NUMBER_OF_BUTTONS> &buttons,
              const MIDICNChannelAddress &baseAddress,
              const MIDICNChannelAddress &incrementAddress)
        : MIDIButtons<DigitalCCSender::sendOn, DigitalCCSender::sendOff,
                      NUMBER_OF_BUTTONS>(config, buttons, baseAddress,
                                         incrementAddress) {}
};

} // namespace Bankable