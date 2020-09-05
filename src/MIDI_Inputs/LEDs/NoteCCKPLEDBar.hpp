#include <AH/Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>
#include <AH/Hardware/LEDs/DotBarDisplayLEDs.hpp>
#include <MIDI_Inputs/NoteCCKPValue.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   Callback class that drives a LED dot/bar display based on a note or
 *          control change value.
 * 
 * @tparam  NumLEDs 
 *          The number of LEDs the display has.
 */
template <uint8_t NumLEDs>
class NoteCCKPLEDBarDriver : public AH::DotBarDisplayLEDs<NumLEDs> {
  public:
    NoteCCKPLEDBarDriver(const AH::PinList<NumLEDs> &leds)
        : AH::DotBarDisplayLEDs<NumLEDs>(leds) {}

    void displayBar(uint8_t value) { this->display(value / 128.0f); }
};

// -------------------------------------------------------------------------- //

template <MIDIMessageType Type, uint8_t NumLEDs>
class NoteCCKPValueBar : public NoteCCKPValue<Type>,
                         public NoteCCKPLEDBarDriver<NumLEDs> {
  public:
    NoteCCKPValueBar(const AH::PinList<NumLEDs> &leds, MIDIAddress address)
        : NoteCCKPValue<Type>(address), NoteCCKPLEDBarDriver<NumLEDs>(leds) {}

  protected:
    void handleUpdate(TwoByteMIDIMatcher::Result match) override {
        NoteCCKPValue<Type>::handleUpdate(match);
        updateDisplay();
    }

    /// If the state is dirty, update the LEDs
    void updateDisplay() {
        if (getDirty()) {
            this->displayBar(this->getValue());
            clearDirty();
        }
    }

  public:
    void begin() override {
        NoteCCKPValue<Type>::begin();
        NoteCCKPLEDBarDriver<NumLEDs>::begin();
        updateDisplay();
    }

    void reset() override {
        NoteCCKPValue<Type>::reset();
        updateDisplay();
    }

  protected:
    using NoteCCKPValue<Type>::clearDirty;
    using NoteCCKPValue<Type>::getDirty;
};

// -------------------------------------------------------------------------- //

/// Class that listens for **Note** events and displays the velocity on an
/// **LED Bar Graph**.
/// @tparam  NumLEDs
///          The number of LEDs the display has.
/// @ingroup midi-input-elements-leds
template <uint8_t NumLEDs>
using NoteLEDBar = NoteCCKPValueBar<MIDIMessageType::NOTE_ON, NumLEDs>;

/// Class that listens for **Control Change** events and displays the
/// value on an **LED Bar Graph**.
/// @tparam  NumLEDs
///          The number of LEDs the display has.
/// @ingroup midi-input-elements-leds
template <uint8_t NumLEDs>
using CCLEDBar = NoteCCKPValueBar<MIDIMessageType::CONTROL_CHANGE, NumLEDs>;

/// Class that listens for **Key Pressure** events and displays the pressure on
/// an **LED Bar Graph**.
/// @tparam  NumLEDs
///          The number of LEDs the display has.
/// @ingroup midi-input-elements-leds
template <uint8_t NumLEDs>
using KPLEDBar = NoteCCKPValueBar<MIDIMessageType::KEY_PRESSURE, NumLEDs>;

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

namespace Bankable {

template <MIDIMessageType Type, uint8_t BankSize, uint8_t NumLEDs>
class NoteCCKPValueBar : public NoteCCKPValue<Type, BankSize>,
                         public NoteCCKPLEDBarDriver<NumLEDs> {
  public:
    NoteCCKPValueBar(const AH::PinList<NumLEDs> &leds, MIDIAddress address)
        : NoteCCKPValue<Type, BankSize>(address), NoteCCKPLEDBarDriver<NumLEDs>(
                                                      leds) {}

  protected:
    void handleUpdate(
        typename BankableTwoByteMIDIMatcher<BankSize>::Result match) override {
        NoteCCKPValue<Type, BankSize>::handleUpdate(match);
        updateDisplay();
    }

    /// If the state is dirty, update the LEDs
    void updateDisplay() {
        if (getDirty()) {
            this->displayBar(this->getValue());
            clearDirty();
        }
    }

  public:
    void begin() override {
        NoteCCKPValue<Type, BankSize>::begin();
        NoteCCKPLEDBarDriver<NumLEDs>::begin();
        updateDisplay();
    }

    void reset() override {
        NoteCCKPValue<Type, BankSize>::reset();
        updateDisplay();
    }

  protected:
    using NoteCCKPValue<Type, BankSize>::clearDirty;
    using NoteCCKPValue<Type, BankSize>::getDirty;

  protected:
    void onBankSettingChange() override {
        NoteCCKPValue<Type, BankSize>::onBankSettingChange();
        updateDisplay();
    }
};

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

/// Class that listens for **Note** events and displays the velocity on an
/// **LED Bar Graph**.
/// This version can be banked.
/// @tparam BankSize
///         The number of banks.
/// @tparam  NumLEDs
///          The number of LEDs the display has.
/// @ingroup BankableMIDIInputElementsLEDs
template <uint8_t BankSize, uint8_t NumLEDs>
using NoteLEDBar =
    NoteCCKPValueBar<MIDIMessageType::NOTE_ON, BankSize, NumLEDs>;

/// Class that listens for **Control Change** events and displays the
/// value on an **LED Bar Graph**.
/// This version can be banked.
/// @tparam BankSize
///         The number of banks.
/// @tparam  NumLEDs
///          The number of LEDs the display has.
/// @ingroup BankableMIDIInputElementsLEDs
template <uint8_t BankSize, uint8_t NumLEDs>
using CCLEDBar =
    NoteCCKPValueBar<MIDIMessageType::CONTROL_CHANGE, BankSize, NumLEDs>;

/// Class that listens for **Key Pressure** events and displays the pressure on
/// an **LED Bar Graph**.
/// This version can be banked.
/// @tparam BankSize
///         The number of banks.
/// @tparam  NumLEDs
///          The number of LEDs the display has.
/// @ingroup BankableMIDIInputElementsLEDs
template <uint8_t BankSize, uint8_t NumLEDs>
using KPLEDBar =
    NoteCCKPValueBar<MIDIMessageType::KEY_PRESSURE, BankSize, NumLEDs>;

} // namespace Bankable

END_CS_NAMESPACE