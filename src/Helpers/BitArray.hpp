#pragma once

#include <stdint.h>

class BitArray {
  public:
    BitArray(uint8_t size) { buffer = new uint8_t[(size + 7) / 8](); }

    ~BitArray() { delete[] buffer; }

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

  private:
    inline uint8_t getBufferIndex(uint8_t bitIndex) { return bitIndex >> 3; }
    inline uint8_t getBufferBit(uint8_t bitIndex) { return bitIndex & 0b111; }
    inline uint8_t getBufferMask(uint8_t bitIndex) {
        return 1 << getBufferBit(bitIndex);
    }

    uint8_t *buffer = nullptr;
};