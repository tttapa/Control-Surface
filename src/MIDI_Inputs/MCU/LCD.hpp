#pragma once

#include <AH/Debug/Debug.hpp>
#include <AH/Math/MinMaxFix.hpp>
#include <MIDI_Inputs/MIDIInputElementSysEx.hpp>
#include <string.h> // memcpy

#ifndef ARDUINO
#include <cassert>
#endif

BEGIN_CS_NAMESPACE

using AH::max;
using AH::min;

namespace MCU {

class LCDCounter {
  public:
    LCDCounter() { instances++; }
    ~LCDCounter() { instances--; }

    static uint8_t getInstances() { return instances; }

  private:
    static uint8_t instances;
};

template <uint8_t BufferSize = 120>
class LCD : public MIDIInputElementSysEx, private LCDCounter {
  public:
    LCD(uint8_t offset = 0, uint8_t CN = 0)
        : MIDIInputElementSysEx{CN}, offset{offset} {
        buffer[BufferSize] = '\0';
        for (uint8_t i = 0; i < BufferSize; i++)
            buffer[i] = ' ';
    }

    const char *getText() const { return &buffer[0]; }

  private:
    bool updateImpl(SysExMessage midimsg) override {
        // Format:
        // F0 mm mm mm nn 12 oo yy... F7
        // mm = manufacturer ID (00 00 66 for Mackie)
        // nn = model number (10 for Logic Control, 11 for Logic Control XT)
        // oo = offset [0x00, 0x6F]
        // yy... = ASCII data
        if (midimsg.data[5] != 0x12)
            return false;

        const uint8_t midiOffset = midimsg.data[6];
        const uint8_t midiLength = midimsg.length - 8;
        const uint8_t *text = midimsg.data + 7;
        const uint8_t midiBufferEnd = midiOffset + midiLength;

        const uint8_t bufferEnd = this->offset + BufferSize;

        // no overlap between incoming range and this range
        if (midiOffset >= bufferEnd || this->offset >= midiBufferEnd)
            return getInstances() == 1;

        uint8_t srcStart = max(0, this->offset - midiOffset);
        uint8_t dstStart = max(0, midiOffset - this->offset);
        uint8_t length = midiBufferEnd - midiOffset -
                         max(0, this->offset - midiOffset) -
                         max(0, midiBufferEnd - bufferEnd);
        // uint8_t length =
        //     BufferSize -                                                   //
        //     max(0, midiOffset - this->offset) -                            //
        //     max(0, BufferSize - midiLength - (midiOffset - this->offset)); //

        DEBUGVAL(this->offset, midiOffset, BufferSize, midiLength, srcStart,
                 dstStart, length);

#ifdef ARDUINO
        memcpy(&buffer[dstStart], &text[srcStart], length);
#else
        for (uint8_t i = 0; i < length; ++i) {
            buffer[dstStart + i] = text[srcStart + i];
            assert(dstStart + i < BufferSize);
            assert(srcStart + i < midiLength);
        }
#endif

        DEBUGFN(getText());

        // If this is the only instance, the others don't have to be updated
        // anymore
        return getInstances() == 1;
    }

    Array<char, BufferSize + 1> buffer;
    uint8_t offset;
};

} // namespace MCU

END_CS_NAMESPACE