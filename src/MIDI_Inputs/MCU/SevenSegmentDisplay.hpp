#pragma once

#include <MIDI_Inputs/MIDIInputElementCC.hpp>
#include <Print.h>
#include <string.h>

BEGIN_CS_NAMESPACE

namespace MCU {

template <uint8_t LENGTH>
class SevenSegmentDisplay : public MIDIInputElementCC, public Printable {
  public:
    /**
    * @brief     Constructor.
    * @todo      Documentation.
    */
    SevenSegmentDisplay(const MIDICNChannelAddress &address)
        : MIDIInputElementCC{address} {
        fillWithSpaces();
    }

    void fillWithSpaces() {
        for (char &c : text)
            c = ' ';
    }

    void reset() override {
#ifdef SEVENSEG_RESET
        fillWithSpaces();
#endif
    }

  private:
    /**
     * @brief   Update a character.
     */
    virtual bool updateImpl(const ChannelMessageMatcher &midimsg,
                            const MIDICNChannelAddress &target) override {
        uint8_t index = LENGTH - 1 - getRangeIndex(target);
        // MIDI msg: character data → bits 0-5
        // MIDI msg: decimal point → bit 6 set, no decimal point → bit 6 not set
        // text:   decimal point → bit 7 set, no decimal point → bit 7 not set
        uint8_t decimalPt = (midimsg.data2 & 0x40) << 1;
        uint8_t chardata = midimsg.data2 & 0x3F;
        uint8_t character = chardata >= 0x20 ? chardata : chardata + 0x40;
        character |= decimalPt;
        text[index] = character;
        return true;
    }

    /// Check if the address of the incoming MIDI message is within the range
    /// of addresses of this element.
    bool match(const MIDICNChannelAddress &target) const override {
        return MIDICNChannelAddress::matchAddressInRange(target, address,
                                                         LENGTH);
    }

    /// @todo
    uint8_t getRangeIndex(const MIDICNChannelAddress &target) const {
        return target.getAddress() - this->address.getAddress();
    }

  public:
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

    /**
     * @brief   Print out the text of the display to the given Print.
     */
    size_t printTo(Print &printer) const override {
        size_t s = 0;
        for (uint8_t i = 0; i < LENGTH; i++) {
            s += printer.print(getCharacterAt(i));
            if (getDecimalPointAt(i))
                s += printer.print('.');
        }
        return s;
    }

  private:
    char text[LENGTH];
};

} // namespace MCU

END_CS_NAMESPACE