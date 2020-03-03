#include "MIDI_Interface.hpp"
#include <Def/CRTP.hpp>

BEGIN_CS_NAMESPACE

template <class Derived>
void MIDI_Sender<Derived>::send(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2) {
    CRTP(Derived).sendOnCable(m, c, d1, d2, 0);
}

template <class Derived>
void MIDI_Sender<Derived>::send(uint8_t m, uint8_t c, uint8_t d1) {
    CRTP(Derived).sendOnCable(m, c, d1, 0);
}

template <class Derived>
void MIDI_Sender<Derived>::sendOnCable(uint8_t m, uint8_t c, uint8_t d1,
                                       uint8_t d2, uint8_t cn) {
    c--;             // Channels are zero-based
    m &= 0xF0;       // bitmask high nibble
    m |= 0b10000000; // set msb
    c &= 0x0F;       // bitmask low nibble
    d1 &= 0x7F;      // clear msb
    d2 &= 0x7F;      // clear msb
    cn &= 0x0F;      // bitmask low nibble
    CRTP(Derived).sendImpl(m, c, d1, d2, cn);
}

template <class Derived>
void MIDI_Sender<Derived>::sendOnCable(uint8_t m, uint8_t c, uint8_t d1,
                                       uint8_t cn) {
    c--;             // Channels are zero-based
    m &= 0xF0;       // bitmask high nibble
    m |= 0b10000000; // set msb
    c &= 0x0F;       // bitmask low nibble
    d1 &= 0x7F;      // clear msb
    cn &= 0x0F;      // bitmask low nibble
    CRTP(Derived).sendImpl(m, c, d1, cn);
}

template <class Derived>
void MIDI_Sender<Derived>::sendOnCable(uint8_t r, uint8_t cn) {
    r |= 0b10000000; // set msb
    cn &= 0x0F;      // bitmask low nibble
    CRTP(Derived).sendImpl(r, cn);
}

template <class Derived>
void MIDI_Sender<Derived>::sendNoteOn(MIDIAddress address, uint8_t velocity) {
    if (address)
        CRTP(Derived).sendImpl(NOTE_ON, address.getRawChannel(),
                               address.getAddress(), velocity,
                               address.getCableNumber());
}
template <class Derived>
void MIDI_Sender<Derived>::sendNoteOff(MIDIAddress address, uint8_t velocity) {
    if (address)
        CRTP(Derived).sendImpl(NOTE_OFF, address.getRawChannel(),
                               address.getAddress(), velocity,
                               address.getCableNumber());
}
template <class Derived>
void MIDI_Sender<Derived>::sendKP(MIDIAddress address, uint8_t pressure) {
    if (address)
        CRTP(Derived).sendImpl(KEY_PRESSURE, address.getRawChannel(),
                               address.getAddress(), pressure,
                               address.getCableNumber());
}
template <class Derived>
void MIDI_Sender<Derived>::sendCC(MIDIAddress address, uint8_t value) {
    if (address)
        CRTP(Derived).sendImpl(CC, address.getRawChannel(),
                               address.getAddress(), value,
                               address.getCableNumber());
}
template <class Derived>
void MIDI_Sender<Derived>::sendPC(MIDIChannelCN address, uint8_t value) {
    if (address)
        CRTP(Derived).sendImpl(PROGRAM_CHANGE, address.getRawChannel(), value,
                               address.getCableNumber());
}
template <class Derived>
void MIDI_Sender<Derived>::sendPC(MIDIAddress address) {
    if (address)
        CRTP(Derived).sendImpl(PROGRAM_CHANGE, address.getRawChannel(),
                               address.getAddress(), address.getCableNumber());
}
template <class Derived>
void MIDI_Sender<Derived>::sendCP(MIDIChannelCN address, uint8_t pressure) {
    if (address)
        CRTP(Derived).sendImpl(CHANNEL_PRESSURE, address.getRawChannel(),
                               pressure, address.getCableNumber());
}
template <class Derived>
void MIDI_Sender<Derived>::sendPB(MIDIChannelCN address, uint16_t value) {
    if (address)
        CRTP(Derived).sendImpl(PITCH_BEND, address.getRawChannel(),
                               value & 0x7F, value >> 7,
                               address.getCableNumber());
}
template <class Derived>
void MIDI_Sender<Derived>::send(SysExMessage message) {
    if (message.length) {
        if (message.length < 2) {
            ERROR(F("Error: invalid SysEx length"), 0x7F7F);
            return;
        }
        CRTP(Derived).sendImpl(message.data, message.length, message.CN);
    }
}
template <class Derived>
void MIDI_Sender<Derived>::send(uint8_t rt, uint8_t cn) {
    if (rt) {
        CRTP(Derived).sendImpl(rt, cn);
    }
}

template <class Derived>
void MIDI_Sender<Derived>::send(RealTimeMessage message) {
    send(message.message, message.CN);
}

template <class Derived>
void MIDI_Sender<Derived>::send(ChannelMessage message) {
    uint8_t m = message.header & 0xF0; // message type
    uint8_t c = message.header & 0x0F; // channel
    // TODO: optimize header?
    if (m != PROGRAM_CHANGE && m != CHANNEL_PRESSURE)
        CRTP(Derived).sendImpl(m, c, message.data1, message.data2, message.CN);
    else
        CRTP(Derived).sendImpl(m, c, message.data1, message.CN);
}

END_CS_NAMESPACE