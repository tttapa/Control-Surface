#if defined(ARDUINO) && defined(FASTLED_VERSION)
#include <FastLED.h>
#endif

#include <AH/STL/type_traits> // std::enable_if, std::is_default_constructible
#include <Settings/NamespaceSettings.hpp>
#include <stdint.h>

BEGIN_CS_NAMESPACE

/// A structure for RGB colors.
struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

/// The default mapping from a 7-bit MIDI value to an RGB color, using the
/// Novation Launchpad mapping.
Color velocityToNovationColor(uint8_t value);

END_CS_NAMESPACE

#ifdef FASTLED_VERSION

#include <MIDI_Inputs/MIDIInputElement.hpp>

BEGIN_CS_NAMESPACE

/// The default mapping from a 7-bit MIDI value to an RGB color, using the
/// Novation Launchpad mapping.
struct DefaultColorMapper {
    /// Map from a 7-bit MIDI value to an RGB color, using the Novation
    /// Launchpad mapping.
    CRGB operator()(uint8_t value, uint8_t index) const {
        (void)index;
        Color c = velocityToNovationColor(value);
        return CRGB {c.r, c.g, c.b};
    }
};

/// Function pointer type to permute indices.
using index_permuter_f = uint8_t (*)(uint8_t);

/// Generic base class for classes that listen for MIDI Note, Control Change and
/// Key Pressure events on a range of addresses and turns on the corresponding
/// LED in a FastLED strip with a color that depends both on the index in the
/// range and the value of the incoming MIDI message.
///
/// @tparam Type
///         The type of MIDI messages to listen for:
///         - MIDIMessageType::NOTE_ON
///         - MIDIMessageType::CONTROL_CHANGE
///         - MIDIMessageType::KEY_PRESSURE
/// @tparam RangeLen
///         The length of the range of addresses to listen to.
/// @tparam ColorMapper
///         A callable that maps a 7-bit MIDI value and the index in the range
///         to a FastLED CRGB color, see @ref DefaultColorMapper for an example.
template <MIDIMessageType Type, uint8_t RangeLen, class ColorMapper>
class NoteCCKPRangeFastLED
    : public MatchingMIDIInputElement<Type, TwoByteRangeMIDIMatcher> {
  public:
    using Matcher = TwoByteRangeMIDIMatcher;

    NoteCCKPRangeFastLED(CRGB *ledcolors, MIDIAddress address,
                         const ColorMapper &colormapper)
        : MatchingMIDIInputElement<Type, TwoByteRangeMIDIMatcher>(
              {address, RangeLen}),
          ledcolors(ledcolors), colormapper(colormapper) {}

    NoteCCKPRangeFastLED(AH::Array<CRGB, RangeLen> &ledcolors,
                         MIDIAddress address, const ColorMapper &colormapper)
        : NoteCCKPRangeFastLED(ledcolors.data, address, colormapper) {}

    NoteCCKPRangeFastLED(CRGB *ledcolors, MIDIAddress address,
                         const ColorMapper &colormapper,
                         index_permuter_f index_permuter)
        : MatchingMIDIInputElement<Type, TwoByteRangeMIDIMatcher>(
              {address, RangeLen}),
          ledcolors(ledcolors), colormapper(colormapper),
          ledIndexPermuter(index_permuter) {}

    NoteCCKPRangeFastLED(AH::Array<CRGB, RangeLen> &ledcolors,
                         MIDIAddress address, const ColorMapper &colormapper,
                         index_permuter_f index_permuter)
        : NoteCCKPRangeFastLED(ledcolors.data, address, colormapper,
                               index_permuter) {}

    template <class ColorMapper_ = ColorMapper>
    NoteCCKPRangeFastLED(CRGB *ledcolors, MIDIAddress address,
                         typename std::enable_if<std::is_default_constructible<
                             ColorMapper_>::value>::type * = nullptr)
        : MatchingMIDIInputElement<Type, TwoByteRangeMIDIMatcher>(
              {address, RangeLen}),
          ledcolors(ledcolors) {}

    template <class ColorMapper_ = ColorMapper>
    NoteCCKPRangeFastLED(AH::Array<CRGB, RangeLen> &ledcolors,
                         MIDIAddress address,
                         typename std::enable_if<std::is_default_constructible<
                             ColorMapper_>::value>::type * = nullptr)
        : NoteCCKPRangeFastLED(ledcolors.data, address) {}

    /** 
     * @brief   Set the maximum brightness of the LEDs.
     * @param   brightness
     *          The maximum brightness [0, 255]
     */
    void setBrightness(uint8_t brightness) { this->brightness = brightness; }
    /// Get the maximum brightness of the LEDs.
    uint8_t getBrightness() const { return this->brightness; }

    /**
     * @brief   Change the mapping from the MIDI index to the LED index.
     * 
     * The MIDI index is derived from the note or controller number.
     *
     * The function should take the (zero-based) MIDI index value as a 
     * parameter, and return the corresponding LED index (zero-based).
     * By default, the LED index is the same as the MIDI index.
     */
    void setLEDIndexPermuter(index_permuter_f permuter) {
        this->ledIndexPermuter = permuter ? permuter : identityPermuter;
    }

    void begin() override { resetLEDs(); }

    void handleUpdate(typename Matcher::Result match) override {
        updateLED(match.index, match.value);
    }

    void reset() override { resetLEDs(); }

    void updateLED(uint8_t index, uint8_t value) {
        // Apply the color mapper to convert the value and index to a color
        CRGB newColor = CRGB(colormapper(value, index));
        // Apply the brightness to the color
        newColor = newColor.nscale8_video(brightness);
        // Map the note index to the LED index
        uint8_t ledIndex = ledIndexPermuter(index);
        // Check if the color changed
        dirty |= ledcolors[ledIndex] != newColor;
        // Update the LED color
        ledcolors[ledIndex] = newColor;
    }

    void resetLEDs() {
        for (uint8_t index = 0; index < RangeLen; ++index)
            updateLED(index, 0);
    }

    /// Check if the colors changed since the last time the dirty flag was
    /// cleared. When it's dirty, you should probably call `FastLED.show()` in
    /// the main loop.
    bool getDirty() const { return dirty; }
    /// Clear the dirty flag.
    void clearDirty() { dirty = false; }

  private:
    CRGB *ledcolors;
    bool dirty = true;
    uint8_t brightness = 255;
    index_permuter_f ledIndexPermuter = identityPermuter;

    static uint8_t identityPermuter(uint8_t i) { return i; }

  public:
    ColorMapper colormapper;
};

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //
// Everything below is just definitions of type aliases to make the library
// easier to use.
//
// It defines MIDI elements that listen to (a single, a range of)
// (MIDI Note, MIDI Control Change, MIDI Key Pressure) message(s) that display
// the values of these messages using FastLED LEDs or LED strips.
// An optional color mapper can be supplied that defines the mapping from a
// MIDI value [0, 127] to an RGB color.
// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

