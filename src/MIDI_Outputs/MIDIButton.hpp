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
            lock();
            sendOn(getChannel(baseChannel), getAddress(baseAddress));
        } else if (state == Button::Rising) {
            sendOff(getChannel(baseChannel), getAddress(baseAddress));
            unlock();
        }
    }

  private:
    Button button;
    const uint8_t baseAddress;
    const uint8_t baseChannel;
};

// -------------------------------------------------------------------------- //

/**
 * @brief   An abstract class for momentary push buttons that send MIDI events.
 * 
 * The buttons are debounced.
 * 
 * @see     Button
 */
template <uint8_t NUMBER_OF_BUTTONS>
class MIDIButtons_Bankable
    : public virtual Abstract_Digital_MIDI_Output_Addressable,
      public BankableMIDIOutputAddressable {
  public:
    /**
     * @brief   Construct a new MIDIButtons_Bankable.
     * 
     * @todo    Documentation
     */
    MIDIButtons_Bankable(const Button (&buttons)[NUMBER_OF_BUTTONS],
                         uint8_t baseAddress, uint8_t baseChannel,
                         uint8_t addressIncrement, uint8_t channelIncrement)
        : buttons(buttons), baseAddress(baseAddress), baseChannel(baseChannel),
          increment(((channelIncrement & 0xF) << 4) |
                    (addressIncrement & 0xF)) {}

    void update() final override {
        uint8_t channel = baseChannel;
        uint8_t address = baseAddress;
        uint8_t channelIncrement = getChannelIncrement();
        uint8_t addressIncrement = getAddressIncrement();
        for (uint8_t i = 0; i < NUMBER_OF_BUTTONS; i++) {
            Button::State state = buttons[i].getState();
            if (state == Button::Falling) {
                if (!activeButtons)
                    lock(); // Don't allow changing of the bank setting
                activeButtons++;
                sendOn(channel, address);
            } else if (state == Button::Rising) {
                sendOff(channel, address);
                activeButtons--;
                if (!activeButtons)
                    unlock();
            }
            channel += channelIncrement;
            address += addressIncrement;
        }
    }

    uint8_t getChannelIncrement() const { return increment >> 4; }
    uint8_t getAddressIncrement() const { return increment & 0xF; }

  private:
    Button buttons[NUMBER_OF_BUTTONS];
    const uint8_t baseAddress;
    const uint8_t baseChannel;
    const uint8_t increment;
    uint8_t activeButtons = 0;
};
// -------------------------------------------------------------------------- //

/**
 * @brief   An abstract class for momentary push buttons that send MIDI events.
 * 
 * The buttons are debounced.
 * 
 * @see     Button
 */
template <uint8_t NUMBER_OF_BUTTONS>
class MIDIButtons : public virtual Abstract_Digital_MIDI_Output_Addressable {
  public:
    /**
     * @brief   Construct a new MIDIButtons.
     * 
     * @todo    Documentation
     */
    MIDIButtons(const Button (&buttons)[NUMBER_OF_BUTTONS], uint8_t baseAddress,
                uint8_t baseChannel, uint8_t addressIncrement,
                uint8_t channelIncrement)
        : buttons(buttons), baseAddress(baseAddress), baseChannel(baseChannel),
          increment(((channelIncrement & 0xF) << 4) |
                    (addressIncrement & 0xF)) {}

    void update() final override {
        uint8_t channel = baseChannel;
        uint8_t address = baseAddress;
        uint8_t channelIncrement = getChannelIncrement();
        uint8_t addressIncrement = getAddressIncrement();
        for (uint8_t i = 0; i < NUMBER_OF_BUTTONS; i++) {
            Button::State state = buttons[i].getState();
            if (state == Button::Falling) {
                sendOn(channel, address);
            } else if (state == Button::Rising) {
                sendOff(channel, address);
            }
            channel += channelIncrement;
            address += addressIncrement;
        }
    }

    uint8_t getChannelIncrement() const { return increment >> 4; }
    uint8_t getAddressIncrement() const { return increment & 0xF; }

  private:
    Button buttons[NUMBER_OF_BUTTONS];
    const uint8_t baseAddress;
    const uint8_t baseChannel;
    const uint8_t increment;
};