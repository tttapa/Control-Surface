#if defined(ARDUINO) && defined(FASTLED_VERSION)
#include <FastLED.h>
#endif

#include <Settings/NamespaceSettings.hpp>
#include <stdint.h>

BEGIN_CS_NAMESPACE

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
#ifdef FASTLED_VERSION
    explicit operator CRGB() const { return CRGB{r, g, b}; }
#endif
};

struct DefaultColorMapper {
    Color operator()(uint8_t value) const;
};

END_CS_NAMESPACE

#ifdef FASTLED_VERSION

#include <MIDI_Inputs/NoteCCRange.hpp>

BEGIN_CS_NAMESPACE

template <class ColorMapper>
class NoteCCFastLED {
  public:
    NoteCCFastLED(CRGB *leds, const ColorMapper &colormapper)
        : leds(leds), colormapper{colormapper} {}

    /** 
     * @brief   Set the maximum brightness of the LEDs.
     * @param  brightness
     *         The maximum brightness [0, 255]
     */
    void setBrightness(uint8_t brightness) { this->brightness = brightness; }
    /// Get the maximum brightness of the LEDs.
    uint8_t getBrightness() const { return this->brightness; }

    template <class T>
    void begin(const T &t) {
        update(t);
    }

    template <class T>
    void update(const T &t, uint8_t index) {
        uint8_t value = t.getValue(index);
        leds[index] = CRGB{colormapper(value)}.nscale8_video(brightness);
    }

    template <class T>
    void update(const T &t) {
        for (uint8_t index = 0; index < t.length(); ++index) {
            uint8_t value = t.getValue(index);
            leds[index] = CRGB{colormapper(value)}.nscale8_video(brightness);
        }
    }

  private:
    CRGB *leds;
    uint8_t brightness = 255;

  public:
    ColorMapper colormapper;
};

/**
 * @brief   
 * 
 * @tparam  RangeLen 
 * @tparam  DefaultColorMapper 
 * 
 * @ingroup midi-input-elements-leds
 */
template <uint8_t RangeLen, class ColorMapper = DefaultColorMapper>
class NoteRangeFastLED
    : public GenericNoteCCRange<MIDIInputElementNote, RangeLen,
                                NoteCCFastLED<ColorMapper>> {
  public:
#if 0
    NoteRangeFastLED(CRGB (&leds)[RangeLen], MIDICNChannelAddress address,
                     const ColorMapper &colormapper = {})
        : GenericNoteCCRange<MIDIInputElementNote, RangeLen,
                             NoteCCFastLED<ColorMapper>>{
              address,
              {leds, colormapper},
          } {}
#endif

    NoteRangeFastLED(Array<CRGB, RangeLen> &leds, MIDICNChannelAddress address,
                     const ColorMapper &colormapper = {})
        : GenericNoteCCRange<MIDIInputElementNote, RangeLen,
                             NoteCCFastLED<ColorMapper>>{
              address,
              {leds.data, colormapper},
          } {}

    NoteRangeFastLED(CRGB *leds, MIDICNChannelAddress address,
                     const ColorMapper &colormapper = {})
        : GenericNoteCCRange<MIDIInputElementNote, RangeLen,
                             NoteCCFastLED<ColorMapper>>{
              address,
              {leds, colormapper},
          } {}

    /// @copydoc    NoteCCFastLED::setBrightness
    void setBrightness(uint8_t brightness) {
        this->callback.setBrightness(brightness);
    }
    /// @copydoc    NoteCCFastLED::getBrightness
    uint8_t getBrightness() const { return this->callback.getBrightness(); }
};

template <class ColorMapper = DefaultColorMapper>
class NoteValueFastLED : public GenericNoteCCRange<MIDIInputElementNote, 1,
                                                   NoteCCFastLED<ColorMapper>> {
  public:
    NoteValueFastLED(CRGB &led, MIDICNChannelAddress address,
                     const ColorMapper &colormapper = {})
        : GenericNoteCCRange<MIDIInputElementNote, 1,
                             NoteCCFastLED<ColorMapper>>{
              address,
              {&led, colormapper},
          } {}

    /// @copydoc    NoteCCFastLED::setBrightness
    void setBrightness(uint8_t brightness) {
        this->callback.setBrightness(brightness);
    }
    /// @copydoc    NoteCCFastLED::getBrightness
    uint8_t getBrightness() const { return this->callback.getBrightness(); }
};

