#pragma once

#include <Helpers/Error.hpp>
#include <stdint.h>

template <uint8_t N>
class BitArray {
  public:
    bool get(uint8_t bitIndex) {
        return buffer[getBufferIndex(bitIndex)] & getBufferMask(bitIndex);
    }

    void set(uint8_t bitIndex) {
        buffer[getBufferIndex(bitIndex)] |= getBufferMask(bitIndex);
    }

    void clear(uint8_t bitIndex) {
        buffer[getBufferIndex(bitIndex)] &= ~getBufferMask(bitIndex);
    }

    void set(uint8_t bitIndex, bool state) {
        if (state)
            set(bitIndex);
        else
            clear(bitIndex);
    }

    uint8_t safeIndex(uint8_t byteIndex) const {
        if (byteIndex >= getBufferLength()) {
            ERROR(F("Error: index out of bounds (")
                      << +byteIndex << F(", length is ") << +getBufferLength()
                      << ')',
                  0xFFFF);
            return getBufferLength() - 1;
        }
        return byteIndex;
    }

    inline uint8_t getByte(uint8_t byteIndex) const {
        return buffer[byteIndex];
        // return buffer[safeIndex(byteIndex)];
    }

    uint8_t getBufferLength() const { return bufferLength; }

  private:
    inline uint8_t getBufferIndex(uint8_t bitIndex) const {
        return safeIndex(bitIndex >> 3); // bitIndex / 8
    }
    inline uint8_t getBufferBit(uint8_t bitIndex) const {
        return bitIndex & 0b111; // bitIndex % 8
    }
    inline uint8_t getBufferMask(uint8_t bitIndex) const {
        return 1 << getBufferBit(bitIndex);
    }

    constexpr static uint8_t bufferLength = (uint8_t)((N + 7) / 8);
    uint8_t buffer[bufferLength] = {};
};