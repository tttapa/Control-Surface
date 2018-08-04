#pragma once

#include <Def/Def.hpp>
#include <Hardware/IncrementDecrementButtons.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

#include <MIDI_Senders/DigitalNoteSender.hpp>

/**
 * @brief   An abstract class for two buttons that send incremental MIDI events.
 */
template <RelativeSendFunction send>
class MIDIIncrementDecrementButtons : public MIDIOutputElement {
  protected:
    /**
     * @brief   Construct a new MIDIIncrementDecrementButtons.
     *
     * @todo    Documentation
     */
    MIDIIncrementDecrementButtons(const IncrementDecrementButtons &buttons,
                                  uint8_t address, uint8_t channel = 1,
                                  uint8_t speedMultiply = 1,
                                  uint8_t resetAddress = NO_ADDRESS)
        : buttons(buttons), address(address), channel(channel),
          speedMultiply(speedMultiply), resetAddress(resetAddress) {}

  public:
    void begin() override { buttons.begin(); }

    void update() override {
        using IncrDecrButtons = IncrementDecrementButtons;
        switch (buttons.getState()) {
            case IncrDecrButtons::Increment:
                send(speedMultiply, channel, address);
                break;
            case IncrDecrButtons::Decrement:
                send(-speedMultiply, channel, address);
                break;
            case IncrDecrButtons::Reset: reset(); break;
            default: break;
        }
    }

    void reset() {
        if (resetAddress != NO_ADDRESS) {
            sendOn(channel, resetAddress);
            sendOff(channel, resetAddress);
        }
    }

  private:
    IncrementDecrementButtons buttons;
    const uint8_t address;
    const uint8_t channel;
    const uint8_t speedMultiply;
    const uint8_t resetAddress;

    constexpr static DigitalSendFunction sendOn = DigitalNoteSender::sendOn;
    constexpr static DigitalSendFunction sendOff = DigitalNoteSender::sendOff;
};