/// @addtogroup midi-input-elements-leds
/// @{

/**
 * @brief   MIDI Input Element that listens for MIDI Note messages in a given 
 *          range, and displays their values using a FastLED LED strip.
 * 
 * This class doesn't actually write to the LEDs directly, it writes to a buffer
 * of CRGB values that is sent to the LEDs by the FastLED library in the user
 * code. To know when to update the LEDs, you can use the 
 * @ref NoteCCKPRangeFastLED::getDirty() and 
 * @ref NoteCCKPRangeFastLED::clearDirty() methods.
 * 
 * @tparam  RangeLen 
 *          The length of the range of MIDI note numbers to listen for.
 * @tparam  ColorMapper 
 *          The color mapper that defines how each MIDI velocity value should be
 *          mapped to an RGB color for the LEDs.
 */
template <uint8_t RangeLen, class ColorMapper = DefaultColorMapper>
using NoteRangeFastLED =
    NoteCCKPRangeFastLED<MIDIMessageType::NOTE_ON, RangeLen, ColorMapper>;

/**
 * @brief   MIDI Input Element that listens for MIDI Note messages on a specific
 *          note, and displays its value using a FastLED LED strip.
 * 
 * This class doesn't actually write to the LEDs directly, it writes to a buffer
 * of CRGB values that is sent to the LEDs by the FastLED library in the user
 * code. To know when to update the LEDs, you can use the 
 * @ref NoteCCKPRangeFastLED::getDirty() and 
 * @ref NoteCCKPRangeFastLED::clearDirty() methods.
 * 
 * @tparam  ColorMapper 
 *          The color mapper that defines how each MIDI velocity value should be
 *          mapped to an RGB color for the LEDs.
 */
