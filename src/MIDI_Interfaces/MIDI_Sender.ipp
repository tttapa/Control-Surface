#include "MIDI_Sender.hpp"
#include <AH/Containers/CRTP.hpp>

BEGIN_CS_NAMESPACE

template <class Derived>
void MIDI_Sender<Derived>::send(MIDIMessageType m, Channel c, uint8_t d1,
                                uint8_t d2, Cable cable) {
    send(ChannelMessage(m, c, d1, d2, cable));
}

template <class Derived>
void MIDI_Sender<Derived>::send(MIDIMessageType m, Channel c, uint8_t d1,
                                Cable cable) {
    send(ChannelMessage(m, c, d1, 0, cable));
}

template <class Derived>
void MIDI_Sender<Derived>::sendNoteOn(MIDIAddress address, uint8_t velocity) {
    if (address)
        CRTP(Derived).sendChannelMessageImpl({
            MIDIMessageType::NOTE_ON,
            address.getChannel(),
            address.getAddress(),
            uint8_t(velocity & 0x7F),
            address.getCableNumber(),
        });
}
template <class Derived>
void MIDI_Sender<Derived>::sendNoteOff(MIDIAddress address, uint8_t velocity) {
    if (address)
        CRTP(Derived).sendChannelMessageImpl({
            MIDIMessageType::NOTE_OFF,
            address.getChannel(),
            address.getAddress(),
            uint8_t(velocity & 0x7F),
            address.getCableNumber(),
        });
}
template <class Derived>
void MIDI_Sender<Derived>::sendKP(MIDIAddress address, uint8_t pressure) {
    if (address)
        CRTP(Derived).sendChannelMessageImpl({
            MIDIMessageType::KEY_PRESSURE,
            address.getChannel(),
            address.getAddress(),
            uint8_t(pressure & 0x7F),
            address.getCableNumber(),
        });
}
template <class Derived>
void MIDI_Sender<Derived>::sendCC(MIDIAddress address, uint8_t value) {
    if (address)
        CRTP(Derived).sendChannelMessageImpl({
            MIDIMessageType::CONTROL_CHANGE,
            address.getChannel(),
            address.getAddress(),
            uint8_t(value & 0x7F),
            address.getCableNumber(),
        });
}
template <class Derived>
void MIDI_Sender<Derived>::sendPC(MIDIChannelCable address, uint8_t value) {
    if (address)
        CRTP(Derived).sendChannelMessageImpl({
            MIDIMessageType::PROGRAM_CHANGE,
            address.getChannel(),
            uint8_t(value & 0x7F),
            uint8_t(0x00),
            address.getCableNumber(),
        });
}
template <class Derived>
void MIDI_Sender<Derived>::sendPC(MIDIAddress address) {
    if (address)
        CRTP(Derived).sendChannelMessageImpl({
            MIDIMessageType::PROGRAM_CHANGE,
            address.getChannel(),
            address.getAddress(),
            uint8_t(0x00),
            address.getCableNumber(),
        });
}
template <class Derived>
void MIDI_Sender<Derived>::sendCP(MIDIChannelCable address, uint8_t pressure) {
    if (address)
        CRTP(Derived).sendChannelMessageImpl({
            MIDIMessageType::CHANNEL_PRESSURE,
            address.getChannel(),
            uint8_t(pressure & 0x7F),
            uint8_t(0x00),
            address.getCableNumber(),
        });
}
template <class Derived>
void MIDI_Sender<Derived>::sendPB(MIDIChannelCable address, uint16_t value) {
    if (address)
        CRTP(Derived).sendChannelMessageImpl({
            MIDIMessageType::PITCH_BEND,
            address.getChannel(),
            uint8_t((value >> 0) & 0x7F),
            uint8_t((value >> 7) & 0x7F),
            address.getCableNumber(),
        });
}
template <class Derived>
void MIDI_Sender<Derived>::send(SysExMessage message) {
    if (message.length > 0)
        CRTP(Derived).sendSysExImpl(message);
}

template <class Derived>
void MIDI_Sender<Derived>::send(RealTimeMessage message) {
    if (message.isValid())
        CRTP(Derived).sendRealTimeImpl(message);
}

template <class Derived>
void MIDI_Sender<Derived>::send(ChannelMessage message) {
    if (message.hasValidChannelMessageHeader()) {
        message.sanitize();
        CRTP(Derived).sendChannelMessageImpl(message);
    }
}

template <class Derived>
void MIDI_Sender<Derived>::send(SysCommonMessage message) {
    if (message.hasValidSystemCommonHeader()) {
        message.sanitize();
        CRTP(Derived).sendSysCommonImpl(message);
    }
}

template <class Derived>
template <uint16_t N>
void MIDI_Sender<Derived>::sendSysEx(const uint8_t (&sysexdata)[N],
                                     Cable cable) {
    send(SysExMessage(sysexdata, N, cable));
}
template <class Derived>
void MIDI_Sender<Derived>::sendSysEx(const uint8_t *data, uint16_t length,
                                     Cable cable) {
    send(SysExMessage(data, length, cable));
}

template <class Derived>
void MIDI_Sender<Derived>::sendRealTime(MIDIMessageType rt, Cable cable) {
    send(RealTimeMessage(rt, cable));
}
template <class Derived>
void MIDI_Sender<Derived>::sendRealTime(uint8_t rt, Cable cable) {
    send(RealTimeMessage(rt, cable));
}

template <class Derived>
void MIDI_Sender<Derived>::sendNow() {
    CRTP(Derived).sendNowImpl();
}

END_CS_NAMESPACE