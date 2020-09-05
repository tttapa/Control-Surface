#pragma once

#include <AH/Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>
#include <MIDI_Inputs/NoteCCKPValue.hpp>

BEGIN_CS_NAMESPACE

// -------------------------------------------------------------------------- //

/// Generic base class for classes that listen for MIDI Note, Control Change and
/// Key Pressure events on a range of addresses and turns on the corresponding
/// LED when the value is higher than a threshold.
///
/// @tparam Type
///         The type of MIDI messages to listen for:
///         - MIDIMessageType::NOTE_ON
///         - MIDIMessageType::CONTROL_CHANGE
///         - MIDIMessageType::KEY_PRESSURE
/// @tparam RangeLen
///         The length of the range of addresses to listen to.
template <MIDIMessageType Type, uint8_t RangeLen>
class NoteCCKPRangeLEDs
    : public MatchingMIDIInputElement<Type, TwoByteRangeMIDIMatcher> {
  public:
    /// Constructor.
    ///
    /// @param  ledPins
    ///         The output pins with the LEDs connected.
    /// @param  address
    ///         The first address to listen to.
    NoteCCKPRangeLEDs(AH::PinList<RangeLen> ledPins, MIDIAddress address)
        : MatchingMIDIInputElement<Type, TwoByteRangeMIDIMatcher>(
              {address, RangeLen}),
          ledPins(ledPins) {}

  private:
    void handleUpdate(typename TwoByteRangeMIDIMatcher::Result match) override {
        PinStatus_t state = match.value >= threshold ? HIGH : LOW;
        AH::ExtIO::digitalWrite(ledPins[match.index], state);
    }

  public:
    /// Set the pinmode of the LEDs to OUTPUT.
    void begin() override {
        for (pin_t ledPin : ledPins) {
            AH::ExtIO::pinMode(ledPin, OUTPUT);
            AH::ExtIO::digitalWrite(ledPin, LOW);
        }
    }

    /// Turn off the LEDs.
    void reset() override {
        for (pin_t ledPin : ledPins)
            AH::ExtIO::digitalWrite(ledPin, LOW);
    }

    /// Get the LED threshold.
    uint8_t getThreshold() const { return threshold; }
    /// Set the LED threshold. LED will be on if `getValue() >= getThreshold()`.
    void setThreshold(uint8_t threshold) { this->threshold = threshold; }

  private:
    AH::PinList<RangeLen> ledPins;
    uint8_t threshold = 0x01;
};

/// Class that listens for MIDI Note events on a range of addresses and turns
/// on the corresponding LED when the value is higher than a threshold.
/// @ingroup    midi-input-elements-leds
template <uint8_t RangeLen>
using NoteRangeLEDs = NoteCCKPRangeLEDs<MIDIMessageType::NOTE_ON, RangeLen>;

/// Class that listens for MIDI Control Change events on a range of addresses
/// and turns on the corresponding LED when the value is higher than a threshold.
/// @ingroup    midi-input-elements-leds
template <uint8_t RangeLen>
using CCRangeLEDs = NoteCCKPRangeLEDs<MIDIMessageType::CONTROL_CHANGE, RangeLen>;

/// Class that listens for MIDI Key Pressure events on a range of addresses and
/// turns on the corresponding LED when the value is higher than a threshold.
/// @ingroup    midi-input-elements-leds
template <uint8_t RangeLen>
using KPRangeLEDs = NoteCCKPRangeLEDs<MIDIMessageType::KEY_PRESSURE, RangeLen>;

// -------------------------------------------------------------------------- //

namespace Bankable {

/// Generic base class for classes that listen for MIDI Note, Control Change and
/// Key Pressure events on a range of addresses and save their values. This
/// version listens accross multiple banks.
///
/// @tparam Type
///         The type of MIDI messages to listen for:
///         - MIDIMessageType::NOTE_ON
///         - MIDIMessageType::CONTROL_CHANGE
///         - MIDIMessageType::KEY_PRESSURE
/// @tparam BankSize
///         The number of banks.
/// @tparam RangeLen
///         The length of the range of addresses to listen to.
template <MIDIMessageType Type, uint8_t BankSize, uint8_t RangeLen>
class NoteCCKPRangeLEDs : public NoteCCKPRange<Type, BankSize, RangeLen> {
  public:
    using Matcher = typename NoteCCKPRange<Type, BankSize, RangeLen>::Matcher;

