/* ✔ */

#pragma once

#include <AH/Error/Error.hpp>
#include <AH/Settings/NamespaceSettings.hpp>
#include <stdint.h>

BEGIN_AH_NAMESPACE

/// @addtogroup AH_Containers
/// @{

/**
 * @brief   A class for arrays of bits.
 * 
 * @tparam  N
 *          The number of bits.
 */
template <uint16_t N>
class BitArray {
  public:
    /**
     * @brief   Get the value of the given bit.
     * 
     * @param   bitIndex
     *          The (zero-based) index of the bit to read.
     */
    bool get(uint16_t bitIndex) const {
        return buffer[getBufferIndex(bitIndex)] & getBufferMask(bitIndex);
    }

    /**
     * @brief   Set the value of the given bit to 1.
     * 
     * @param   bitIndex
     *          The (zero-based) index of the bit to set.
     */
    void set(uint16_t bitIndex) {
        buffer[getBufferIndex(bitIndex)] |= getBufferMask(bitIndex);
    }

    /**
     * @brief   Clear the value of the given bit to 0.
     * 
     * @param   bitIndex
     *          The (zero-based) index of the bit to clear.
     */
    void clear(uint16_t bitIndex) {
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
    void set(uint16_t bitIndex, bool state) {
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
    uint16_t safeIndex(uint16_t byteIndex) const {
        if (byteIndex >= getBufferLength()) {
            ERROR(F("Error: index out of bounds (")
                      << byteIndex << F(", length is ") << getBufferLength()
                      << ')',
                  0xFFFF);
            return getBufferLength() - 1; // LCOV_EXCL_LINE
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
    const uint8_t &getByte(uint16_t byteIndex) const {
        return buffer[byteIndex];
        // return buffer[safeIndex(byteIndex)];
    }
    /// @copydoc AH::BitArray::getByte(uint16_t) const
    uint8_t &getByte(uint16_t byteIndex) {
        return buffer[byteIndex];
        // return buffer[safeIndex(byteIndex)];
    }

    /**
     * @brief   Set the byte at the given index.
     * 
     * This function can be used to quickly write all of the bits, when reading
     * them in from an I/O expander, for example.
     * 
     * @note    No bounds checking is performed.
     * 
     * @param   byteIndex
     *          The index of the byte within the array. 
     * @param   value
     *          The byte to write.
     */
    void setByte(uint16_t byteIndex, uint8_t value) {
        buffer[byteIndex] = value;
    }

    /**
     * @brief   Get the buffer length in bytes.
     */
    uint16_t getBufferLength() const { return bufferLength; }

  private:
    uint16_t getBufferIndex(uint16_t bitIndex) const {
        return safeIndex(bitIndex / 8);
    }
    uint8_t getBufferBit(uint16_t bitIndex) const { return bitIndex % 8; }
    uint8_t getBufferMask(uint16_t bitIndex) const {
        return 1 << getBufferBit(bitIndex);
    }

    constexpr static uint16_t bufferLength = (uint16_t)((N + 7) / 8);
    uint8_t buffer[bufferLength] = {};
};

/// @}

END_AH_NAMESPACE
