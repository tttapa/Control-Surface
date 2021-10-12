#pragma once

#include <AH/Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>
#include <AH/Math/IncreaseBitDepth.hpp>
#include <MIDI_Inputs/NoteCCKPValue.hpp>

BEGIN_CS_NAMESPACE

// -------------------------------------------------------------------------- //

/// Generic base class for classes that listen for MIDI Note, Control Change and
/// Key Pressure events on a single address and turn on an LED with a
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
    using Matcher = TwoByteMIDIMatcher;
    using Parent = MatchingMIDIInputElement<Type, Matcher>;

    /// @param  ledPin
    ///         The PWM pin with the LED connected.
    /// @param  address
    ///         The address to listen to.
    NoteCCKPLEDPWM(pin_t ledPin, MIDIAddress address)
        : Parent(address), ledPin(ledPin) {}

  private:
    void handleUpdate(typename Matcher::Result match) override {
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

  private:
    pin_t ledPin;
};

/// Class that listens for MIDI Note events on a single address and turns
/// on an LED with a brightness/duty cycle proportional to the velocity.
/// @ingroup    midi-input-elements-leds
using NoteLEDPWM = NoteCCKPLEDPWM<MIDIMessageType::NOTE_ON>;

/// Class that listens for MIDI Control Change events on a single address and
/// turns on an LED with a brightness/duty cycle proportional to the value.
/// @ingroup    midi-input-elements-leds
using CCLEDPWM = NoteCCKPLEDPWM<MIDIMessageType::CONTROL_CHANGE>;

/// Class that listens for MIDI Key Pressure events on a single address and
/// turns on an LED with a brightness/duty cycle proportional to the pressure.
/// @ingroup    midi-input-elements-leds
using KPLEDPWM = NoteCCKPLEDPWM<MIDIMessageType::KEY_PRESSURE>;

// -------------------------------------------------------------------------- //

namespace Bankable {

/// Generic base class for classes that listen for MIDI Note, Control Change and
/// Key Pressure events on a single address and turn on an LED with a
/// brightness/duty cycle proportional to the MIDI value.
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
    using Parent = NoteCCKPValue<Type, BankSize>;
    using Matcher = typename Parent::Matcher;

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
        : Parent(config, address), ledPin(ledPin) {}

  protected:
    void handleUpdate(typename Matcher::Result match) override {
        bool newdirty = Parent::handleUpdateImpl(match);
        if (newdirty) 
            display();
        this->dirty |= newdirty;
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
        Parent::reset();
        AH::ExtIO::digitalWrite(ledPin, LOW);
    }

    using Parent::getValue;

  protected:
    void onBankSettingChange() override { 
        Parent::onBankSettingChange();
        display(); 
    }

  private:
    pin_t ledPin;
};

/// Class that listens for MIDI Note events on a single address and
/// turns on an LED with a brightness/duty cycle proportional to the velocity.
/// This version listens accross multiple banks.
/// @ingroup    BankableMIDIInputElementsLEDs
template <uint8_t BankSize>
using NoteLEDPWM = NoteCCKPLEDPWM<MIDIMessageType::NOTE_ON, BankSize>;

/// Class that listens for MIDI Control Change events on a single address and
/// turns on an LED with a brightness/duty cycle proportional to the value.
/// This version listens accross multiple banks.
/// @ingroup    BankableMIDIInputElementsLEDs
template <uint8_t BankSize>
using CCLEDPWM = NoteCCKPLEDPWM<MIDIMessageType::CONTROL_CHANGE, BankSize>;

/// Class that listens for MIDI Key Pressure events on a single address and
/// turns on an LED with a brightness/duty cycle proportional to the pressure.
/// This version listens accross multiple banks.
/// @ingroup    BankableMIDIInputElementsLEDs
template <uint8_t BankSize>
using KPLEDPWM = NoteCCKPLEDPWM<MIDIMessageType::KEY_PRESSURE, BankSize>;

} // namespace Bankable

END_CS_NAMESPACE