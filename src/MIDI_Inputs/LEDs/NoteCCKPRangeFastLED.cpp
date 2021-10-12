#include "NoteCCKPRangeFastLED.hpp"
#include "NovationLaunchpadColorLUT.hpp"
#include <AH/Arduino-Wrapper.h> // pgm_read_byte_near

BEGIN_CS_NAMESPACE

Color velocityToNovationColor(uint8_t value) {
    value &= 0x7F;
    uint8_t r = pgm_read_byte_near(&NovationLaunchpadColorLUT[value][2]);
    uint8_t g = pgm_read_byte_near(&NovationLaunchpadColorLUT[value][1]);
    uint8_t b = pgm_read_byte_near(&NovationLaunchpadColorLUT[value][0]);
    return Color{r, g, b};
}

END_CS_NAMESPACE
