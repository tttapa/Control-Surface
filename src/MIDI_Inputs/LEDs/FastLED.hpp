#if defined(ARDUINO) && defined(FASTLED_VERSION)
#include <FastLED.h>
#endif

#include <Settings/NamespaceSettings.hpp>
#include <stdint.h>

BEGIN_CS_NAMESPACE

/// A structure for RGB colors.
struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
#ifdef FASTLED_VERSION
    explicit operator CRGB() const { return CRGB{r, g, b}; }
#endif
};

/// The default mapping from a 7-bit MIDI value to an RGB color.
/// This uses the Novation Launchpad mapping.
struct DefaultColorMapper {
    /// Map from a 7-bit MIDI value to an RGB color.
    Color operator()(uint8_t value, uint8_t index) const;
};

END_CS_NAMESPACE

#ifdef FASTLED_VERSION

#include <MIDI_Inputs/NoteCCRange.hpp>

BEGIN_CS_NAMESPACE

/// Function pointer type to permute indices.
using index_permuter_f = uint8_t (*)(uint8_t);

/// Callback for Note or CC range or value input that displays the value to a
/// FastLED strip.
template <class ColorMapper>
class NoteCCFastLEDCallback : public SimpleNoteCCValueCallback {
  public:
    NoteCCFastLEDCallback(CRGB *ledcolors, const ColorMapper &colormapper)
        : ledcolors(ledcolors), colormapper(colormapper) {}

    NoteCCFastLEDCallback(CRGB *ledcolors, const ColorMapper &colormapper,
                          index_permuter_f index_permuter)
        : ledcolors(ledcolors), colormapper(colormapper),
          ledIndexPermuter(index_permuter) {}

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
     * The function should take the (zer-based) MIDI index value as a parameter,
     * and return the corresponding LED index (zero-based).
     * By default, the LED index is the same as the MIDI index.
     */
    void setLEDIndexPermuter(index_permuter_f permuter) {
        this->ledIndexPermuter = permuter ? permuter : identityPermuter;
    }

    // Called once upon initialization.
    void begin(const INoteCCValue &input) override { updateAll(input); }

    // Called each time a MIDI message is received and an LED has to be updated.
    // @param   input
    //          The NoteCCRange or NoteCCValue object this callback belongs to.
    //          This is the object that actually receives and stores the MIDI
    //          values.
    // @param   index
    //          The index of the value that changed. (zero-based)
    void update(const INoteCCValue &input, uint8_t index) override {
        // Get the MIDI value that changed [0, 127]
        uint8_t value = input.getValue(index);
        // Apply the color mapper to convert the value to a color
        CRGB newColor = CRGB(colormapper(value, index));
        // Apply the brightness to the color
        newColor = newColor.nscale8_video(brightness);
        // Map the note index to the LED index
        uint8_t ledIndex = ledIndexPermuter(index);
        // Update the LED color
        ledcolors[ledIndex] = newColor;
    }

