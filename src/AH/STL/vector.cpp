#if defined(TEENSYDUINO) && !defined(AVR) && TEENSYDUINO < 153
#include "vector"
#include <AH/Error/Error.hpp>

namespace std {
void __throw_bad_alloc() { FATAL_ERROR(F("bad_alloc"), 0x6371); }

void __throw_length_error(char const *e) {
    FATAL_ERROR(F("length_error: ") << e, 0x6372);
}
} // namespace std
#endif