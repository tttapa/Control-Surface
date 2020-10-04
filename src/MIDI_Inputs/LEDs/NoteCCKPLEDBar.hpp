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
class NoteCCKPLEDBar : 
    public MatchingMIDIInputElement<Type, TwoByteMIDIMatcher>,
    public NoteCCKPLEDBarDriver<NumLEDs> {
  public:
    using Matcher = TwoByteMIDIMatcher;
    using Parent = MatchingMIDIInputElement<Type, Matcher>;

    NoteCCKPLEDBar(const AH::PinList<NumLEDs> &leds, MIDIAddress address)
        : Parent(address), NoteCCKPLEDBarDriver<NumLEDs>(leds) {}

  protected:
    void handleUpdate(typename Matcher::Result match) override {
        this->displayBar(match.value);
    }

  public:
    void begin() override {
        NoteCCKPLEDBarDriver<NumLEDs>::begin();
        this->displayBar(0);
    }

    void reset() override { this->displayBar(0); }
};

// -------------------------------------------------------------------------- //

/// Class that listens for **Note** events and displays the velocity on an
/// **LED Bar Graph**.
/// @tparam  NumLEDs
///          The number of LEDs the display has.
/// @ingroup midi-input-elements-leds
template <uint8_t NumLEDs>
using NoteLEDBar = NoteCCKPLEDBar<MIDIMessageType::NOTE_ON, NumLEDs>;

/// Class that listens for **Control Change** events and displays the
/// value on an **LED Bar Graph**.
/// @tparam  NumLEDs
///          The number of LEDs the display has.
/// @ingroup midi-input-elements-leds
template <uint8_t NumLEDs>
using CCLEDBar = NoteCCKPLEDBar<MIDIMessageType::CONTROL_CHANGE, NumLEDs>;

/// Class that listens for **Key Pressure** events and displays the pressure on
/// an **LED Bar Graph**.
/// @tparam  NumLEDs
///          The number of LEDs the display has.
/// @ingroup midi-input-elements-leds
template <uint8_t NumLEDs>
using KPLEDBar = NoteCCKPLEDBar<MIDIMessageType::KEY_PRESSURE, NumLEDs>;

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

namespace Bankable {

template <MIDIMessageType Type, uint8_t BankSize, uint8_t NumLEDs>
class NoteCCKPLEDBar : public NoteCCKPValue<Type, BankSize>,
                       public NoteCCKPLEDBarDriver<NumLEDs> {
  public:
    using Parent = NoteCCKPValue<Type, BankSize>;
    using Matcher = typename Parent::Matcher;

    NoteCCKPLEDBar(const AH::PinList<NumLEDs> &leds, MIDIAddress address)
        : Parent(address), NoteCCKPLEDBarDriver<NumLEDs>(
                                                      leds) {}

  protected:
    void handleUpdate(typename Matcher::Result match) override {
        bool newdirty = Parent::handleUpdateImpl(match);
        if (newdirty)
            updateDisplay();
        this->dirty |= newdirty;
    }

    void updateDisplay() {
        this->displayBar(this->getValue());
    }

  public:
    void begin() override {
        Parent::begin();
        NoteCCKPLEDBarDriver<NumLEDs>::begin();
        this->displayBar(0);
    }

    void reset() override {
        Parent::reset();
        this->displayBar(0);
    }

  protected:
    void onBankSettingChange() override {
        Parent::onBankSettingChange();
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
    NoteCCKPLEDBar<MIDIMessageType::NOTE_ON, BankSize, NumLEDs>;

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
    NoteCCKPLEDBar<MIDIMessageType::CONTROL_CHANGE, BankSize, NumLEDs>;

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
    NoteCCKPLEDBar<MIDIMessageType::KEY_PRESSURE, BankSize, NumLEDs>;

} // namespace Bankable

END_CS_NAMESPACE