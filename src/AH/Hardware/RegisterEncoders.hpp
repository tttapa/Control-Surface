#pragma once

#include <AH/Settings/NamespaceSettings.hpp>
#include <AH/Settings/Warnings.hpp>

AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

AH_DIAGNOSTIC_EXTERNAL_HEADER()
#include <AH/Arduino-Wrapper.h>
AH_DIAGNOSTIC_POP()

#include <AH/Containers/Array.hpp>
#include <AH/STL/limits>
#include <AH/STL/type_traits>

BEGIN_AH_NAMESPACE

/// Lookup table that maps rotary encoder (2-bit gray code) state changes to 
/// position deltas.
constexpr static int8_t RegisterEncodersLUT[16] = {
    0,  // 0 0 0 0
    +1, // 0 0 0 1
    -1, // 0 0 1 0
    +2, // 0 0 1 1
    -1, // 0 1 0 0
    0,  // 0 1 0 1
    -2, // 0 1 1 0
    +1, // 0 1 1 1
    +1, // 1 0 0 0
    -2, // 1 0 0 1
    0,  // 1 0 1 0
    -1, // 1 0 1 1
    +2, // 1 1 0 0
    -1, // 1 1 0 1
    +1, // 1 1 1 0
    0,  // 1 1 1 1
};

/**
 * @brief   Class for keeping track of the position of multiple rotary encoders.
 *
 * @tparam  RegisterType
 *          The type of the GPIO registers.
 * @tparam  NumEnc
 *          The number of encoders.
 * @tparam  EncoderPositionType
 *          The type used for saving the encoder positions. `int32_t` is the
 *          default because this matches the Encoder library. You can use small
 *          unsigned types such as `uint8_t` or `uint16_t` if you're just 
 *          interrested in the deltas.
 * @tparam  InterruptSafe
 *          Make the `update` method safe to use inside of an interrupt. 
 *          It makes the necessary variables `volatile` and disables interrupts
 *          while reading the positions from the main program.
 */
template <class RegisterType, uint8_t NumEnc,
          class EncoderPositionType = int32_t, bool InterruptSafe = false>
class RegisterEncoders {
  private:
    static_assert(std::is_unsigned<RegisterType>::value,
                  "RegisterType should be an unsigned integer type");

    using EncoderPositionStorageType =
        typename std::conditional<InterruptSafe, volatile EncoderPositionType,
                                  EncoderPositionType>::type;

    using StateStorageType =
        typename std::conditional<InterruptSafe, volatile RegisterType,
                                  RegisterType>::type;

    StateStorageType state = std::numeric_limits<RegisterType>::max();
    Array<EncoderPositionStorageType, NumEnc> positions {{}};

  public:
    /// Reset the positions to zero and the state to 0xFF...FF.
    void reset() {
        positions = {{}};
        state = std::numeric_limits<RegisterType>::max();
    }

    /// Reset the positions to zero and the state to the given value.
    void reset(RegisterType resetstate) {
        positions = {{}};
        state = resetstate;
    }

    /**
     * @brief   Update the encoder positions based on the new state.
     * 
     * Can be called from within an ISR, on the condition that @p InterruptSafe
     is set to `true`.
     * 
     * Don't call this function both from the ISR and from your main program,
     * only call it from one of the two.
     *
     * @retval  true
     *          The new state is different from the old state, positions have
     *          been updated.
     * @retval  false
     *          The new state is the same as the old state, nothing updated.
     */
    bool update(RegisterType newstate) {
        RegisterType oldstate = state;

        // If the state didn't change, do nothing
        if (newstate == oldstate)
            return false;

        // Save the new state
        state = newstate;

        // For each encoder, compare the new state of its two pins
        // to the old state. Combine the four states into a 4-bit
        // number and use a lookup table to determine the delta between
        // the two encoder positions.
        for (uint8_t i = 0; i < NumEnc; ++i) {
            // Top two bits are new pin states
            uint8_t change = static_cast<uint8_t>(newstate) & 0b11;
            change <<= 2;
            // Bottom two bits are old pin states
            change |= static_cast<uint8_t>(oldstate) & 0b11;
            auto delta =
                static_cast<EncoderPositionType>(RegisterEncodersLUT[change]);
            if (delta != 0) // small speedup on AVR
                positions[i] += delta;
            oldstate >>= 2;
            newstate >>= 2;
        }
        return true;
    }

