#include "MIDI_Interface.h"

void MIDI_Interface::begin()
{
    ;
}

bool MIDI_Interface::refresh()
{
    ;
}

void MIDI_Interface::sendImpl(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2)
{
    ;
}

void MIDI_Interface::sendImpl(uint8_t m, uint8_t c, uint8_t d1)
{
    ;
}

void MIDI_Interface::send(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2)
{
    c--; // Channels are zero-based

    m &= 0xF0;       // bitmask high nibble
    m |= 0b10000000; // set msb
    c &= 0xF;        // bitmask low nibble
    d1 &= 0x7F;      // clear msb
    d2 &= 0x7F;      // clear msb

    sendImpl(m, c, d1, d2);
}

void MIDI_Interface::send(uint8_t m, uint8_t c, uint8_t d1)
{
    c--; // Channels are zero-based

    m &= 0xF0;       // bitmask high nibble
    m |= 0b10000000; // set msb
    c &= 0xF;        // bitmask low nibble
    d1 &= 0x7F;      // clear msb

    sendImpl(m, c, d1);
}

uint8_t MIDI_Interface::read()
{
    if (availableMIDIevents == 0)
        return 0;

    uint8_t midiByte = ringbuffer[readIndex];
    incrementReadIndex(1);
    if (writeIndex == readIndex)
        availableMIDIevents = 0;
    else if (isHeader(ringbuffer[readIndex])) // if the next byte is a header byte, the previous event was finished
        availableMIDIevents--;
    return midiByte;
}
uint8_t MIDI_Interface::peek()
{
    if (availableMIDIevents == 0)
        return 0;
    return ringbuffer[readIndex];
}

uint8_t MIDI_Interface::getNextHeader()
{
    if (isHeader(ringbuffer[readIndex]))
        return read();
    while (!isHeader(ringbuffer[readIndex]) && writeIndex != readIndex)
    {
        incrementReadIndex(1);
    }
    if (writeIndex == readIndex) // if the buffer is empty
    {
        availableMIDIevents = 0;
        return 0;
    }
    availableMIDIevents--; // next header is reached, so previous event was finished
    return read();
}

void MIDI_Interface::incrementWriteIndex(size_t incr)
{
    writeIndex = (writeIndex + incr) % bufferSize;
}
void MIDI_Interface::incrementReadIndex(size_t incr)
{
    readIndex = (readIndex + incr) % bufferSize;
}
inline bool MIDI_Interface::isHeader(uint8_t data)
{
    return (data & (1 << 7)) && data != SysExEnd;
}

size_t MIDI_Interface::mod(int a, int b)
{
    return ((a % b) + b) % b;
}

size_t MIDI_Interface::available()
{
    return availableMIDIevents;
}

MIDI_Interface *MIDI_Interface::DefaultMIDI_Interface = nullptr;

void sendMIDI(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2)
{
    if (MIDI_Interface::getDefault() != nullptr)
        MIDI_Interface::getDefault()->send(m, c, d1, d2);
}
void sendMIDI(uint8_t m, uint8_t c, uint8_t d1)
{
    if (MIDI_Interface::getDefault() != nullptr)
        MIDI_Interface::getDefault()->send(m, c, d1);
}
void startMIDI()
{
    if (MIDI_Interface::getDefault() != nullptr)
        MIDI_Interface::getDefault()->begin();
}

bool refreshMIDI()
{
    if (MIDI_Interface::getDefault() != nullptr)
        return MIDI_Interface::getDefault()->refresh();
    return false;
}

size_t availableMIDI()
{
    if (MIDI_Interface::getDefault() != nullptr)
        return MIDI_Interface::getDefault()->available();
    return 0;
}

uint8_t readMIDI()
{
    if (MIDI_Interface::getDefault() != nullptr)
        return MIDI_Interface::getDefault()->read();
    return 0;
}