// #include "../Control_Surface.h"
#include "./MIDI_Input_7SegmentDisplay.h"
#include <cstring>

using namespace ExtIO;

// public:

MCU_7SegmentDisplay::MCU_7SegmentDisplay(uint8_t address, uint8_t length)
    : MIDI_Input_Element_CC(address, 1, length, 1), length(length)
{
    textBuffer = (char *)malloc(sizeof(char) * (length + 1));
    memset(textBuffer, ' ', sizeof(char) * length);
    textBuffer[length] = '\0';
}
MCU_7SegmentDisplay::~MCU_7SegmentDisplay()
{
    free(textBuffer);
}
bool MCU_7SegmentDisplay::updateImpl(const MIDI_message_matcher &midimsg)
{
    uint8_t data1 = midimsg.data1;
    uint8_t data2 = midimsg.data2;
    uint8_t charIndex = length - 1 - (data1 - address);
    charIndex = charIndex < length ? charIndex : length - 1;
    bool decimalPt = data2 & 0x40;
    data2 &= 0x3F;
    char character = data2 >= 0x20 ? data2 : data2 + 0x40;
#ifdef DEBUG
    DEBUG << "\tCharacter: " << charIndex << tab << character << (decimalPt ? '.' : ' ') << endl;
#endif
    textBuffer[charIndex] = character;
    print();
    refresh();
    return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------//

// public:

MCU_TimeDisplay::MCU_TimeDisplay() : MCU_7SegmentDisplay(0x40, 0xA) {}

void MCU_TimeDisplay::refresh()
{
    ;
}
void MCU_TimeDisplay::print() // TODO: add support for 5-digit bar counts
{
#ifdef DEBUG
    char barStr[5], beatStr[3], frameStr[4];
    getBars(barStr);
    getBeats(beatStr);
    getFrames(frameStr);
    DEBUG << "Bar: " << barStr << "\tBeat: " << beatStr << "\tFrame: " << frameStr << endl;
#endif
}
void MCU_TimeDisplay::getText(char *buff)
{
    strncpy(buff, &textBuffer[0], 10);
}
void MCU_TimeDisplay::getBars(char *buff)
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
void MCU_TimeDisplay::getBeats(char *buff)
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
void MCU_TimeDisplay::getFrames(char *buff)
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

//----------------------------------------------------------------------------------------------------------------------------------------//

// public:

MCU_AssignmentDisplay::MCU_AssignmentDisplay() : MCU_7SegmentDisplay(0x4A, 0x2) {}
void refresh()
{
    ;
}
void MCU_AssignmentDisplay::print()
{
#ifdef DEBUG
    char str[3];
    getText(str);
    DEBUG << "Assignment: " << str << endl;
#endif
}

void MCU_AssignmentDisplay::getText(char *buff)
{
    strncpy(buff, &textBuffer[0], 3);
}