#ifndef MIDI_Input_7SegmentDisplay_H_
#define MIDI_Input_7SegmentDisplay_H_

#include "MIDI_Input_Element.h"
#include "ExtendedInputOutput/ExtendedInputOutput.h"

using namespace ExtIO;

class MCU_7SegmentDisplay : public MIDI_Input_Element_CC
{
  public:
    MCU_7SegmentDisplay(uint8_t address, uint8_t length)
        : MIDI_Input_Element_CC(address, 1, length, 1), length(length)
    {
        textBuffer = (char *)malloc(sizeof(char) * (length + 1));
        memset(textBuffer, ' ', sizeof(char) * length);
        textBuffer[length] = '\0';
    }
    ~MCU_7SegmentDisplay()
    {
        free(textBuffer);
    }
    bool updateImpl(uint8_t targetAddress, uint8_t targetChannel)
    {
        uint8_t charIndex = length - 1 - (targetAddress - address);
        charIndex = charIndex < length ? charIndex : length - 1;
        uint8_t data2 = MIDI_Interface::getDefault()->read();
        bool decimalPt = data2 & 0x40;
        data2 &= 0x3F;
        char character = data2 >= 0x20 ? data2 : data2 + 0x40;
#ifdef DEBUG
        Serial.printf("\t%d\t%c%c\r\n", charIndex, character, decimalPt ? '.' : '\0');
#endif
        textBuffer[charIndex] = character;
        print();
        refresh();
        return true;
    }
    virtual void print() {}

  protected:
    const size_t length;
    char *textBuffer = nullptr;
};

class MCU_TimeDisplay : public MCU_7SegmentDisplay
{
  public:
    MCU_TimeDisplay() : MCU_7SegmentDisplay(0x40, 0xA) {}
    void refresh()
    {
        ;
    }
    void print()
    {
#ifdef DEBUG
        char barStr[5], beatStr[3], frameStr[4];
        getBars(barStr);
        getBeats(beatStr);
        getFrames(frameStr);
        Serial.printf("Bar: %s\tBeat: %s\tFrame: %s\r\n", barStr, beatStr, frameStr);
#endif
    }
    void getBars(char *buff)
    {
        if (textBuffer[5] == ' ')
        {
            strncpy(buff, &textBuffer[0], 3);
            buff[3] = '\0';
        }
        else
        {
            strncpy(buff, &textBuffer[0], 4);
            buff[4] = '\0';
        }
    }
    void getBeats(char *buff)
    {
        if (textBuffer[5] == ' ')
        {
            strncpy(buff, &textBuffer[3], 2);
        }
        else
        {
            strncpy(buff, &textBuffer[4], 2);
        }
        buff[2] = '\0';
    }
    void getFrames(char *buff)
    {
        if (textBuffer[5] == ' ')
        {
            strncpy(buff, &textBuffer[7], 4);
        }
        else
        {
            strncpy(buff, &textBuffer[8], 3);
        }
    }
};

class MCU_AssignmentDisplay : public MCU_7SegmentDisplay
{
  public:
    MCU_AssignmentDisplay() : MCU_7SegmentDisplay(0x4A, 0x2) {}
    void refresh()
    {
        ;
    }
    void print()
    {
#ifdef DEBUG
        char str[3];
        getText(str);
        Serial.printf("Assignment: %s\r\n", str);
#endif
    }

    void getText(char *buff)
    {
        strncpy(buff, &textBuffer[0], 3);
    }
};
#endif // MIDI_Input_7SegmentDisplay_H_