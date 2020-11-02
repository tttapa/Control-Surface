#pragma once

#include <AH/STL/algorithm> // std::fill
#include <MIDI_Inputs/MIDIInputElement.hpp>
#include <MIDI_Inputs/MIDIInputElementMatchers.hpp>
#include <Print.h>

BEGIN_CS_NAMESPACE

namespace MCU {

/// Class that receives and saves the text of a Mackie Control Universal
/// 7-segment display like the assignment display and the time display.
///
/// @note   Implements Control Change updates only, not System Exclusive.
///
/// @ingroup MIDIInputElements
template <uint8_t LENGTH>
class SevenSegmentDisplay
    : public MatchingMIDIInputElement<MIDIMessageType::CONTROL_CHANGE,
                                      TwoByteRangeMIDIMatcher>,
      public Printable {
  public:
    using Matcher = TwoByteRangeMIDIMatcher;
    using Parent =
        MatchingMIDIInputElement<MIDIMessageType::CONTROL_CHANGE, Matcher>;

    SevenSegmentDisplay(MIDIAddress address) : Parent({address, LENGTH}) {
        fillWithSpaces();
    }

    void reset() override {
        if (!ignoreReset) {
            fillWithSpaces();
            dirty = true;
        }
    }

  protected:
    /// Update a single character.
    void handleUpdate(typename Matcher::Result match) override {
        uint8_t index = LENGTH - 1 - match.index;
        // MIDI msg: character data → bits 0-5
        // MIDI msg: decimal point → bit 6 set, no decimal point → bit 6 not set
        // text:   decimal point → bit 7 set, no decimal point → bit 7 not set
        uint8_t decimalPt = (match.value & 0x40) << 1;
        uint8_t chardata = match.value & 0x3F;
        uint8_t character = chardata >= 0x20 ? chardata : chardata + 0x40;
        character |= decimalPt;
        dirty |= text[index] != character;
        text[index] = character;
    }

    void fillWithSpaces() { std::fill(std::begin(text), std::end(text), ' '); }

  public:
    /// @name Data access
    /// @{

    /**
     * @brief   Copy the ASCII text into the given buffer.
     *
     * @param[out] buffer
     *          The destination to write the text to.
     *          Will be null-terminated.
     *          Should have a size of at least `length`+1 bytes.
     * @param[in] offset
     *          The offset to start copying from (in the source text, the offset
     *          in the destination buffer is always zero).
     * @param[in] length
     *          The number of characters to copy.
     */
    void getText(char *buffer, uint8_t offset = 0,
                 uint8_t length = LENGTH) const {
        if (offset >= LENGTH)
            offset = LENGTH - 1;
        if (length > LENGTH - offset)
            length = LENGTH - offset;
        for (uint8_t i = 0; i < length; i++)
            buffer[i] = getCharacterAt(i + offset);
        buffer[length] = '\0';
    }

    /**
     * @brief   Get the character at the given index.
     * @todo    Documentation.
     */
    char getCharacterAt(uint8_t index) const { return text[index] & 0x7F; }

    /**
     * @brief   Copy the decimal points into the given buffer.
     *
     * @param[out] buffer
     *          The destination to write the decimal points to.
     *          Should have a size of at least `LENGTH` bytes.
     */
    void getDecimalPoints(bool *buffer) const {
        for (uint8_t i = 0; i < LENGTH; i++)
            buffer[i] = getDecimalPointAt(i);
    }

    /**
     * @brief   Get the decimal point state at the given index.
     * @todo    Documentation.
     */
    bool getDecimalPointAt(uint8_t index) const { return text[index] & 0x80; }

    /// @}

    /// @name Printing
    /// @{

    /// Print out the text of the display to the given Print.
    size_t printTo(Print &printer) const override {
        size_t s = 0;
        for (uint8_t i = 0; i < LENGTH; i++) {
            s += printer.print(getCharacterAt(i));
            if (getDecimalPointAt(i))
                s += printer.print('.');
        }
        return s;
    }

    /// @}

    /// @name   Detecting changes
    /// @{

    /// Check if the value was updated since the last time the dirty flag was
    /// cleared.
    bool getDirty() const { return dirty; }
    /// Clear the dirty flag.
    void clearDirty() { dirty = false; }

    /// @}

  private:
    AH::Array<char, LENGTH> text; ///< Non-ASCII and not null-terminated.
    bool dirty = true;

  public:
    /// Don't reset the state when calling the `reset` method. This is the
    /// default, because in the original MCU, displays don't get reset when a
    /// Reset All Controllers message is received.
    bool ignoreReset = true;
};

} // namespace MCU

END_CS_NAMESPACE