    /// Constructor.
    ///
    /// @param  config
    ///         The bank configuration to use.
    /// @param  ledPins
    ///         The output pins with the LEDs connected.
    /// @param  address
    ///         The base address to listen to.
    NoteCCKPRangeLEDs(BankConfig<BankSize> config,
                      AH::PinList<RangeLen> ledPins, MIDIAddress address)
        : NoteCCKPRange<Type, BankSize, RangeLen>(config, address),
          ledPins(ledPins) {}

  protected:
    void handleUpdate(typename Matcher::Result match) override {
        NoteCCKPRange<Type, BankSize, RangeLen>::handleUpdate(match);
        if (getDirty()) {
            display(match.index);
            clearDirty();
        }
    }

    void display(uint8_t index) {
        PinStatus_t state = getValue(index) >= threshold ? HIGH : LOW;
        AH::ExtIO::digitalWrite(ledPins[index], state);
    }

  public:
    /// Set the pinmode of the LEDs to OUTPUT.
    void begin() override {
        for (pin_t ledPin : ledPins) {
            AH::ExtIO::pinMode(ledPin, OUTPUT);
            AH::ExtIO::digitalWrite(ledPin, LOW);
        }
    }

    /// Reset all values to zero and turn off the LEDs.
    void reset() override {
        NoteCCKPRange<Type, BankSize, RangeLen>::reset();
        for (pin_t ledPin : ledPins)
            AH::ExtIO::digitalWrite(ledPin, LOW);
        clearDirty();
    }

    using NoteCCKPRange<Type, BankSize, RangeLen>::getValue;

    /// Get the LED threshold.
    uint8_t getThreshold() const { return threshold; }
    /// Set the LED threshold. LED will be on if `getValue() >= getThreshold()`.
    void setThreshold(uint8_t threshold) { this->threshold = threshold; }

  private: // These methods are used internally, don't allow external use.
    using NoteCCKPRange<Type, BankSize, RangeLen>::getDirty;
    using NoteCCKPRange<Type, BankSize, RangeLen>::clearDirty;

  protected:
    void onBankSettingChange() override {
        for (uint8_t index = 0; index < RangeLen; ++index)
            display(index);
    }

  private:
    AH::PinList<RangeLen> ledPins;
    uint8_t threshold = 0x01;
};

/// Class that listens for MIDI Note events on a single address and
/// turns on an LED when the value is higher than a threshold.
/// This version listens accross multiple banks.
/// @ingroup    BankableMIDIInputElementsLEDs
template <uint8_t BankSize, uint8_t RangeLen>
using NoteRangeLEDs =
    NoteCCKPRangeLEDs<MIDIMessageType::NOTE_ON, BankSize, RangeLen>;

/// Class that listens for MIDI Control Change events on a single address and
/// turns on an LED when the value is higher than a threshold.
/// This version listens accross multiple banks.
/// @ingroup    BankableMIDIInputElementsLEDs
template <uint8_t BankSize, uint8_t RangeLen>
using CCRangeLEDs =
    NoteCCKPRangeLEDs<MIDIMessageType::CONTROL_CHANGE, BankSize, RangeLen>;

/// Class that listens for MIDI Key Pressure events on a single address and
/// turns on an LED when the value is higher than a threshold.
/// This version listens accross multiple banks.
/// @ingroup    BankableMIDIInputElementsLEDs
template <uint8_t BankSize, uint8_t RangeLen>
using KPRangeLEDs =
    NoteCCKPRangeLEDs<MIDIMessageType::KEY_PRESSURE, BankSize, RangeLen>;

} // namespace Bankable

END_CS_NAMESPACE