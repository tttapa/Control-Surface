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

//---------------------------------------------DISCARDING-INPUT---------------------------------------------//

bool MIDI_Interface::refresh() // Virtual function that is implemented by child classes,
{                              // it should discard MIDI data from the UART or USB buffer
    return false;              // returns true as long as there's data to read, returns false if UART/USB buffer is empty
}