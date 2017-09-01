#include "../Settings/Settings.h"

#include "MIDI_Interface.h"

MIDI_Interface::MIDI_Interface() // Constructor (make this the default MIDI interface)
{
    setDefault();
}

MIDI_Interface *MIDI_Interface::DefaultMIDI_Interface = nullptr;

void MIDI_Interface::setDefault() // Set this MIDI interface as the default interface
{
    DefaultMIDI_Interface = this;
}

MIDI_Interface *MIDI_Interface::getDefault() // static function that returns the default interface (doesn't depend on a particular MIDI interface instance )
{
    return DefaultMIDI_Interface;
}

//---------------------------------------------SENDING-MIDI---------------------------------------------//

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

#ifndef NO_MIDI_INPUT

//---------------------------------------------ADDING-TO-BUFFER---------------------------------------------//

bool MIDI_Interface::refresh() // Virtual function that is implemented by child classes,
{                              // it should take MIDI data from the UART or USB buffer and add it to the MIDI buffer,
    return false;              // returns true as long as there's data to read, returns false if UART/USB buffer is empty or if MIDI buffer is full
}

void MIDI_Interface::startMessage() // start adding a new message to the buffer
{
    writeIndex = messageStartIndex; // if the previous message wasn't finished, overwrite it
}

bool MIDI_Interface::addToMessage(uint8_t data) // add a byte to the current message (and increment write index)
{
    if (!hasSpaceLeft()) // if the buffer is full
        return false;
    buffer[writeIndex] = data; // add the data to the buffer
    incrementWriteIndex();
    return true;
}

bool MIDI_Interface::hasSpaceLeft(size_t bytes) // check if the buffer has at least 'bytes' bytes of free space available
{
#ifdef DEBUG
    bool avail = messages == 0 || mod(readIndex - writeIndex, bufferSize) >= bytes; // if the buffer is empty, or if the write index is at least 'bytes' places before the read index
    if (!avail)
        Serial.println("Buffer full");
    return avail;
#else
    return messages == 0 || mod(readIndex - writeIndex, bufferSize) >= bytes; // if the buffer is empty, or if the write index is at least 'bytes' places before the read index
#endif
}

void MIDI_Interface::finishMessage() // finish the message
{
    messageStartIndex = writeIndex; // messageStartIndex no longer points to the start of this message, but to the next empty position
    messages++;                     // the message is finished, so increment message counter
}

//---------------------------------------------READING-FROM-BUFFER---------------------------------------------//

uint8_t MIDI_Interface::read() // read a byte and move the read index to the next byte
{
    if (messages == 0) // if the buffer is empty
        return 0;

    uint8_t midiByte = buffer[readIndex];
    incrementReadIndex();
    if (messageStartIndex == readIndex) // if the read index points to the same element as the write index after incrementing, then the buffer is empty
        messages = 0;
    else if (isHeader()) // if the next byte is a header byte, the previous message was finished
        messages--;
    return midiByte;
}

uint8_t MIDI_Interface::peek() // read a byte without incrementing the read index
{
    if (messages == 0)
        return 0;
    return buffer[readIndex];
}

uint8_t MIDI_Interface::getNextHeader() // read until the first header byte and return it (if it exists, otherwise, return zero)
{
    if (isHeader()) // if the current byte is a header byte
        return read();
    do // if it's not a header byte
    {
        incrementReadIndex();                                // increment the read index
    } while (!isHeader() && messageStartIndex != readIndex); // until a header byte is reached, or until the buffer is empty

    if (messageStartIndex == readIndex) // if the buffer is empty
    {
        messages = 0;
        return 0;
    }
    else // next header is reached, so previous event was finished
    {
        messages--;
        return read();
    }
}

size_t MIDI_Interface::available() // returns the number of MIDI messages in the buffer
{
    return messages;
}

//---------------------------------------------HELPERS---------------------------------------------//

size_t MIDI_Interface::mod(int a, int b) // modulo operator that works for negative numbers as well
{
    return ((a % b) + b) % b;
}

void MIDI_Interface::incrementWriteIndex()
{
    writeIndex = (writeIndex + 1) % bufferSize;
}

void MIDI_Interface::incrementReadIndex()
{
    readIndex = (readIndex + 1) % bufferSize;
}

bool MIDI_Interface::isHeader() // returns true if the read index points to a MIDI header byte and not to a SysExEnd byte or data byte
{
    return (buffer[readIndex] & (1 << 7)) && buffer[readIndex] != SysExEnd;
}

//===================================================================================================//

#endif // ifndef NO_MIDI_INPUT