template <uint8_t RangeLen, class ColorMapper = DefaultColorMapper>
class CCRangeFastLED : public GenericNoteCCRange<MIDIInputElementCC, RangeLen,
                                                 NoteCCFastLED<ColorMapper>> {

  public:
#if 0
    CCRangeFastLED(CRGB (&leds)[RangeLen], MIDICNChannelAddress address,
                   const ColorMapper &colormapper = {})
        : GenericNoteCCRange<MIDIInputElementCC, RangeLen,
                             NoteCCFastLED<ColorMapper>>{
              address,
              {leds, colormapper},
          } {}
#endif

    CCRangeFastLED(Array<CRGB, RangeLen> &leds, MIDICNChannelAddress address,
                   const ColorMapper &colormapper = {})
        : GenericNoteCCRange<MIDIInputElementCC, RangeLen,
                             NoteCCFastLED<ColorMapper>>{
              address,
              {leds.data, colormapper},
          } {}

    CCRangeFastLED(CRGB *leds, MIDICNChannelAddress address,
                   const ColorMapper &colormapper = {})
        : GenericNoteCCRange<MIDIInputElementCC, RangeLen,
                             NoteCCFastLED<ColorMapper>>{
              address,
              {leds, colormapper},
          } {}

    /// @copydoc    NoteCCFastLED::setBrightness
    void setBrightness(uint8_t brightness) {
        this->callback.setBrightness(brightness);
    }
    /// @copydoc    NoteCCFastLED::getBrightness
    uint8_t getBrightness() const { return this->callback.getBrightness(); }
};

template <class ColorMapper = DefaultColorMapper>
class CCValueFastLED : public GenericNoteCCRange<MIDIInputElementCC, 1,
                                                 NoteCCFastLED<ColorMapper>> {
  public:
    CCValueFastLED(CRGB &led, MIDICNChannelAddress address,
                   const ColorMapper &colormapper = {})
        : GenericNoteCCRange<MIDIInputElementCC, 1, NoteCCFastLED<ColorMapper>>{
              address,
              {&led, colormapper},
          } {}

    /// @copydoc    NoteCCFastLED::setBrightness
    void setBrightness(uint8_t brightness) {
        this->callback.setBrightness(brightness);
    }
    /// @copydoc    NoteCCFastLED::getBrightness
    uint8_t getBrightness() const { return this->callback.getBrightness(); }
};

