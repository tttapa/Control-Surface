#pragma once

#include "MIDI_Input_CC.hpp"
#include <Print.h>
#include <string.h>

template <uint8_t LENGTH>
class MCU_7SegmentDisplay : public MIDI_Input_CC, public Printable {
  public:
    /**
    * @brief     Constructor.
    * @todo      Documentation.
    */
    MCU_7SegmentDisplay(uint8_t baseChannel, uint8_t baseAddress)
        : MIDI_Input_CC(baseChannel, baseAddress) {
        memset(text, ' ', LENGTH);
    }

  private:
    /**
     * @brief   Update a character.
     */
    virtual bool updateImpl(const MIDI_message_matcher &midimsg) {
        uint8_t targetAddress = midimsg.data1;
        uint8_t index = targetAddress - getBaseAddress();
        // decimal point → 0x80, no decimal point → 0x00
        uint8_t decimalPt = (midimsg.data2 & 0x40) << 1;
        uint8_t data2 = midimsg.data2 & 0x3F;
        uint8_t character = data2 >= 0x20 ? data2 : data2 + 0x40;
        DEBUGREF(F("\tCharacter @")
                 << getBaseAddress() << ':' << index << '\t' << character
                 << (decimalPt ? '.' : ' '));
        character |= decimalPt;
        text[index] = character;
        return true;
    }

    /**
     * @brief   Check if the address of the incoming MIDI message
     *          matches an address of this element.
     */
    virtual inline bool matchAddress(uint8_t targetAddress) const {
        return getBaseAddress() <= targetAddress &&
               targetAddress < getBaseAddress() + LENGTH;
    }

  public:
    /**
     * @brief   Copy the ASCII text into the given buffer.
     * 
     * @param   buffer
     *          The destination to write the text to.  
     *          Will be null-terminated.  
     *          Should have a size of at least `length`+1 bytes.
     */
    void getText(char *buffer, uint8_t offset = 0,
                 uint8_t length = LENGTH) const {
        uint8_t end = offset + length;
        if (end > LENGTH)
            end = LENGTH;
        for (uint8_t i = offset; i < end; i++)
            buffer[i] = getCharacterAt(i);
        buffer[end] = '\0';
    }

    /**
     * @brief   Get the character at the given index.
     * @todo    Documentation.
     */
    inline char getCharacterAt(uint8_t index) const {
        return text[index] & 0x7F;
    }

    /**
     * @brief   Copy the decimal points into the given buffer.
     * 
     * @param   buffer
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
    inline bool getDecimalPointAt(uint8_t index) const {
        return text[index] & 0x80;
    }

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