    /**
     * @brief   Read the position of the given encoder.
     * 
     * Does not update the state, just reads it from the buffered position.
     * 
     * Don't call this function from within an ISR.
     * 
     * @param   idx
     *          The index of the encoder to read [0, NumEnc - 1].
     */
    EncoderPositionType read(uint8_t idx) const {
        if (InterruptSafe) {
            noInterrupts();
            EncoderPositionType ret = positions[idx];
            interrupts();
            return ret;
        } else {
            return positions[idx];
        }
    }

    /**
     * @brief   Read the position of the given encoder and reset it to zero.
     * 
     * Does not update the state, just reads it from the buffered position.
     * 
     * Don't call this function from within an ISR.
     * 
     * @param   idx
     *          The index of the encoder to read [0, NumEnc - 1].
     */
    EncoderPositionType readAndReset(uint8_t idx) {
        if (InterruptSafe) {
            noInterrupts();
            EncoderPositionType ret = positions[idx];
            positions[idx] = 0;
            interrupts();
            return ret;
        } else {
            EncoderPositionType ret = positions[idx];
            positions[idx] = 0;
            return ret;
        }
    }

    /**
     * @brief   Set the position of the given encoder.
     * 
     * Don't call this function from within an ISR.
     * 
     * @param   idx
     *          The index of the encoder to write [0, NumEnc - 1].
     * @param   pos
     *          The position value to write.
     */
    void write(uint8_t idx, EncoderPositionType pos) {
        if (InterruptSafe) {
            noInterrupts();
            positions[idx] = pos;
            interrupts();
        } else {
            positions[idx] = pos;
        }
    }

    /**
     * @brief   Proxy to access a single encoder of the encoders managed by
     *          RegisterEncoders.
     */
    class Encoder {
      private:
        friend class RegisterEncoders;

        /// A pointer to the position value inside of the RegisterEncoders class.
        EncoderPositionStorageType *position;

        Encoder(EncoderPositionStorageType *position) : position(position) {}

      public:
        /// Read the position of the encoder.
        /// @see RegisterEncoders::read
        EncoderPositionType read() const {
            if (InterruptSafe) {
                noInterrupts();
                EncoderPositionType ret = *position;
                interrupts();
                return ret;
            } else {
                return *position;
            }
        }

        /// Read the position of the encoder and reset it to zero.
        /// @see RegisterEncoders::readAndReset
        EncoderPositionType readAndReset() {
            if (InterruptSafe) {
                noInterrupts();
                EncoderPositionType ret = *position;
                *position = 0;
                interrupts();
                return ret;
            } else {
                EncoderPositionType ret = *position;
                *position = 0;
                return ret;
            }
        }

        /// Set the position of the encoder.
        /// @see RegisterEncoders::write
        void write(EncoderPositionType pos) {
            if (InterruptSafe) {
                noInterrupts();
                *position = pos;
                interrupts();
            } else {
                *position = pos;
            }
        }

#if defined(__GNUC__) && !defined(__clang__) && __GNUC__ < 5
        // begin_if_possible doesn't work on GCC 4.x and earlier, so provide a
        // dummy begin method instead.
        void begin() {}
#endif
    };

    /**
     * @brief   Get a proxy to one of the encoders managed by this object.
     * 
     * @param   index
     *          The index of the encoder to access.
     */
    Encoder operator[](uint8_t index) {
        if (index >= NumEnc)
            index = NumEnc - 1;
        return &positions[index];
    }
};

END_AH_NAMESPACE

AH_DIAGNOSTIC_POP()
