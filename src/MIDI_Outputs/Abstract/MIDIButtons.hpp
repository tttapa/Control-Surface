#pragma once

#include <AH/Hardware/Button.hpp>
#include <Def/Def.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   An abstract class for momentary push buttons that send MIDI events.
 *
 * The buttons are debounced.
 *
 * @see     Button
 */
template <class Sender, uint8_t NUMBER_OF_BUTTONS>
class MIDIButtons : public MIDIOutputElement {
  protected:
    /**
     * @brief   Construct a new MIDIButtons.
     *
     * @todo    Documentation
     */
    MIDIButtons(const Array<AH::Button, NUMBER_OF_BUTTONS> &buttons,
                const MIDICNChannelAddress &baseAddress,
                const RelativeMIDICNChannelAddress &incrementAddress,
                const Sender &sender)
        : buttons{buttons}, baseAddress(baseAddress),
          incrementAddress(incrementAddress), sender{sender} {}

  public:
    void begin() final override {
        for (auto &button : buttons)
            button.begin();
    }
    void update() final override {
        MIDICNChannelAddress address = baseAddress;
        for (auto &button : buttons) {
            AH::Button::State state = button.update();
            if (state == AH::Button::Falling) {
                sender.sendOn(address);
            } else if (state == AH::Button::Rising) {
                sender.sendOff(address);
            }
            address += incrementAddress;
        }
    }

    AH::Button::State getButtonState(size_t index) const {
        return buttons[index].getState();
    }

#ifdef AH_INDIVIDUAL_BUTTON_INVERT
    void invert() {
        for (auto &button : buttons)
            button.invert();
    }
#endif

  private:
    Array<AH::Button, NUMBER_OF_BUTTONS> buttons;
    const MIDICNChannelAddress baseAddress;
    const RelativeMIDICNChannelAddress incrementAddress;

  public:
    Sender sender;
};

END_CS_NAMESPACE