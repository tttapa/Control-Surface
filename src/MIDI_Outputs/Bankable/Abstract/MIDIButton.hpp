#pragma once

#include <AH/Hardware/Button.hpp>
#include <Banks/BankableAddresses.hpp>
#include <Def/Def.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>
#include <Control_Surface/Control_Surface_Class.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {

/**
 * @brief   An abstract class for momentary push buttons that send MIDI events.
 *
 * The button is debounced.
 *
 * @see     Button
 */
template <class BankAddress, class Sender>
class MIDIButton : public MIDIOutputElement {
  public:
    /**
     * @brief   Construct a new bankable MIDIButton.
     *
     * @param   bankAddress
     *          The bankable MIDI address to send to.
     * @param   pin
     *          The digital input pin with the button connected.
     *          The internal pull-up resistor will be enabled.
     * @param   sender
     *          The MIDI sender to use.
     */
    MIDIButton(const BankAddress &bankAddress, pin_t pin, const Sender &sender)
        : address{bankAddress}, button{pin}, sender(sender) {}

    /**
     * @brief   Initialize the button. Enables the internal pull-up resistor.
     * @see     AH::Button::begin
     */
    void begin() override { button.begin(); }

    void update() override {
        // Lock the bank setting and get the MIDI cable number to lock
        bool unlock = address.lock();
        auto sendAddress = address.getActiveAddress();
        auto cn = sendAddress.getCableNumber();

        // Lock the MIDI cable and update the button if successful
        if (Control_Surface.try_lock_mutex(cn)) {

            AH::Button::State state = button.update();
            if (state == AH::Button::Falling) {
                sender.sendOn(sendAddress);
                unlock = false;
            } else if (state == AH::Button::Rising) {
                sender.sendOff(sendAddress);
                unlock = true;
            }

            // Unlock the MIDI cable
            Control_Surface.unlock_mutex(cn);
        }

        // Unlock the bank setting again
        if (unlock)
            address.unlock();
    }

#ifdef AH_INDIVIDUAL_BUTTON_INVERT
    void invert() { button.invert(); }
#endif

    AH::Button::State getButtonState() const { return button.getState(); }

  private:
    BankAddress address;
    AH::Button button;

  public:
    Sender sender;
};

} // namespace Bankable

END_CS_NAMESPACE