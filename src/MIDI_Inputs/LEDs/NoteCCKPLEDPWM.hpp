#pragma once

#include <AH/Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>
#include <AH/Math/IncreaseBitDepth.hpp>
#include <MIDI_Inputs/NoteCCKPValue.hpp>

BEGIN_CS_NAMESPACE

// -------------------------------------------------------------------------- //

/// Generic base class for classes that listen for MIDI Note, Control Change and
/// Key Pressure events on a single address and turns on an LED with a
/// brightness/duty cycle proportional to the MIDI value.
///
/// @tparam Type
///         The type of MIDI messages to listen for:
///         - MIDIMessageType::NOTE_ON
///         - MIDIMessageType::CONTROL_CHANGE
///         - MIDIMessageType::KEY_PRESSURE
template <MIDIMessageType Type>
class NoteCCKPLEDPWM
    : public MatchingMIDIInputElement<Type, TwoByteMIDIMatcher> {
  public:
    /// Constructor.
    ///
    /// @param  ledPin
    ///         The PWM pin with the LED connected.
    /// @param  address
    ///         The address to listen to.
    NoteCCKPLEDPWM(pin_t ledPin, MIDIAddress address)
        : MatchingMIDIInputElement<Type, TwoByteMIDIMatcher>(address),
          ledPin(ledPin) {}

  private:
    void handleUpdate(typename TwoByteMIDIMatcher::Result match) override {
        auto value = AH::increaseBitDepth<8, 7, uint8_t, uint8_t>(match.value);
        AH::ExtIO::analogWrite(ledPin, value);
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
using NoteLEDPWM = NoteCCKPLEDPWM<MIDIMessageType::NOTE_ON>;

/// Class that listens for MIDI Control Change events on a single address and
/// turns on an LED when the value is higher than a threshold.
/// @ingroup    midi-input-elements-leds
using CCLEDPWM = NoteCCKPLEDPWM<MIDIMessageType::CONTROL_CHANGE>;

/// Class that listens for MIDI Key Pressure events on a single address and
/// turns on an LED when the value is higher than a threshold.
/// @ingroup    midi-input-elements-leds
using KPLEDPWM = NoteCCKPLEDPWM<MIDIMessageType::KEY_PRESSURE>;

// -------------------------------------------------------------------------- //

namespace Bankable {

/// Generic base class for classes that listen for MIDI Note, Control Change and
/// Key Pressure events on a single address and save their value. This version
/// listens accross multiple banks.
///
/// @tparam Type
///         The type of MIDI messages to listen for:
///         - MIDIMessageType::NOTE_ON
///         - MIDIMessageType::CONTROL_CHANGE
///         - MIDIMessageType::KEY_PRESSURE
/// @tparam BankSize
///         The number of banks.
template <MIDIMessageType Type, uint8_t BankSize>
class NoteCCKPLEDPWM : public NoteCCKPValue<Type, BankSize> {
  public:
    using Matcher = typename NoteCCKPValue<Type, BankSize>::Matcher;

    /// Constructor.
    ///
    /// @param  config
    ///         The bank configuration to use.
    /// @param  ledPin
    ///         The PWM pin with the LED connected.
    /// @param  address
    ///         The base address to listen to.
    NoteCCKPLEDPWM(BankConfig<BankSize> config, pin_t ledPin,
                   MIDIAddress address)
        : NoteCCKPValue<Type, BankSize>(config, address), ledPin(ledPin) {}

  protected:
    void handleUpdate(typename Matcher::Result match) override {
        NoteCCKPValue<Type, BankSize>::handleUpdate(match);
        if (getDirty()) {
            display();
            clearDirty();
        }
    }

    void display() {
        auto value = AH::increaseBitDepth<8, 7, uint8_t, uint8_t>(getValue());
        AH::ExtIO::analogWrite(ledPin, value);
    }

  public:
    /// Set the pinmode of the LED to OUTPUT.
    void begin() override {
        AH::ExtIO::pinMode(ledPin, OUTPUT);
        AH::ExtIO::digitalWrite(ledPin, LOW);
    }

    /// Reset all values to zero and turn off the LED.
    void reset() override {
        NoteCCKPValue<Type, BankSize>::reset();
        AH::ExtIO::digitalWrite(ledPin, LOW);
        clearDirty();
    }

    using NoteCCKPValue<Type, BankSize>::getValue;

    /// Get the LED threshold.
    uint8_t getThreshold() const { return threshold; }
    /// Set the LED threshold. LED will be on if `getValue() >= getThreshold()`.
    void setThreshold(uint8_t threshold) { this->threshold = threshold; }

  private: // These methods are used internally, don't allow external use.
    using NoteCCKPValue<Type, BankSize>::getDirty;
    using NoteCCKPValue<Type, BankSize>::clearDirty;

  protected:
    void onBankSettingChange() override { display(); }

  private:
    pin_t ledPin;
    uint8_t threshold = 0x01;
};

/// Class that listens for MIDI Note events on a single address and
/// turns on an LED when the value is higher than a threshold.
/// This version listens accross multiple banks.
/// @ingroup    BankableMIDIInputElementsLEDs
template <uint8_t BankSize>
using NoteLEDPWM = NoteCCKPLEDPWM<MIDIMessageType::NOTE_ON, BankSize>;

/// Class that listens for MIDI Control Change events on a single address and
/// turns on an LED when the value is higher than a threshold.
/// This version listens accross multiple banks.
/// @ingroup    BankableMIDIInputElementsLEDs
template <uint8_t BankSize>
using CCLEDPWM = NoteCCKPLEDPWM<MIDIMessageType::CONTROL_CHANGE, BankSize>;

/// Class that listens for MIDI Key Pressure events on a single address and
/// turns on an LED when the value is higher than a threshold.
/// This version listens accross multiple banks.
/// @ingroup    BankableMIDIInputElementsLEDs
template <uint8_t BankSize>
using KPLEDPWM = NoteCCKPLEDPWM<MIDIMessageType::KEY_PRESSURE, BankSize>;

} // namespace Bankable

END_CS_NAMESPACE