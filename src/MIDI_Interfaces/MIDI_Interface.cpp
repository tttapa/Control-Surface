#include "../Settings/Settings.h"

#include "MIDI_Interface.h"
#include "../Helpers/StreamOut.h"

MIDI_Interface::MIDI_Interface(MIDI_Parser &parser) : parser(parser)// Constructor (make this the default MIDI interface)
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

    //----------------------------------------PARSER------------------------------------------//

#ifndef NO_MIDI_INPUT

MIDI_message MIDI_Interface::getChannelMessage() {
    return parser.getChannelMessage();
}

const uint8_t *MIDI_Interface::getSysExBuffer() {
    return parser.getSysExBuffer();
}

size_t MIDI_Interface::getSysExLength() {
    return parser.getSysExLength();
}

#endif // ifndef NO_MIDI_INPUT