  private:
    CRGB *ledcolors;
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
// (MIDI Note, MIDI Control Change) message(s) that display the values of
// these messages using FastLED LEDs or LED strips.
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
 * code.
 * 
 * @tparam  RangeLen 
 *          The length of the range of MIDI note numbers to listen for.
 * @tparam  DefaultColorMapper 
 *          The color mapper that defines how each MIDI value should be mapped
 *          to an RGB color for the LEDs.
 */
template <uint8_t RangeLen, class ColorMapper = DefaultColorMapper>
class NoteRangeFastLED
    : public GenericNoteCCRange<MIDIInputElementNote, RangeLen,
                                NoteCCFastLEDCallback<ColorMapper>> {
  public:
    /**
     * @brief Construct a new NoteRangeFastLED object.
     * 
     * @param   leds 
     *          A buffer of CRGB colors, used by the FastLED library. This class
     *          only writes to the buffer, it's the responsibility of the user
     *          to actually output this buffer to the LEDs in the main loop, 
     *          using the FastLED library.
     * @param   address 
     *          The first address in the range. The entire range is defined by
     *          this address and the `RangeLen` template parameter.  
     *          For example, if `address = 10` and `RangeLen = 4`, the this
     *          object will listen to addresses 10, 11, 12, 13.
     * @param   colormapper 
     *          Optional initialization for the color mapper.
     */
    NoteRangeFastLED(Array<CRGB, RangeLen> &leds, MIDICNChannelAddress address,
                     const ColorMapper &colormapper = {})
        : GenericNoteCCRange<MIDIInputElementNote, RangeLen,
                             NoteCCFastLEDCallback<ColorMapper>>{
              address,
              {leds.data, colormapper},
          } {}

    /**
     * @brief Construct a new NoteRangeFastLED object.
     * 
     * @param   leds 
     *          A buffer of CRGB colors, used by the FastLED library. This class
     *          only writes to the buffer, it's the responsibility of the user
     *          to actually output this buffer to the LEDs in the main loop, 
     *          using the FastLED library.
     * @param   address 
     *          The first address in the range. The entire range is defined by
     *          this address and the `RangeLen` template parameter.  
     *          For example, if `address = 10` and `RangeLen = 4`, the this
     *          object will listen to addresses 10, 11, 12, 13.
     * @param   colormapper 
     *          Optional initialization for the color mapper.
     */
    NoteRangeFastLED(CRGB *leds, MIDICNChannelAddress address,
                     const ColorMapper &colormapper = {})
        : GenericNoteCCRange<MIDIInputElementNote, RangeLen,
                             NoteCCFastLEDCallback<ColorMapper>>{
              address,
              {leds, colormapper},
          } {}

    /// @copydoc    NoteCCFastLEDCallback::setBrightness
    void setBrightness(uint8_t brightness) {
        this->callback.setBrightness(brightness);
    }
    /// @copydoc    NoteCCFastLEDCallback::getBrightness
    uint8_t getBrightness() const { return this->callback.getBrightness(); }
    /// @copydoc    NoteCCFastLEDCallback::setLEDIndexPermuter
    void setLEDIndexPermuter(index_permuter_f permuter) {
        this->callback.setLEDIndexPermuter(permuter);
    }
};

template <class ColorMapper = DefaultColorMapper>
class NoteValueFastLED
    : public GenericNoteCCRange<MIDIInputElementNote, 1,
                                NoteCCFastLEDCallback<ColorMapper>> {
  public:
    NoteValueFastLED(CRGB &led, MIDICNChannelAddress address,
                     const ColorMapper &colormapper = {})
        : GenericNoteCCRange<MIDIInputElementNote, 1,
                             NoteCCFastLEDCallback<ColorMapper>>{
              address,
              {&led, colormapper},
          } {}

    /// @copydoc    NoteCCFastLEDCallback::setBrightness
    void setBrightness(uint8_t brightness) {
        this->callback.setBrightness(brightness);
    }
    /// @copydoc    NoteCCFastLEDCallback::getBrightness
    uint8_t getBrightness() const { return this->callback.getBrightness(); }
};

template <uint8_t RangeLen, class ColorMapper = DefaultColorMapper>
class CCRangeFastLED
    : public GenericNoteCCRange<MIDIInputElementCC, RangeLen,
                                NoteCCFastLEDCallback<ColorMapper>> {

  public:
    CCRangeFastLED(Array<CRGB, RangeLen> &leds, MIDICNChannelAddress address,
                   const ColorMapper &colormapper = {})
        : GenericNoteCCRange<MIDIInputElementCC, RangeLen,
                             NoteCCFastLEDCallback<ColorMapper>>{
              address,
              {leds.data, colormapper},
          } {}

    CCRangeFastLED(CRGB *leds, MIDICNChannelAddress address,
                   const ColorMapper &colormapper = {})
        : GenericNoteCCRange<MIDIInputElementCC, RangeLen,
                             NoteCCFastLEDCallback<ColorMapper>>{
              address,
              {leds, colormapper},
          } {}

    /// @copydoc    NoteCCFastLEDCallback::setBrightness
    void setBrightness(uint8_t brightness) {
        this->callback.setBrightness(brightness);
    }
    /// @copydoc    NoteCCFastLEDCallback::getBrightness
    uint8_t getBrightness() const { return this->callback.getBrightness(); }
    /// @copydoc    NoteCCFastLEDCallback::setLEDIndexPermuter
    void setLEDIndexPermuter(index_permuter_f permuter) {
        this->callback.setLEDIndexPermuter(permuter);
    }
};

template <class ColorMapper = DefaultColorMapper>
class CCValueFastLED
    : public GenericNoteCCRange<MIDIInputElementCC, 1,
                                NoteCCFastLEDCallback<ColorMapper>> {
  public:
    CCValueFastLED(CRGB &led, MIDICNChannelAddress address,
                   const ColorMapper &colormapper = {})
        : GenericNoteCCRange<MIDIInputElementCC, 1,
                             NoteCCFastLEDCallback<ColorMapper>>{
              address,
              {&led, colormapper},
          } {}

    /// @copydoc    NoteCCFastLEDCallback::setBrightness
    void setBrightness(uint8_t brightness) {
        this->callback.setBrightness(brightness);
    }
    /// @copydoc    NoteCCFastLEDCallback::getBrightness
    uint8_t getBrightness() const { return this->callback.getBrightness(); }
};

/// @}

namespace Bankable {

/// @addtogroup midi-input-elements-leds
/// @{

template <uint8_t RangeLen, uint8_t BankSize,
          class ColorMapper = DefaultColorMapper>
class NoteRangeFastLED
    : public GenericNoteCCRange<MIDIInputElementNote, RangeLen, BankSize,
                                NoteCCFastLEDCallback<ColorMapper>> {
  public:
    NoteRangeFastLED(BankConfig<BankSize> config, Array<CRGB, RangeLen> &leds,
                     MIDICNChannelAddress address,
                     const ColorMapper &colormapper = {})
        : GenericNoteCCRange<MIDIInputElementNote, RangeLen, BankSize,
                             NoteCCFastLEDCallback<ColorMapper>>{
              config,
              address,
              {leds.data, colormapper},
          } {}

    NoteRangeFastLED(BankConfig<BankSize> config, CRGB *leds,
                     MIDICNChannelAddress address,
                     const ColorMapper &colormapper = {})
        : GenericNoteCCRange<MIDIInputElementNote, RangeLen, BankSize,
                             NoteCCFastLEDCallback<ColorMapper>>{
              config,
              address,
              {leds, colormapper},
          } {}

    /// @copydoc    NoteCCFastLEDCallback::setBrightness
    void setBrightness(uint8_t brightness) {
        this->callback.setBrightness(brightness);
    }
    /// @copydoc    NoteCCFastLEDCallback::getBrightness
    uint8_t getBrightness() const { return this->callback.getBrightness(); }
    /// @copydoc    NoteCCFastLEDCallback::setLEDIndexPermuter
    void setLEDIndexPermuter(index_permuter_f permuter) {
        this->callback.setLEDIndexPermuter(permuter);
    }
};

template <uint8_t BankSize, class ColorMapper = DefaultColorMapper>
class NoteValueFastLED
    : public GenericNoteCCRange<MIDIInputElementNote, 1, BankSize,
                                NoteCCFastLEDCallback<ColorMapper>> {
  public:
    NoteValueFastLED(BankConfig<BankSize> config, CRGB &led,
                     MIDICNChannelAddress address,
                     const ColorMapper &colormapper = {})
        : GenericNoteCCRange<MIDIInputElementNote, 1, BankSize,
                             NoteCCFastLEDCallback<ColorMapper>>{
              config,
              address,
              {&led, colormapper},
          } {}

    /// @copydoc    NoteCCFastLEDCallback::setBrightness
    void setBrightness(uint8_t brightness) {
        this->callback.setBrightness(brightness);
    }
    /// @copydoc    NoteCCFastLEDCallback::getBrightness
    uint8_t getBrightness() const { return this->callback.getBrightness(); }
};

template <uint8_t RangeLen, uint8_t BankSize,
          class ColorMapper = DefaultColorMapper>
class CCRangeFastLED
    : public GenericNoteCCRange<MIDIInputElementCC, RangeLen, BankSize,
                                NoteCCFastLEDCallback<ColorMapper>> {
  public:
    CCRangeFastLED(BankConfig<BankSize> config, Array<CRGB, RangeLen> &leds,
                   MIDICNChannelAddress address,
                   const ColorMapper &colormapper = {})
        : GenericNoteCCRange<MIDIInputElementCC, RangeLen, BankSize,
                             NoteCCFastLEDCallback<ColorMapper>>{
              config,
              address,
              {leds.data, colormapper},
          } {}

    CCRangeFastLED(BankConfig<BankSize> config, CRGB *leds,
                   MIDICNChannelAddress address,
                   const ColorMapper &colormapper = {})
        : GenericNoteCCRange<MIDIInputElementCC, RangeLen, BankSize,
                             NoteCCFastLEDCallback<ColorMapper>>{
              config,
              address,
              {leds, colormapper},
          } {}

    /// @copydoc    NoteCCFastLEDCallback::setBrightness
    void setBrightness(uint8_t brightness) {
        this->callback.setBrightness(brightness);
    }
    /// @copydoc    NoteCCFastLEDCallback::getBrightness
    uint8_t getBrightness() const { return this->callback.getBrightness(); }
    /// @copydoc    NoteCCFastLEDCallback::setLEDIndexPermuter
    void setLEDIndexPermuter(index_permuter_f permuter) {
        this->callback.setLEDIndexPermuter(permuter);
    }
};

template <uint8_t BankSize, class ColorMapper = DefaultColorMapper>
class CCValueFastLED
    : public GenericNoteCCRange<MIDIInputElementCC, 1, BankSize,
                                NoteCCFastLEDCallback<ColorMapper>> {
  public:
    CCValueFastLED(BankConfig<BankSize> config, CRGB &led,
                   MIDICNChannelAddress address,
                   const ColorMapper &colormapper = {})
        : GenericNoteCCRange<MIDIInputElementCC, 1, BankSize,
                             NoteCCFastLEDCallback<ColorMapper>>{
              config,
              address,
              {&led, colormapper},
          } {}

    /// @copydoc    NoteCCFastLEDCallback::setBrightness
    void setBrightness(uint8_t brightness) {
        this->callback.setBrightness(brightness);
    }
    /// @copydoc    NoteCCFastLEDCallback::getBrightness
    uint8_t getBrightness() const { return this->callback.getBrightness(); }
};

/// @}

} // namespace Bankable

END_CS_NAMESPACE

#endif