template <class ColorMapper = DefaultColorMapper>
using NoteValueFastLED =
    NoteCCKPRangeFastLED<MIDIMessageType::NOTE_ON, 1, ColorMapper>;

/**
 * @brief   MIDI Input Element that listens for MIDI Control Change messages in
 *          a given range, and displays their values using a FastLED LED strip.
 * 
 * This class doesn't actually write to the LEDs directly, it writes to a buffer
 * of CRGB values that is sent to the LEDs by the FastLED library in the user
 * code. To know when to update the LEDs, you can use the 
 * @ref NoteCCKPRangeFastLED::getDirty() and 
 * @ref NoteCCKPRangeFastLED::clearDirty() methods.
 * 
 * @tparam  RangeLen 
 *          The length of the range of MIDI note numbers to listen for.
 * @tparam  ColorMapper 
 *          The color mapper that defines how each MIDI control change value 
 *          should be mapped to an RGB color for the LEDs.
 */
template <uint8_t RangeLen, class ColorMapper = DefaultColorMapper>
using CCRangeFastLED = NoteCCKPRangeFastLED<MIDIMessageType::CONTROL_CHANGE,
                                            RangeLen, ColorMapper>;

/**
 * @brief   MIDI Input Element that listens for MIDI Control Change messages on
 *          a specific controller, and displays its value using a FastLED LED
 *          strip.
 * 
 * This class doesn't actually write to the LEDs directly, it writes to a buffer
 * of CRGB values that is sent to the LEDs by the FastLED library in the user
 * code. To know when to update the LEDs, you can use the 
 * @ref NoteCCKPRangeFastLED::getDirty() and 
 * @ref NoteCCKPRangeFastLED::clearDirty() methods.
 * 
 * @tparam  ColorMapper 
 *          The color mapper that defines how each MIDI control change value
 *          should be mapped to an RGB color for the LEDs.
 */
template <class ColorMapper = DefaultColorMapper>
using CCValueFastLED =
    NoteCCKPRangeFastLED<MIDIMessageType::CONTROL_CHANGE, 1, ColorMapper>;

/**
 * @brief   MIDI Input Element that listens for MIDI Key Pressure messages in a
 *          given range, and displays their values using a FastLED LED strip.
 * 
 * This class doesn't actually write to the LEDs directly, it writes to a buffer
 * of CRGB values that is sent to the LEDs by the FastLED library in the user
 * code. To know when to update the LEDs, you can use the 
 * @ref NoteCCKPRangeFastLED::getDirty() and 
 * @ref NoteCCKPRangeFastLED::clearDirty() methods.
 * 
 * @tparam  RangeLen 
 *          The length of the range of MIDI note numbers to listen for.
 * @tparam  ColorMapper 
 *          The color mapper that defines how each MIDI pressure value should be
 *          mapped to an RGB color for the LEDs.
 */
template <uint8_t RangeLen, class ColorMapper = DefaultColorMapper>
using KPRangeFastLED =
    NoteCCKPRangeFastLED<MIDIMessageType::KEY_PRESSURE, RangeLen, ColorMapper>;

/**
 * @brief   MIDI Input Element that listens for MIDI Key Pressure messages on a
 *          specific note, and displays its value using a FastLED LED strip.
 * 
 * This class doesn't actually write to the LEDs directly, it writes to a buffer
 * of CRGB values that is sent to the LEDs by the FastLED library in the user
 * code. To know when to update the LEDs, you can use the 
 * @ref NoteCCKPRangeFastLED::getDirty() and 
 * @ref NoteCCKPRangeFastLED::clearDirty() methods.
 * 
 * @tparam  ColorMapper 
 *          The color mapper that defines how each MIDI pressure value should be
 *          mapped to an RGB color for the LEDs.
 */
template <class ColorMapper = DefaultColorMapper>
using KPValueFastLED =
    NoteCCKPRangeFastLED<MIDIMessageType::KEY_PRESSURE, 1, ColorMapper>;

