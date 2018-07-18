#pragma once

#include "Abstract_MIDI_Output_Addressable.hpp"
#include <Hardware/Button.h>

/**
 * @brief   An abstract class for momentary push buttons that send MIDI events.
 * 
 * The button is debounced.
 * 
 * @see     Button
 */
class MIDIButton : public virtual Abstract_Digital_MIDI_Output_Addressable {
  public:
    /**
     * @brief   Construct a new MIDIButton.
     * 
     * @param   pin
     *          The digital input pin with the button connected.  
     *          The internal pull-up resistor will be enabled.
     */
    MIDIButton(pin_t pin, uint8_t baseAddress, uint8_t baseChannel)
        : button{pin}, baseAddress(baseAddress), baseChannel(baseChannel) {}

    void update() final override {
        Button::State state = button.getState();
        if (state == Button::Falling) {
            sendOn(baseChannel, baseAddress);
        } else if (state == Button::Rising) {
            sendOff(baseChannel, baseAddress);
        }
    }

  private:
    Button button;
    const uint8_t baseAddress;
    const uint8_t baseChannel;
};

// -------------------------------------------------------------------------- //

#include <Banks/BankableMIDIOutputAddressable.hpp>

/**
 * @brief   An abstract class for momentary push buttons that send MIDI events.
 * 
 * The button is debounced.
 * 
 * @see     Button
 */
class MIDIButton_Bankable
    : public virtual Abstract_Digital_MIDI_Output_Addressable,
      public BankableMIDIOutputAddressable {
  public:
    /**
     * @brief   Construct a new bankable MIDIButton.
     * 
     * @param   pin
     *          The digital input pin with the button connected.  
     *          The internal pull-up resistor will be enabled.
     */
    MIDIButton_Bankable(pin_t pin, uint8_t baseAddress, uint8_t baseChannel)
        : button{pin}, baseAddress(baseAddress), baseChannel(baseChannel) {}

    void update() final override {
        Button::State state = button.getState();
        if (state == Button::Falling) {
            channel = getChannel(getBaseChannel());
            address = getAddress(getBaseAddress());
            sendOn(channel, address);
        } else if (state == Button::Rising) {
            sendOff(channel, address);
        }
    }

    uint8_t getBaseAddress() const { return baseAddress; }
    uint8_t getBaseChannel() const { return baseChannel; }

  private:
    Button button;
    const uint8_t baseAddress;
    const uint8_t baseChannel;
    uint8_t channel;
    uint8_t address;
};

// -------------------------------------------------------------------------- //