#pragma once

#include <AH/Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>
#include <MIDI_Inputs/NoteCCKPValue.hpp>

BEGIN_CS_NAMESPACE

// -------------------------------------------------------------------------- //

/// Generic base class for classes that listen for MIDI Note, Control Change and
/// Key Pressure events on a single address and turns on an LED when the value
/// is higher than a threshold.
///
/// @tparam Type
///         The type of MIDI messages to listen for:
///         - @ref MIDIMessageType::NoteOn
///         - @ref MIDIMessageType::ControlChange
///         - @ref MIDIMessageType::KeyPressure
template <MIDIMessageType Type>
class NoteCCKPLED : public MatchingMIDIInputElement<Type, TwoByteMIDIMatcher> {
  public:
    using Matcher = TwoByteMIDIMatcher;
    using Parent = MatchingMIDIInputElement<Type, Matcher>;

    /// @param  ledPin
    ///         The output pin with the LED connected.
    /// @param  address
    ///         The address to listen to.
    NoteCCKPLED(pin_t ledPin, MIDIAddress address)
        : Parent(address), ledPin(ledPin) {}

  private:
    void handleUpdate(typename Matcher::Result match) override {
        PinStatus_t state = match.value >= threshold ? HIGH : LOW;
        AH::ExtIO::digitalWrite(ledPin, state);
    }

  public:
    /// Set the pinmode of the LED to OUTPUT.
    void begin() override {
        AH::ExtIO::pinMode(ledPin, OUTPUT);
        AH::ExtIO::digitalWrite(ledPin, LOW);
    }

    /// Turn off the LED.
    void reset() override { AH::ExtIO::digitalWrite(ledPin, LOW); }

    /// Get the LED threshold.
    uint8_t getThreshold() const { return threshold; }
    /// Set the LED threshold. LED will be on if `getValue() >= getThreshold()`.
    void setThreshold(uint8_t threshold) { this->threshold = threshold; }

  private:
    pin_t ledPin;
    uint8_t threshold = 0x01;
};

/// Class that listens for MIDI Note events on a single address and turns
/// on an LED when the value is higher than a threshold.
/// @ingroup    midi-input-elements-leds
using NoteLED = NoteCCKPLED<MIDIMessageType::NoteOn>;

/// Class that listens for MIDI Control Change events on a single address and
/// turns on an LED when the value is higher than a threshold.
/// @ingroup    midi-input-elements-leds
using CCLED = NoteCCKPLED<MIDIMessageType::ControlChange>;

/// Class that listens for MIDI Key Pressure events on a single address and
/// turns on an LED when the value is higher than a threshold.
/// @ingroup    midi-input-elements-leds
using KPLED = NoteCCKPLED<MIDIMessageType::KeyPressure>;

using NoteValueLED [[deprecated("Use NoteLED instead")]] = NoteLED;
using CCValueLED [[deprecated("Use CCLED instead")]] = CCLED;

// -------------------------------------------------------------------------- //

namespace Bankable {

/// Generic base class for classes that listen for MIDI Note, Control Change and
/// Key Pressure events on a single address and save their value. This version
/// listens accross multiple banks.
///
/// @tparam Type
///         The type of MIDI messages to listen for:
///         - @ref MIDIMessageType::NoteOn
///         - @ref MIDIMessageType::ControlChange
///         - @ref MIDIMessageType::KeyPressure
/// @tparam BankSize
///         The number of banks.
template <MIDIMessageType Type, uint8_t BankSize>
class NoteCCKPLED : public NoteCCKPValue<Type, BankSize> {
  public:
    using Parent = NoteCCKPValue<Type, BankSize>;
    using Matcher = typename Parent::Matcher;

    /// @param  config
    ///         The bank configuration to use.
    /// @param  ledPin
    ///         The output pin with the LED connected.
    /// @param  address
    ///         The base address to listen to.
    NoteCCKPLED(BankConfig<BankSize> config, pin_t ledPin, MIDIAddress address)
        : Parent(config, address), ledPin(ledPin) {}

  protected:
    void handleUpdate(typename Matcher::Result match) override {
        bool newdirty = Parent::handleUpdateImpl(match);
        if (newdirty)
            display();
        this->dirty |= newdirty;
    }

    void display() {
        PinStatus_t state = getValue() >= threshold ? HIGH : LOW;
        AH::ExtIO::digitalWrite(ledPin, state);
    }

  public:
    /// Set the pinmode of the LED to OUTPUT.
    void begin() override {
        AH::ExtIO::pinMode(ledPin, OUTPUT);
        AH::ExtIO::digitalWrite(ledPin, LOW);
    }

    /// Reset all values to zero and turn off the LED.
    void reset() override {
        Parent::reset();
        AH::ExtIO::digitalWrite(ledPin, LOW);
    }

    using Parent::getValue;

    /// Get the LED threshold.
    uint8_t getThreshold() const { return threshold; }
    /// Set the LED threshold. LED will be on if `getValue() >= getThreshold()`.
    void setThreshold(uint8_t threshold) { this->threshold = threshold; }

  protected:
    void onBankSettingChange() override {
        Parent::onBankSettingChange();
        display();
    }

  private:
    pin_t ledPin;
    uint8_t threshold = 0x01;
};

/// Class that listens for MIDI Note events on a single address and
/// turns on an LED when the value is higher than a threshold.
/// This version listens accross multiple banks.
/// @ingroup    BankableMIDIInputElementsLEDs
template <uint8_t BankSize>
using NoteLED = NoteCCKPLED<MIDIMessageType::NoteOn, BankSize>;

/// Class that listens for MIDI Control Change events on a single address and
/// turns on an LED when the value is higher than a threshold.
/// This version listens accross multiple banks.
/// @ingroup    BankableMIDIInputElementsLEDs
template <uint8_t BankSize>
using CCLED = NoteCCKPLED<MIDIMessageType::ControlChange, BankSize>;

/// Class that listens for MIDI Key Pressure events on a single address and
/// turns on an LED when the value is higher than a threshold.
/// This version listens accross multiple banks.
/// @ingroup    BankableMIDIInputElementsLEDs
template <uint8_t BankSize>
using KPLED = NoteCCKPLED<MIDIMessageType::KeyPressure, BankSize>;

template <uint8_t BankSize>
using NoteValueLED [[deprecated("Use NoteLED instead")]] = NoteLED<BankSize>;
template <uint8_t BankSize>
using CCValueLED [[deprecated("Use CCLED instead")]] = CCLED<BankSize>;

} // namespace Bankable

END_CS_NAMESPACE