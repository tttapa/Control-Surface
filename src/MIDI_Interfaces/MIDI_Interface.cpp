#include "../Settings/Settings.h"

#include "MIDI_Interface.h"
#include "../Helpers/StreamOut.h"

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

    //----------------------------------------ADDING-TO-SYSEX-BUFFER------------------------------------------//

#ifndef NO_MIDI_INPUT

void MIDI_Interface::startSysEx() // start adding a new SysEx message to the buffer
{
    SysExLength = 0; // if the previous message wasn't finished, overwrite it
#ifdef DEBUG
    DEBUG << "SysExLength = 0" << endl;
#endif
}

bool MIDI_Interface::addSysExByte(uint8_t data) // add a byte to the current SysEx message
{
    if (!hasSpaceLeft()) // if the buffer is full
        return false;
    SysExBuffer[SysExLength] = data; // add the data to the buffer
    SysExLength++;
#ifdef DEBUG
    DEBUG << "SysEx data byte: " << hex << data << dec << tab << "SysExLength = " << SysExLength << endl; // BUG: data is not printed as hex
#endif
    return true;
}

bool MIDI_Interface::hasSpaceLeft() // check if the buffer has at least 1 byte of free space available
{
#ifdef DEBUG
    bool avail = SysExLength < bufferSize;
    if (!avail)
        Serial.println("Buffer full");
    return avail;
#else
    return SysExLength < bufferSize;
#endif
}

#endif // ifndef NO_MIDI_INPUT