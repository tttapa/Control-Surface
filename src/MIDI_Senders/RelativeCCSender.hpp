#pragma once

#include <Control_Surface/Control_Surface_Class.hpp>

#include <AH/Arduino-Wrapper.h> // for constrain

BEGIN_CS_NAMESPACE

/// The encoding to use for relative control change value.
enum relativeCCmode {
    /**
     * @brief    Encode negative MIDI CC values as 7-bit two's complement.
     *
     * | Encoded  | Value | 
     * |---------:|------:|
     * | 000'0000 |  0    |
     * | 000'0001 | +1    |
     * | 011'1111 | +63   |
     * | 100'0000 | -64   |
     * | 100'0001 | -63   |
     * | 111'1111 | -1    |
     */
    TWOS_COMPLEMENT,
    /** 
     * @brief   Encode negative MIDI CC values by adding a fixed offset of 
     *          @f$ 2^6 = 64 @f$.
     *
     * | Encoded  | Value | 
     * |---------:|------:|
     * | 000'0000 | -64   |
     * | 000'0001 | -63   |
     * | 011'1111 | -1    |
     * | 100'0000 |  0    |
     * | 100'0001 | +1    |
     * | 111'1111 | +63   |
     */
    BINARY_OFFSET,
    /**
     * @brief   Encode negative MIDI CC values by using the most significant bit
     *          as a sign bit, and the six least significant bits as the 
     *          absolute value.
     *
     * | Encoded  | Value | 
     * |---------:|------:|
     * | 000'0000 | +0    |
     * | 000'0001 | +1    |
     * | 011'1111 | +63   |
     * | 100'0000 | -0    |
     * | 100'0001 | -1    |
     * | 111'1111 | -63   |
     */
    SIGN_MAGNITUDE,
    /**
     * @brief   Encode negative MIDI CC values by incrementing the address if 
     *          the number is negative, the MIDI value that's sent is always the
     *          absolute value of the relative delta.
     * 
     * For example, if the base address is 0x10, a delta value of +4 will be 
     * sent as a value of 4 to address 0x10, and a delta value of -8 will be 
     * sent as a value of 8 to address 0x11.
     */
    NEXT_ADDRESS = 4,
    /// First relative mode in Reaper.
    REAPER_RELATIVE_1 = TWOS_COMPLEMENT,
    /// Second relative mode in Reaper.
    REAPER_RELATIVE_2 = BINARY_OFFSET,
    /// Third relative mode in Reaper.
    REAPER_RELATIVE_3 = SIGN_MAGNITUDE,
    /// Relative mode in Tracktion.
    TRACKTION_RELATIVE = TWOS_COMPLEMENT,
    /// Relative mode used by the Mackie Control Universal protocol.
    MACKIE_CONTROL_RELATIVE = SIGN_MAGNITUDE,
    /// Korg KONTROL in Inc/Dec mode 1.
    KORG_KONTROL_INC_DEC_1 = NEXT_ADDRESS,
};

/**
 * @brief   Class that sends relative/incremental MIDI control change messages.
 *          This is often used for rotary encoders.
 * 
 * This requires explicit support from your DAW. You have to select relative
 * MIDI CC mode in the MIDI learn settings.
 * 
 * This class supports multiple different modes, see @ref relativeCCmode.
 * 
 * @ingroup MIDI_Senders
 */
class RelativeCCSender {
  public:
    /// Convert an 8-bit two's complement integer to a 7-bit two's complement
    /// integer.
    static uint8_t toTwosComplement7bit(int8_t value) { return value & 0x7F; }
    /// Convert an 8-bit two's complement integer to a 7-bit integer with a
    /// binary offset of 64. In other words, a value of 0 corresponds to -64,
    /// a value of 64 corresponds to 0, and a value of 127 corresponds to 63.
    static uint8_t toBinaryOffset7bit(int8_t value) { return value + 64; }
    /// Convert an 8-bit two's complement integer to 7-bit sign-magnitude
    /// format.
    static uint8_t toSignedMagnitude7bit(int8_t value) {
        uint8_t mask = value >> 7;
        uint8_t abs = (value + mask) ^ mask;
        uint8_t sign = mask & 0b01000000;
        return (abs & 0b00111111) | sign;
    }
    /// Convert an 8-bit two's complement integer to a 7-bit value to send over
    /// MIDI.
    static uint8_t mapRelativeCC(int8_t value) {
        switch (mode) {
            case TWOS_COMPLEMENT: return toTwosComplement7bit(value);
            case BINARY_OFFSET: return toBinaryOffset7bit(value);
            case SIGN_MAGNITUDE: return toSignedMagnitude7bit(value);
            case NEXT_ADDRESS: return value < 0 ? -value : value;
            default: return 0; // Keeps the compiler happy
        }
    }

    /// Send a relative CC message.
    static void send(long delta, MIDIAddress address) {
        if (delta < 0 && mode == NEXT_ADDRESS)
            address = address + 1;
        while (delta != 0) {
            // Constrain relative movement to +/-15 for
            // Mackie Control Universal compatibility
            long thisDelta = constrain(delta, -15, 15);
            uint8_t msgVal = mapRelativeCC(thisDelta);
            // send a Control Change MIDI event
            Control_Surface.sendControlChange(address, msgVal);
            delta -= thisDelta;
        }
    }

    /// Set the relative CC mode that's used. Requires the same setting on the
    /// receiving end, in your DAW, for example.
    static void setMode(relativeCCmode mode) { RelativeCCSender::mode = mode; }

  private:
    static relativeCCmode mode;
};

END_CS_NAMESPACE