#ifdef ARDUINO

#include "FastLED.hpp"
#include "NovationLaunchpadColorLUT.hpp"

BEGIN_CS_NAMESPACE

CRGB DefaultColorMapper::operator()(uint8_t value) const {
    uint8_t r = pgm_read_byte_near(NovationLaunchpadColorLUT + 3 * value + 2);
    uint8_t g = pgm_read_byte_near(NovationLaunchpadColorLUT + 3 * value + 1);
    uint8_t b = pgm_read_byte_near(NovationLaunchpadColorLUT + 3 * value + 0);
    return {r, g, b};
}

END_CS_NAMESPACE

#endif