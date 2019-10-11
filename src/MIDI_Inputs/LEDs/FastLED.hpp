#ifdef ARDUINO

#include <FastLED.h>
#include <MIDI_Inputs/NoteCCRange.hpp>

BEGIN_CS_NAMESPACE

struct DefaultColorMapper {
    CRGB operator()(uint8_t value) const;
};

template <class ColorMapper>
class NoteCCFastLED {
  public:
    NoteCCFastLED(CRGB *leds, const ColorMapper &colormapper = {})
        : leds(leds), colormapper{colormapper} {}

    void setBrightness(uint8_t brightness) { this->brightness = brightness; }
    uint8_t getBrightness() const { return this->brightness; }

    template <class T>
    void begin(const T &t) {
        update(t);
    }

    template <class T>
    void update(const T &t, uint8_t index) {
        uint8_t value = t.getValue(index);
        leds[index] = colormapper(value).nscale8_video(brightness);
    }

    template <class T>
    void update(const T &t) {
        for (uint8_t index = 0; index < t.length(); ++index) {
            uint8_t value = t.getValue(index);
            leds[index] = colormapper(value).nscale8_video(brightness);
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
using NoteRangeFastLED = GenericNoteCCRange<MIDIInputElementNote,        //
                                            RangeLen,                    //
                                            NoteCCFastLED<ColorMapper>>; //

template <class ColorMapper = DefaultColorMapper>
using NoteValueFastLED = GenericNoteCCRange<MIDIInputElementNote,        //
                                            1,                           //
                                            NoteCCFastLED<ColorMapper>>; //

template <uint8_t RangeLen, class ColorMapper = DefaultColorMapper>
using CCRangeFastLED = GenericNoteCCRange<MIDIInputElementCC,          //
                                          RangeLen,                    //
                                          NoteCCFastLED<ColorMapper>>; //

template <class ColorMapper = DefaultColorMapper>
using CCValueFastLED = GenericNoteCCRange<MIDIInputElementCC,          //
                                          1,                           //
                                          NoteCCFastLED<ColorMapper>>; //

namespace Bankable {

template <uint8_t RangeLen, uint8_t BankSize,
          class ColorMapper = DefaultColorMapper>
using NoteRangeFastLED = GenericNoteCCRange<MIDIInputElementNote,        //
                                            RangeLen,                    //
                                            BankSize,                    //
                                            NoteCCFastLED<ColorMapper>>; //

template <uint8_t BankSize, class ColorMapper = DefaultColorMapper>
using NoteValueFastLED = GenericNoteCCRange<MIDIInputElementNote,        //
                                            1,                           //
                                            BankSize,                    //
                                            NoteCCFastLED<ColorMapper>>; //

template <uint8_t RangeLen, uint8_t BankSize,
          class ColorMapper = DefaultColorMapper>
using CCRangeFastLED = GenericNoteCCRange<MIDIInputElementCC,          //
                                          RangeLen,                    //
                                          BankSize,                    //
                                          NoteCCFastLED<ColorMapper>>; //

template <uint8_t BankSize, class ColorMapper = DefaultColorMapper>
using CCValueFastLED = GenericNoteCCRange<MIDIInputElementCC,          //
                                          1,                           //
                                          BankSize,                    //
                                          NoteCCFastLED<ColorMapper>>; //

} // namespace Bankable

END_CS_NAMESPACE

#endif