namespace Bankable {

template <uint8_t RangeLen, uint8_t BankSize,
          class ColorMapper = DefaultColorMapper>
class NoteRangeFastLED
    : public GenericNoteCCRange<MIDIInputElementNote, RangeLen, BankSize,
                                NoteCCFastLED<ColorMapper>> {
  public:
#if 0
    NoteRangeFastLED(BankConfig<BankSize> config, CRGB (&leds)[RangeLen],
                     MIDICNChannelAddress address,
                     const ColorMapper &colormapper = {})
        : GenericNoteCCRange<MIDIInputElementNote, RangeLen, BankSize,
                             NoteCCFastLED<ColorMapper>>{
              config,
              address,
              {leds, colormapper},
          } {}
#endif

    NoteRangeFastLED(BankConfig<BankSize> config, Array<CRGB, RangeLen> &leds,
                     MIDICNChannelAddress address,
                     const ColorMapper &colormapper = {})
        : GenericNoteCCRange<MIDIInputElementNote, RangeLen, BankSize,
                             NoteCCFastLED<ColorMapper>>{
              config,
              address,
              {leds.data, colormapper},
          } {}

    NoteRangeFastLED(BankConfig<BankSize> config, CRGB *leds,
                     MIDICNChannelAddress address,
                     const ColorMapper &colormapper = {})
        : GenericNoteCCRange<MIDIInputElementNote, RangeLen, BankSize,
                             NoteCCFastLED<ColorMapper>>{
              config,
              address,
              {leds, colormapper},
          } {}

    /// @copydoc    NoteCCFastLED::setBrightness
    void setBrightness(uint8_t brightness) {
        this->callback.setBrightness(brightness);
    }
    /// @copydoc    NoteCCFastLED::getBrightness
    uint8_t getBrightness() const { return this->callback.getBrightness(); }
};

template <uint8_t BankSize, class ColorMapper = DefaultColorMapper>
class NoteValueFastLED
    : public GenericNoteCCRange<MIDIInputElementNote, 1, BankSize,
                                NoteCCFastLED<ColorMapper>> {
  public:
    NoteValueFastLED(BankConfig<BankSize> config, CRGB &led,
                     MIDICNChannelAddress address,
                     const ColorMapper &colormapper = {})
        : GenericNoteCCRange<MIDIInputElementNote, 1, BankSize,
                             NoteCCFastLED<ColorMapper>>{
              config,
              address,
              {&led, colormapper},
          } {}

    /// @copydoc    NoteCCFastLED::setBrightness
    void setBrightness(uint8_t brightness) {
        this->callback.setBrightness(brightness);
    }
    /// @copydoc    NoteCCFastLED::getBrightness
    uint8_t getBrightness() const { return this->callback.getBrightness(); }
};

template <uint8_t RangeLen, uint8_t BankSize,
          class ColorMapper = DefaultColorMapper>
class CCRangeFastLED
    : public GenericNoteCCRange<MIDIInputElementCC, RangeLen, BankSize,
                                NoteCCFastLED<ColorMapper>> {
  public:
#if 0
    CCRangeFastLED(BankConfig<BankSize> config, CRGB (&leds)[RangeLen],
                   MIDICNChannelAddress address,
                   const ColorMapper &colormapper = {})
        : GenericNoteCCRange<MIDIInputElementCC, RangeLen, BankSize,
                             NoteCCFastLED<ColorMapper>>{
              config,
              address,
              {leds, colormapper},
          } {}
#endif

    CCRangeFastLED(BankConfig<BankSize> config, Array<CRGB, RangeLen> &leds,
                   MIDICNChannelAddress address,
                   const ColorMapper &colormapper = {})
        : GenericNoteCCRange<MIDIInputElementCC, RangeLen, BankSize,
                             NoteCCFastLED<ColorMapper>>{
              config,
              address,
              {leds.data, colormapper},
          } {}

    CCRangeFastLED(BankConfig<BankSize> config, CRGB *leds,
                   MIDICNChannelAddress address,
                   const ColorMapper &colormapper = {})
        : GenericNoteCCRange<MIDIInputElementCC, RangeLen, BankSize,
                             NoteCCFastLED<ColorMapper>>{
              config,
              address,
              {leds, colormapper},
          } {}

    /// @copydoc    NoteCCFastLED::setBrightness
    void setBrightness(uint8_t brightness) {
        this->callback.setBrightness(brightness);
    }
    /// @copydoc    NoteCCFastLED::getBrightness
    uint8_t getBrightness() const { return this->callback.getBrightness(); }
};

template <uint8_t BankSize, class ColorMapper = DefaultColorMapper>
class CCValueFastLED
    : public GenericNoteCCRange<MIDIInputElementCC, 1, BankSize,
                                NoteCCFastLED<ColorMapper>> {
  public:
    CCValueFastLED(BankConfig<BankSize> config, CRGB &led,
                   MIDICNChannelAddress address,
                   const ColorMapper &colormapper = {})
        : GenericNoteCCRange<MIDIInputElementCC, 1, BankSize,
                             NoteCCFastLED<ColorMapper>>{
              config,
              address,
              {&led, colormapper},
          } {}

    /// @copydoc    NoteCCFastLED::setBrightness
    void setBrightness(uint8_t brightness) {
        this->callback.setBrightness(brightness);
    }
    /// @copydoc    NoteCCFastLED::getBrightness
    uint8_t getBrightness() const { return this->callback.getBrightness(); }
};

} // namespace Bankable

END_CS_NAMESPACE

#endif