#include "MIDI_Interface.hpp"
#include <Def/CRTP.hpp>

BEGIN_CS_NAMESPACE

template <class Derived>
void MIDI_Sender<Derived>::send(MIDIMessageType m, Channel c, uint8_t d1,
                                uint8_t d2, Cable cable) {
    sendOnCable(m, c, d1, d2, cable);
}

template <class Derived>
void MIDI_Sender<Derived>::send(MIDIMessageType m, Channel c, uint8_t d1,
                                Cable cable) {
    sendOnCable(m, c, d1, cable);
}

template <class Derived>
void MIDI_Sender<Derived>::sendOnCable(MIDIMessageType m, Channel c, uint8_t d1,
                                       uint8_t d2, Cable cable) {
    uint8_t mm = static_cast<uint8_t>(m);
    uint8_t cc = c.getRaw();
    mm &= 0xF0;       // bitmask high nibble
    mm |= 0b10000000; // set msb
    d1 &= 0x7F;       // clear msb
    d2 &= 0x7F;       // clear msb
    CRTP(Derived).sendImpl(mm | cc, d1, d2, cable);
}

template <class Derived>
void MIDI_Sender<Derived>::sendOnCable(MIDIMessageType m, Channel c, uint8_t d1,
                                       Cable cable) {
    uint8_t mm = static_cast<uint8_t>(m);
    uint8_t cc = c.getRaw();
    mm &= 0xF0;       // bitmask high nibble
    mm |= 0b10000000; // set msb
    d1 &= 0x7F;       // clear msb
    CRTP(Derived).sendImpl(mm | cc, d1, cable);
}

template <class Derived>
void MIDI_Sender<Derived>::sendOnCable(MIDIMessageType r, Cable cable) {
    uint8_t rr = static_cast<uint8_t>(r);
    rr |= 0b10000000; // set msb
    CRTP(Derived).sendImpl(rr, cable);
}

template <class Derived>
void MIDI_Sender<Derived>::sendNoteOn(MIDIAddress address, uint8_t velocity) {
    if (address)
        sendOnCable(MIDIMessageType::NOTE_ON, address.getChannel(),
                    address.getAddress(), velocity, address.getCableNumber());
}
template <class Derived>
void MIDI_Sender<Derived>::sendNoteOff(MIDIAddress address, uint8_t velocity) {
    if (address)
        sendOnCable(MIDIMessageType::NOTE_OFF, address.getChannel(),
                    address.getAddress(), velocity, address.getCableNumber());
}
template <class Derived>
void MIDI_Sender<Derived>::sendKP(MIDIAddress address, uint8_t pressure) {
    if (address)
        sendOnCable(MIDIMessageType::KEY_PRESSURE, address.getChannel(),
                    address.getAddress(), pressure, address.getCableNumber());
}
template <class Derived>
void MIDI_Sender<Derived>::sendCC(MIDIAddress address, uint8_t value) {
    if (address)
        sendOnCable(MIDIMessageType::CONTROL_CHANGE, address.getChannel(),
                    address.getAddress(), value, address.getCableNumber());
}
template <class Derived>
void MIDI_Sender<Derived>::sendPC(MIDIChannelCable address, uint8_t value) {
    if (address)
        sendOnCable(MIDIMessageType::PROGRAM_CHANGE, address.getChannel(),
                    value, address.getCableNumber());
}
template <class Derived>
void MIDI_Sender<Derived>::sendPC(MIDIAddress address) {
    if (address)
        sendOnCable(MIDIMessageType::PROGRAM_CHANGE, address.getChannel(),
                    address.getAddress(), address.getCableNumber());
}
template <class Derived>
void MIDI_Sender<Derived>::sendCP(MIDIChannelCable address, uint8_t pressure) {
    if (address)
        sendOnCable(MIDIMessageType::CHANNEL_PRESSURE, address.getChannel(),
                    pressure, address.getCableNumber());
}
template <class Derived>
void MIDI_Sender<Derived>::sendPB(MIDIChannelCable address, uint16_t value) {
    if (address)
        sendOnCable(MIDIMessageType::PITCH_BEND, address.getChannel(),
                    value & 0x7F, value >> 7, address.getCableNumber());
}
template <class Derived>
void MIDI_Sender<Derived>::send(SysExMessage message) {
    if (message.length) {
        if (message.length < 2) {
            ERROR(F("Error: invalid SysEx length"), 0x7F7F);
            return;
        }
        CRTP(Derived).sendImpl(message.data, message.length, message.cable);
    }
}

template <class Derived>
void MIDI_Sender<Derived>::send(RealTimeMessage message) {
    CRTP(Derived).sendImpl(message.message, message.cable);
}

template <class Derived>
void MIDI_Sender<Derived>::send(ChannelMessage message) {
    if (message.hasTwoDataBytes())
        CRTP(Derived).sendImpl(message.header, message.data1, message.data2,
                               message.cable);
    else
        CRTP(Derived).sendImpl(message.header, message.data1, message.cable);
}

END_CS_NAMESPACE