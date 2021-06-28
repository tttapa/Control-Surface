#pragma once

#include <AH/Debug/Debug.hpp>
#include <AH/Math/MinMaxFix.hpp>
#include <MIDI_Inputs/MIDIInputElement.hpp>
#include <string.h> // memcpy

#ifndef ARDUINO
#include <cassert>
#endif

BEGIN_CS_NAMESPACE

using AH::max;
using AH::min;

namespace MCU {

/// Counts the number of instances of the LCD class.
/// If there are multiple LCD objects, we have to update all of them before
/// breaking out of the MIDI input handling loop.
class LCDCounter {
  public:
    LCDCounter() { instances++; }
    ~LCDCounter() { instances--; }

    static uint8_t getInstances() { return instances; }

  private:
    static uint8_t instances;
};
/**
 * @brief   A class that represents the Mackie Control Universal LCD display and
 *          saves the text it receives.
 * 
 * The format of the MIDI message is as follows (hex):
 * | SysEx Start | Data 1 | Data 2 | Data 3 | Data 4 | Data 5 | Data 6 | Data 7 | &emsp;...&emsp; | SysEx End |
 * |:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:---:|:----:|
 * | `F0` | `mm` | `mm` | `mm` | `nn` | `12` | `oo` | `yy` | ... | `F7` |
 * 
 * - `mm` is manufacturer ID (`00 00 66` for Mackie)
 * - `nn` is model number (`10` for Logic Control, `11` for Logic Control XT)
 * - `oo` is offset relative to the first character of the LCD [0x00, 0x6F]
 * - `yy`... is the ASCII text data
 * 
 * The manufacturer ID and model number are ignored by this class.
 * 
 * @ingroup MIDIInputElements
 */
template <uint8_t BufferSize = 112>
class LCD : public MIDIInputElementSysEx, private LCDCounter {
  public:
    /// @param  offset
    ///         The text sent over MIDI is 112 characters long, by changing the
    ///         offset within this text and the length of the text, we can 
    ///         listen to just a section of the text. E.g. `offset = 56` and 
    ///         `BufferSize = 56` will listen to just the second line of the 
    ///         LCD display. The default is `offset = 0` and `BufferSize = 112`,
    ///         i.e. listening to the entire display.
    /// @param  cable
    ///         The MIDI USB cable number to listen for.
    LCD(uint8_t offset = 0, Cable cable = CABLE_1)
        : offset(offset), cable(cable) {
        // Null-terminate the buffer
        buffer[BufferSize] = '\0';
        // Fill the buffer with spaces
        for (uint8_t i = 0; i < BufferSize; i++)
            buffer[i] = ' ';
    }

  protected:
    bool updateWith(SysExMessage midimsg) override {
        // If this message is meant for a different cable than ours, return:
        if (midimsg.getCable() != this->cable)
            return false;

        // We can't handle chunked SysEx data (yet), and it wouldn't make a ton
        // of sense, since the default SysEx buffer size is the same size as the 
        // SysEx message we expect, so it shouldn't arrive in chunks.
        if (!midimsg.isCompleteMessage()) 
            return false;

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

        // If there's no overlap between incoming range and the range that we're
        // listening for, return:
        if (midiOffset >= bufferEnd || this->offset >= midiBufferEnd)
            // If there are other instances, maybe it'll match one of those,
            // otherwise, stop handling this message:
            return getInstances() == 1;

        // Find the ranges that overlap between the text data in the message
        // (src) and the range of characters we're listening for (dst):
        uint8_t srcStart = max(0, this->offset - midiOffset);
        uint8_t dstStart = max(0, midiOffset - this->offset);
        uint8_t length = midiBufferEnd - midiOffset -
                         max(0, this->offset - midiOffset) -
                         max(0, midiBufferEnd - bufferEnd);

        // Copy the interesting part to our buffer:
#ifdef ARDUINO
        memcpy(&buffer[dstStart], &text[srcStart], length);
#else // Tests
        for (uint8_t i = 0; i < length; ++i) {
            buffer[dstStart + i] = text[srcStart + i];
            assert(dstStart + i < BufferSize);
            assert(srcStart + i < midiLength);
        }
#endif

        dirty = true;

        // If this is the only instance, the others don't have to be updated
        // anymore, so we return true to break the loop:
        return getInstances() == 1;
    }

  public:
    /// @name   Data access
    /// @{

    /// Get a pointer to the null-terminated display text.
    const char *getText() const { return buffer.data; }

    /// @}

    /// @name   Detecting changes
    /// @{

    /// Check if the text was updated since the last time the dirty flag was
    /// cleared.
    bool getDirty() const { return dirty; }
    /// Clear the dirty flag.
    void clearDirty() { dirty = false; }

    /// @}

  private:
    Array<char, BufferSize + 1> buffer;
    uint8_t offset;
    Cable cable;
    bool dirty = true;
};

} // namespace MCU

END_CS_NAMESPACE