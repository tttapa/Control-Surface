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

MIDI_event *MIDI_Interface::read()
{
    if (writeIndex == readIndex)
        return nullptr;
    MIDI_event *msg = &ringbuffer[readIndex];
    readIndex = readIndex < bufferSize - 1 ? readIndex + 1 : 0;
    return msg;
}

size_t MIDI_Interface::mod(size_t a, size_t b)
{
    return ((a % b) + b) % b;
}

size_t MIDI_Interface::available()
{
    return mod((writeIndex - readIndex), bufferSize);
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

MIDI_event *readMIDI()
{
    if (MIDI_Interface::getDefault() != nullptr)
        return MIDI_Interface::getDefault()->read();
    return nullptr;
}