/// @}

namespace Bankable {

/// The default mapping from a 7-bit MIDI value to an RGB color, using the
/// Novation Launchpad mapping.
struct DefaultColorMapper {
    /// Map from a 7-bit MIDI value to an RGB color, using the Novation
    /// Launchpad mapping.
    CRGB operator()(uint8_t value, uint8_t bankIndex, uint8_t index) const {
        (void)bankIndex;
        (void)index;
        Color c = velocityToNovationColor(value);
        return CRGB {c.r, c.g, c.b};
    }
};

/// Generic base class for classes that listen for MIDI Note, Control Change and
/// Key Pressure events on a range of addresses and turns on the corresponding
/// LED in a FastLED strip with a color that depends both on the active bank,
/// the index in the range, and the value of the incoming MIDI message.
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
/// @tparam ColorMapper
///         A callable that maps a 7-bit MIDI value, the bank index and the
///         index in the range to a FastLED CRGB color, see
///         @ref Bankable::DefaultColorMapper for an example.
template <MIDIMessageType Type, uint8_t BankSize, uint8_t RangeLen,
          class ColorMapper>
class NoteCCKPRangeFastLED : public NoteCCKPRange<Type, BankSize, RangeLen> {
  public:
    using Parent = NoteCCKPRange<Type, BankSize, RangeLen>;
    using Matcher = typename Parent::Matcher;

    NoteCCKPRangeFastLED(BankConfig<BankSize> config, CRGB *ledcolors,
                         MIDIAddress address, const ColorMapper &colormapper)
        : Parent(config, address), ledcolors(ledcolors),
          colormapper(colormapper) {}

    NoteCCKPRangeFastLED(BankConfig<BankSize> config,
                         AH::Array<CRGB, RangeLen> &ledcolors,
                         MIDIAddress address, const ColorMapper &colormapper)
        : NoteCCKPRangeFastLED(config, ledcolors.data, address, colormapper) {}

    NoteCCKPRangeFastLED(BankConfig<BankSize> config, CRGB *ledcolors,
                         MIDIAddress address, const ColorMapper &colormapper,
                         index_permuter_f index_permuter)
        : Parent(config, address), ledcolors(ledcolors),
          colormapper(colormapper), ledIndexPermuter(index_permuter) {}

    NoteCCKPRangeFastLED(BankConfig<BankSize> config,
                         AH::Array<CRGB, RangeLen> &ledcolors,
                         MIDIAddress address, const ColorMapper &colormapper,
                         index_permuter_f index_permuter)
        : NoteCCKPRangeFastLED(config, ledcolors.data, address, colormapper,
                               index_permuter) {}

    template <class ColorMapper_ = ColorMapper>
    NoteCCKPRangeFastLED(BankConfig<BankSize> config, CRGB *ledcolors,
                         MIDIAddress address,
                         typename std::enable_if<std::is_default_constructible<
                             ColorMapper_>::value>::type * = nullptr)
        : Parent(config, address), ledcolors(ledcolors) {}

    template <class ColorMapper_ = ColorMapper>
    NoteCCKPRangeFastLED(BankConfig<BankSize> config,
                         AH::Array<CRGB, RangeLen> &ledcolors,
                         MIDIAddress address,
                         typename std::enable_if<std::is_default_constructible<
                             ColorMapper_>::value>::type * = nullptr)
        : NoteCCKPRangeFastLED(config, ledcolors.data, address) {}

    /** 
     * @brief   Set the maximum brightness of the LEDs.
     * @param   brightness
     *          The maximum brightness [0, 255]
     */
    void setBrightness(uint8_t brightness) { this->brightness = brightness; }
    /// Get the maximum brightness of the LEDs.
    uint8_t getBrightness() const { return this->brightness; }

    /**
     * @brief   Change the mapping from the MIDI index to the LED index.
     * 
     * The MIDI index is derived from the note or controller number.
     *
     * The function should take the (zero-based) MIDI index value as a 
     * parameter, and return the corresponding LED index (zero-based).
     * By default, the LED index is the same as the MIDI index.
     */
    void setLEDIndexPermuter(index_permuter_f permuter) {
        this->ledIndexPermuter = permuter ? permuter : identityPermuter;
    }

