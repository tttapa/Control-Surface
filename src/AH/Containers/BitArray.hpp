/* ✔ */

#pragma once

#include <AH/Settings/Warnings.hpp>

AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

#include <AH/Error/Error.hpp>
#include <AH/Settings/NamespaceSettings.hpp>
#include <stdint.h>

/// @addtogroup AH_Containers
/// @{

BEGIN_AH_NAMESPACE

/**
 * @brief   A class for arrays of bits.
 * 
 * @tparam  N
 *          The number of bits.
 */
template <uint8_t N>
class BitArray {
  public:
    /**
     * @brief   Get the value of the given bit.
     * 
     * @param   bitIndex
     *          The (zero-based) index of the bit to read.
     */
    bool get(uint8_t bitIndex) {
        return buffer[getBufferIndex(bitIndex)] & getBufferMask(bitIndex);
    }

    /**
     * @brief   Set the value of the given bit to 1.
     * 
     * @param   bitIndex
     *          The (zero-based) index of the bit to set.
     */
    void set(uint8_t bitIndex) {
        buffer[getBufferIndex(bitIndex)] |= getBufferMask(bitIndex);
    }

    /**
     * @brief   Clear the value of the given bit to 0.
     * 
     * @param   bitIndex
     *          The (zero-based) index of the bit to clear.
     */
    void clear(uint8_t bitIndex) {
        buffer[getBufferIndex(bitIndex)] &= ~getBufferMask(bitIndex);
    }

    /**
     * @brief   Set the value of the given bit to the given state.
     * 
     * @param   bitIndex
     *          The (zero-based) index of the bit to set.
     * @param   state
     *          The value to set the bit to.
     */
    void set(uint8_t bitIndex, bool state) {
        state ? set(bitIndex) : clear(bitIndex);
    }

    /**
     * @brief   Check the given byte index, and return it if it is within the
     *          bounds of the array, otherwise, throw an error, and return
     *          the last valid index.
     * 
     * @param   byteIndex 
     *          The index to check.
     */
    uint8_t safeIndex(uint8_t byteIndex) const {
        if (byteIndex >= getBufferLength()) {
            ERROR(F("Error: index out of bounds (")
                      << byteIndex << F(", length is ") << getBufferLength()
                      << ')',
                  0xFFFF);
            return getBufferLength() - 1;
        }
        return byteIndex;
    }

    /**
     * @brief   Get the byte at the given index.
     * 
     * This function can be used to quickly access all of the bits, to send 
     * them out to a shift register, for example.
     * 
     * @note    No bounds checking is performed.
     * 
     * @param   byteIndex
     *          The index of the byte within the array. 
     */
    uint8_t getByte(uint8_t byteIndex) const {
        return buffer[byteIndex];
        // return buffer[safeIndex(byteIndex)];
    }

    /**
     * @brief   Get the buffer length in bytes.
     */
    uint8_t getBufferLength() const { return bufferLength; }

  private:
    uint8_t getBufferIndex(uint8_t bitIndex) const {
        return safeIndex(bitIndex / 8);
    }
    uint8_t getBufferBit(uint8_t bitIndex) const { return bitIndex % 8; }
    uint8_t getBufferMask(uint8_t bitIndex) const {
        return 1 << getBufferBit(bitIndex);
    }

    constexpr static uint8_t bufferLength = (uint8_t)((N + 7) / 8);
    uint8_t buffer[bufferLength] = {};
};

END_AH_NAMESPACE

/// @}

AH_DIAGNOSTIC_POP()