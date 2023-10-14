#include "MIDI_Sender.hpp"
#include <AH/Containers/CRTP.hpp>

BEGIN_CS_NAMESPACE

template <class Derived>
void MIDI_Sender<Derived>::sendChannelMessage(MIDIMessageType m, Channel c,
                                              uint8_t d1, uint8_t d2,
                                              Cable cable) {
    send(ChannelMessage(m, c, d1, d2, cable));
}

template <class Derived>
void MIDI_Sender<Derived>::sendChannelMessage(MIDIMessageType m, Channel c,
                                              uint8_t d1, Cable cable) {
    send(ChannelMessage(m, c, d1, 0, cable));
}

template <class Derived>
void MIDI_Sender<Derived>::sendNoteOn(MIDIAddress address, uint8_t velocity) {
    if (address)
        CRTP(Derived).sendChannelMessageImpl({
            MIDIMessageType::NoteOn,
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
            MIDIMessageType::NoteOff,
            address.getChannel(),
            address.getAddress(),
            uint8_t(velocity & 0x7F),
            address.getCableNumber(),
        });
}
template <class Derived>
void MIDI_Sender<Derived>::sendKeyPressure(MIDIAddress address,
                                           uint8_t pressure) {
    if (address)
        CRTP(Derived).sendChannelMessageImpl({
            MIDIMessageType::KeyPressure,
            address.getChannel(),
            address.getAddress(),
            uint8_t(pressure & 0x7F),
            address.getCableNumber(),
        });
}
template <class Derived>
void MIDI_Sender<Derived>::sendControlChange(MIDIAddress address,
                                             uint8_t value) {
    if (address)
        CRTP(Derived).sendChannelMessageImpl({
            MIDIMessageType::ControlChange,
            address.getChannel(),
            address.getAddress(),
            uint8_t(value & 0x7F),
            address.getCableNumber(),
        });
}
template <class Derived>
void MIDI_Sender<Derived>::sendProgramChange(MIDIChannelCable address,
                                             uint8_t value) {
    if (address)
        CRTP(Derived).sendChannelMessageImpl({
            MIDIMessageType::ProgramChange,
            address.getChannel(),
            uint8_t(value & 0x7F),
            uint8_t(0x00),
            address.getCableNumber(),
        });
}
template <class Derived>
void MIDI_Sender<Derived>::sendProgramChange(MIDIAddress address) {
    if (address)
        CRTP(Derived).sendChannelMessageImpl({
            MIDIMessageType::ProgramChange,
            address.getChannel(),
            address.getAddress(),
            uint8_t(0x00),
            address.getCableNumber(),
        });
}
template <class Derived>
void MIDI_Sender<Derived>::sendChannelPressure(MIDIChannelCable address,
                                               uint8_t pressure) {
    if (address)
        CRTP(Derived).sendChannelMessageImpl({
            MIDIMessageType::ChannelPressure,
            address.getChannel(),
            uint8_t(pressure & 0x7F),
            uint8_t(0x00),
            address.getCableNumber(),
        });
}
template <class Derived>
void MIDI_Sender<Derived>::sendPitchBend(MIDIChannelCable address,
                                         uint16_t value) {
    if (address)
        CRTP(Derived).sendChannelMessageImpl({
            MIDIMessageType::PitchBend,
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
void MIDI_Sender<Derived>::sendSysCommon(MIDIMessageType m, Cable cable) {
    send(SysCommonMessage(m, cable));
}
template <class Derived>
void MIDI_Sender<Derived>::sendSysCommon(MIDIMessageType m, uint8_t data1,
                                         Cable cable) {
    send(SysCommonMessage(m, data1, cable));
}
template <class Derived>
void MIDI_Sender<Derived>::sendSysCommon(MIDIMessageType m, uint8_t data1,
                                         uint8_t data2, Cable cable) {
    send(SysCommonMessage(m, data1, data2, cable));
}

template <class Derived>
void MIDI_Sender<Derived>::sendMTCQuarterFrame(uint8_t data, Cable cable) {
    send(SysCommonMessage(MIDIMessageType::MTCQuarterFrame, data, cable));
}
template <class Derived>
void MIDI_Sender<Derived>::sendMTCQuarterFrame(uint8_t messageType,
                                               uint8_t values, Cable cable) {
    sendMTCQuarterFrame((messageType << 4) | values, cable);
}
template <class Derived>
void MIDI_Sender<Derived>::sendSongPositionPointer(uint16_t spp, Cable cable) {
    SysCommonMessage msg(MIDIMessageType::SongPositionPointer, cable);
    msg.setData14bit(spp);
    send(msg);
}
template <class Derived>
void MIDI_Sender<Derived>::sendSongSelect(uint8_t song, Cable cable) {
    send(SysCommonMessage(MIDIMessageType::SongSelect, song, cable));
}
template <class Derived>
void MIDI_Sender<Derived>::sendTuneRequest(Cable cable) {
    send(SysCommonMessage(MIDIMessageType::TuneRequest, cable));
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
void MIDI_Sender<Derived>::sendTimingClock(Cable cable) {
    sendRealTime(MIDIMessageType::TimingClock, cable);
}
template <class Derived>
void MIDI_Sender<Derived>::sendStart(Cable cable) {
    sendRealTime(MIDIMessageType::Start, cable);
}
template <class Derived>
void MIDI_Sender<Derived>::sendContinue(Cable cable) {
    sendRealTime(MIDIMessageType::Continue, cable);
}
template <class Derived>
void MIDI_Sender<Derived>::sendStop(Cable cable) {
    sendRealTime(MIDIMessageType::Stop, cable);
}
template <class Derived>
void MIDI_Sender<Derived>::sendActiveSensing(Cable cable) {
    sendRealTime(MIDIMessageType::ActiveSensing, cable);
}
template <class Derived>
void MIDI_Sender<Derived>::sendSystemReset(Cable cable) {
    sendRealTime(MIDIMessageType::SystemReset, cable);
}

template <class Derived>
void MIDI_Sender<Derived>::sendNow() {
    CRTP(Derived).sendNowImpl();
}

template <class Derived>
void MIDI_Sender<Derived>::sendKP(MIDIAddress address, uint8_t pressure) {
    sendKeyPressure(address, pressure);
}
template <class Derived>
void MIDI_Sender<Derived>::sendCC(MIDIAddress address, uint8_t value) {
    sendControlChange(address, value);
}
template <class Derived>
void MIDI_Sender<Derived>::sendPC(MIDIAddress address) {
    sendProgramChange(address);
}
template <class Derived>
void MIDI_Sender<Derived>::sendPC(MIDIChannelCable address, uint8_t value) {
    sendProgramChange(address, value);
}
template <class Derived>
void MIDI_Sender<Derived>::sendCP(MIDIChannelCable address, uint8_t pressure) {
    sendChannelPressure(address, pressure);
}
template <class Derived>
void MIDI_Sender<Derived>::sendPB(MIDIChannelCable address, uint16_t value) {
    sendPitchBend(address, value);
}

END_CS_NAMESPACE