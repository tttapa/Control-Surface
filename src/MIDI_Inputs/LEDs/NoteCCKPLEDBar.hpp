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

/// Class that turns on a different number of LEDs depending on the received
/// MIDI velocity, key pressure or Control Change value. Similar to a digital
/// LED VU meter.
/// Can be configured in either bar or dot mode.
template <MIDIMessageType Type, uint8_t NumLEDs>
class NoteCCKPLEDBar
    : public MatchingMIDIInputElement<Type, TwoByteMIDIMatcher>,
      public NoteCCKPLEDBarDriver<NumLEDs> {
  public:
    using Matcher = TwoByteMIDIMatcher;
    using Parent = MatchingMIDIInputElement<Type, Matcher>;

    /// @param  leds
    ///         A list of LED pins.
    /// @param  address
    ///         The MIDI address to listen to.
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
/// **LED Bar Graph**, turning on a different number of LEDs depending on the
/// velocity. Similar to a digital LED VU meter.
/// Can be configured in either bar or dot mode.
/// @tparam  NumLEDs
///          The number of LEDs the display has.
/// @ingroup midi-input-elements-leds
template <uint8_t NumLEDs>
using NoteLEDBar = NoteCCKPLEDBar<MIDIMessageType::NoteOn, NumLEDs>;

/// Class that listens for **Control Change** events and displays the
/// value on an **LED Bar Graph**, turning on a different number of LEDs
/// depending on the value. Similar to a digital LED VU meter.
/// Can be configured in either bar or dot mode.
/// @tparam  NumLEDs
///          The number of LEDs the display has.
/// @ingroup midi-input-elements-leds
template <uint8_t NumLEDs>
using CCLEDBar = NoteCCKPLEDBar<MIDIMessageType::ControlChange, NumLEDs>;

/// Class that listens for **Key Pressure** events and displays the pressure on
/// an **LED Bar Graph**, turning on a different number of LEDs
/// depending on the value. Similar to a digital LED VU meter.
/// Can be configured in either bar or dot mode.
/// @tparam  NumLEDs
///          The number of LEDs the display has.
/// @ingroup midi-input-elements-leds
template <uint8_t NumLEDs>
using KPLEDBar = NoteCCKPLEDBar<MIDIMessageType::KeyPressure, NumLEDs>;

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

namespace Bankable {

/// Class that turns on a different number of LEDs depending on the received
/// MIDI velocity, key pressure or Control Change value. Similar to a digital
/// LED VU meter.
/// Can be configured in either bar or dot mode.
/// This version can be banked.
template <MIDIMessageType Type, uint8_t BankSize, uint8_t NumLEDs>
class NoteCCKPLEDBar : public NoteCCKPValue<Type, BankSize>,
                       public NoteCCKPLEDBarDriver<NumLEDs> {
  public:
    using Parent = NoteCCKPValue<Type, BankSize>;
    using Matcher = typename Parent::Matcher;

    /// @param  config
    ///         The bank configuration to use.
    /// @param  leds
    ///         A list of LED pins.
    /// @param  address
    ///         The MIDI address to listen to.
    NoteCCKPLEDBar(BankConfig<BankSize> config,
                   const AH::PinList<NumLEDs> &leds, MIDIAddress address)
        : Parent(config, address), NoteCCKPLEDBarDriver<NumLEDs>(leds) {}

  protected:
    void handleUpdate(typename Matcher::Result match) override {
        bool newdirty = Parent::handleUpdateImpl(match);
        if (newdirty)
            updateDisplay();
        this->dirty |= newdirty;
    }

    void updateDisplay() { this->displayBar(this->getValue()); }

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
/// **LED Bar Graph**, turning on a different number of LEDs
/// depending on the velocity. Similar to a digital LED VU meter.
/// Can be configured in either bar or dot mode.
/// This version can be banked.
/// @tparam BankSize
///         The number of banks.
/// @tparam  NumLEDs
///          The number of LEDs the display has.
/// @ingroup BankableMIDIInputElementsLEDs
template <uint8_t BankSize, uint8_t NumLEDs>
using NoteLEDBar = NoteCCKPLEDBar<MIDIMessageType::NoteOn, BankSize, NumLEDs>;

/// Class that listens for **Control Change** events and displays the
/// value on an **LED Bar Graph**, turning on a different number of LEDs
/// depending on the value. Similar to a digital LED VU meter.
/// Can be configured in either bar or dot mode.
/// This version can be banked.
/// @tparam BankSize
///         The number of banks.
/// @tparam  NumLEDs
///          The number of LEDs the display has.
/// @ingroup BankableMIDIInputElementsLEDs
template <uint8_t BankSize, uint8_t NumLEDs>
using CCLEDBar =
    NoteCCKPLEDBar<MIDIMessageType::ControlChange, BankSize, NumLEDs>;

/// Class that listens for **Key Pressure** events and displays the pressure on
/// an **LED Bar Graph**, turning on a different number of LEDs
/// depending on the value. Similar to a digital LED VU meter.
/// Can be configured in either bar or dot mode.
/// This version can be banked.
/// @tparam BankSize
///         The number of banks.
/// @tparam  NumLEDs
///          The number of LEDs the display has.
/// @ingroup BankableMIDIInputElementsLEDs
template <uint8_t BankSize, uint8_t NumLEDs>
using KPLEDBar =
    NoteCCKPLEDBar<MIDIMessageType::KeyPressure, BankSize, NumLEDs>;

} // namespace Bankable

END_CS_NAMESPACE