    void begin() override { updateLEDs(); }

    void handleUpdate(typename Matcher::Result match) override {
        bool newdirty = Parent::handleUpdateImpl(match);
        if (newdirty)
            updateLED(match.bankIndex, match.index, match.value);
        this->dirty |= newdirty;
    }

    void reset() override {
        Parent::reset();
        updateLEDs();
    }

    void updateLED(uint8_t bankIndex, uint8_t index, uint8_t value) {
        // Apply the color mapper to convert the value and index to a color
        CRGB newColor = CRGB(colormapper(value, bankIndex, index));
        // Apply the brightness to the color
        newColor = newColor.nscale8_video(brightness);
        // Map the note index to the LED index
        uint8_t ledIndex = ledIndexPermuter(index);
        // Update the LED color
        ledcolors[ledIndex] = newColor;
    }

    void updateLEDs() {
        const auto bankIndex = this->getActiveBank();
        for (uint8_t index = 0; index < RangeLen; ++index)
            updateLED(bankIndex, index, this->getValue(bankIndex, index));
    }

  protected:
    void onBankSettingChange() override {
        Parent::onBankSettingChange();
        updateLEDs();
    }

  private:
    CRGB *ledcolors;
    uint8_t brightness = 255;
    index_permuter_f ledIndexPermuter = identityPermuter;

    static uint8_t identityPermuter(uint8_t i) { return i; }

  public:
    ColorMapper colormapper;
};

/// @addtogroup BankableMIDIInputElementsLEDs
/// @{

/**
 * @brief   MIDI Input Element that listens across banks for MIDI Note messages
 *          in a given range, and displays their values using a FastLED LED 
 *          strip.
 * 
 * This class doesn't actually write to the LEDs directly, it writes to a buffer
 * of CRGB values that is sent to the LEDs by the FastLED library in the user
 * code. To know when to update the LEDs, you can use the 
 * @ref Bankable::NoteCCKPRangeFastLED::getDirty() and 
 * @ref Bankable::NoteCCKPRangeFastLED::clearDirty() methods.
 * 
 * @tparam  BankSize
 *          The number of banks.
 * @tparam  RangeLen 
 *          The length of the range of MIDI note numbers to listen for.
 * @tparam  ColorMapper 
 *          The color mapper that defines how each MIDI velocity value should be
 *          mapped to an RGB color for the LEDs.
 */
template <uint8_t BankSize, uint8_t RangeLen,
          class ColorMapper = DefaultColorMapper>
using NoteRangeFastLED = NoteCCKPRangeFastLED<MIDIMessageType::NOTE_ON,
                                              BankSize, RangeLen, ColorMapper>;

/// @see @ref Bankable::NoteRangeFastLED
template <uint8_t BankSize, class ColorMapper = DefaultColorMapper>
using NoteValueFastLED = NoteRangeFastLED<BankSize, 1, ColorMapper>;

/// @see @ref Bankable::NoteCCKPRangeFastLED
template <uint8_t BankSize, uint8_t RangeLen,
          class ColorMapper = DefaultColorMapper>
using CCRangeFastLED = NoteCCKPRangeFastLED<MIDIMessageType::CONTROL_CHANGE,
                                            BankSize, RangeLen, ColorMapper>;

/// @see @ref Bankable::NoteCCKPRangeFastLED
template <uint8_t BankSize, class ColorMapper = DefaultColorMapper>
using CCValueFastLED = CCRangeFastLED<BankSize, 1, ColorMapper>;

/// @see @ref Bankable::NoteCCKPRangeFastLED
template <uint8_t BankSize, uint8_t RangeLen,
          class ColorMapper = DefaultColorMapper>
using KPRangeFastLED = NoteCCKPRangeFastLED<MIDIMessageType::KEY_PRESSURE,
                                            BankSize, RangeLen, ColorMapper>;

/// @see @ref Bankable::NoteCCKPRangeFastLED
template <uint8_t BankSize, class ColorMapper = DefaultColorMapper>
using KPValueFastLED = KPRangeFastLED<BankSize, 1, ColorMapper>;

/// @}

} // namespace Bankable

END_CS_NAMESPACE

#endif