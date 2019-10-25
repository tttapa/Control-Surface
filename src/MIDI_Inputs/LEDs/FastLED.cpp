#ifdef ARDUINO

#include "FastLED.hpp"
#include "NovationLaunchpadColorLUT.hpp"

BEGIN_CS_NAMESPACE

Color DefaultColorMapper::operator()(uint8_t value, uint8_t index) const {
    (void)index;
    uint8_t r = pgm_read_byte_near(NovationLaunchpadColorLUT + 3 * value + 2);
    uint8_t g = pgm_read_byte_near(NovationLaunchpadColorLUT + 3 * value + 1);
    uint8_t b = pgm_read_byte_near(NovationLaunchpadColorLUT + 3 * value + 0);
    return {r, g, b};
}

END_CS_NAMESPACE

#ifdef TEST_COMPILE_ALL_HEADERS_SEPARATELY
NoteValueFastLED<>;
NoteRangeFastLED<12>;
CCValueFastLED<>;
CCRangeFastLED<12>;

Bankable::NoteValueFastLED<4>;
Bankable::NoteRangeFastLED<4, 12>;
Bankable::CCValueFastLED<4>;
Bankable::CCRangeFastLED<4, 12>;
#endif

#endif