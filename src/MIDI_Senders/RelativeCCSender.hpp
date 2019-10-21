#pragma once

#include <Control_Surface/Control_Surface_Class.hpp>

#include <Arduino.h> // for constrain

BEGIN_CS_NAMESPACE

enum relativeCCmode { TWOS_COMPLEMENT, BINARY_OFFSET, SIGN_MAGNITUDE };

#define REAPER_RELATIVE_1 TWOS_COMPLEMENT
#define REAPER_RELATIVE_2 BINARY_OFFSET
#define REAPER_RELATIVE_3 SIGN_MAGNITUDE

#define TRACKTION_RELATIVE TWOS_COMPLEMENT

#define MACKIE_CONTROL_RELATIVE SIGN_MAGNITUDE

class RelativeCCSender {
  public:
    /** Convert an 8-bit two's complement integer to a 7-bit two's complement
     *  integer. */
    static uint8_t toTwosComplement7bit(int8_t value) { return value & 0x7F; }
    /** Convert an 8-bit two's complement integer to a 7-bit integer with a
     *  binary offset of 64. In other words, a value of 0 corresponds to -64,
     *  a value of 64 corresponds to 0, and a value of 127 corresponds to 63. */
    static uint8_t toBinaryOffset7bit(int8_t value) { return value + 64; }
    /** Convert an 8-bit two's complement integer to 7-bit sign-magnitude
     *  format. */
    static uint8_t toSignedMagnitude7bit(int8_t value) {
        uint8_t mask = value >> 7;
        uint8_t abs = (value + mask) ^ mask;
        uint8_t sign = mask & 0b01000000;
        return (abs & 0b00111111) | sign;
    }
    /** Convert an 8-bit two's complement integer to a 7-bit value to send over
     *  MIDI. */
    static uint8_t mapRelativeCC(int8_t value) {
        switch (mode) {
            case TWOS_COMPLEMENT: return toTwosComplement7bit(value);
            case BINARY_OFFSET: return toBinaryOffset7bit(value);
            case SIGN_MAGNITUDE: return toSignedMagnitude7bit(value);
            default: return 0; // Keeps the compiler happy
        }
    }

    static void send(long delta, MIDICNChannelAddress address) {
        while (delta != 0) {
            // Constrain relative movement to +/-15 for
            // Mackie Control Universal compatibility
            long thisDelta = constrain(delta, -15, 15);
            uint8_t msgVal = mapRelativeCC(thisDelta);
            // send a Control Change MIDI event
            Control_Surface.MIDI().sendCC(address, msgVal);
            delta -= thisDelta;
        }
    }

    static void setMode(relativeCCmode mode) { RelativeCCSender::mode = mode; }

  private:
    static relativeCCmode mode;
};

END_CS_NAMESPACE