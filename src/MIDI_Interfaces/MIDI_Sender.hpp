#pragma once

#include <MIDI_Parsers/MIDI_MessageTypes.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   Statically polymorphic template for classes that send MIDI messages.
 */
template <class Derived>
class MIDI_Sender {
  public:
    /// @name Sending MIDI Channel Voice messages
    /// @{

    /// Send a MIDI %Channel Voice message.
    void send(ChannelMessage message);

    /**
     * @brief   Send a 3-byte MIDI %Channel Voice message.
     *
     * @param   m
     *          MIDI message type. [0x80, 0xE0]
     * @param   c
     *          The MIDI channel. [1, 16]
     * @param   d1
     *          The first data byte. [0, 127]
     * @param   d2
     *          The second data byte. [0, 127]
     * @param   cable
     *          The MIDI Cable Number. [Cable_1, Cable_16]
     */
    void sendChannelMessage(MIDIMessageType m, Channel c, uint8_t d1,
                            uint8_t d2, Cable cable = Cable_1);

    /**
     * @brief   Send a 2-byte MIDI %Channel Voice message.
     *
     * @param   m
     *          MIDI message type. [0x80, 0xE0]
     * @param   c
     *          The MIDI channel. [1, 16]
     * @param   d1
     *          The first data byte. [0, 127]
     * @param   cable
     *          The MIDI Cable Number. [1, 16]
     */
    void sendChannelMessage(MIDIMessageType m, Channel c, uint8_t d1,
                            Cable cable = Cable_1);

    /// Send a MIDI Note On event.
    void sendNoteOn(MIDIAddress address, uint8_t velocity);
    /// Send a MIDI Note Off event.
    void sendNoteOff(MIDIAddress address, uint8_t velocity);
    /// Send a MIDI Key Pressure event.
    void sendKeyPressure(MIDIAddress address, uint8_t pressure);
    /// Send a MIDI Control Change event.
    void sendControlChange(MIDIAddress address, uint8_t value);
    /// Send a MIDI Program Change event.
    void sendProgramChange(MIDIAddress address);
    /// Send a MIDI Program Change event.
    void sendProgramChange(MIDIChannelCable address, uint8_t value);
    /// Send a MIDI %Channel Pressure event.
    void sendChannelPressure(MIDIChannelCable address, uint8_t pressure);
    /// Send a MIDI Pitch Bend event.
    void sendPitchBend(MIDIChannelCable address, uint16_t value);

    /// @}

    /// @name Sending MIDI System Common messages
    /// @{

    /// Send a MIDI System Common message.
    void send(SysCommonMessage message);
    /// Send a MIDI System Common message.
    void sendSysCommon(MIDIMessageType m, Cable cable = Cable_1);
    /// Send a MIDI System Common message.
    void sendSysCommon(MIDIMessageType m, uint8_t data1, Cable cable = Cable_1);
    /// Send a MIDI System Common message.
    void sendSysCommon(MIDIMessageType m, uint8_t data1, uint8_t data2,
                       Cable cable = Cable_1);

    /// Send a MIDI Time Code Quarter Frame.
    void sendMTCQuarterFrame(uint8_t data, Cable cable = Cable_1);
    /// Send a MIDI Time Code Quarter Frame.
    void sendMTCQuarterFrame(uint8_t messageType, uint8_t values,
                             Cable cable = Cable_1);
    /// Send a MIDI Song Position Pointer message.
    void sendSongPositionPointer(uint16_t spp, Cable cable = Cable_1);
    /// Send a MIDI Song Select message.
    void sendSongSelect(uint8_t song, Cable cable = Cable_1);
    /// Send a MIDI Tune Request.
    void sendTuneRequest(Cable cable = Cable_1);

    /// @}

    /// @name Sending MIDI System Exclusive messages
    /// @{

    /// Send a MIDI System Exclusive message.
    void send(SysExMessage message);
    /// Send a MIDI System Exclusive message.
    template <uint16_t N>
    void sendSysEx(const uint8_t (&sysexdata)[N], Cable cable = Cable_1);
    /// Send a MIDI System Exclusive message.
    void sendSysEx(const uint8_t *data, uint16_t length, Cable cable = Cable_1);

    /// @}

    /// @name Sending MIDI Real-Time messages
    /// @{

    /// Send a MIDI Real-Time message.
    void send(RealTimeMessage message);
    /// Send a MIDI Real-Time message.
    void sendRealTime(MIDIMessageType rt, Cable cable = Cable_1);
    /// Send a MIDI Real-Time message.
    void sendRealTime(uint8_t rt, Cable cable = Cable_1);

    /// Send a MIDI Timing Clock message.
    void sendTimingClock(Cable cable = Cable_1);
    /// Send a MIDI Start message.
    void sendStart(Cable cable = Cable_1);
    /// Send a MIDI Continue message.
    void sendContinue(Cable cable = Cable_1);
    /// Send a MIDI Stop message.
    void sendStop(Cable cable = Cable_1);
    /// Send a MIDI Active Sensing message.
    void sendActiveSensing(Cable cable = Cable_1);
    /// Send a MIDI System Reset message.
    void sendSystemReset(Cable cable = Cable_1);

    /// @}

    /// @name Flushing the MIDI send buffer
    /// @{

    /// Causes all buffered messages to be sent immediately.
    /// @note   Doesn't necessarily wait until all data has been sent, it just
    ///         triggers the transmission, so everything will be transmitted as
    ///         soon as possible.
    void sendNow();

    /// @}

    /// @name Deprecated
    /// @{

    /// Send a MIDI Key Pressure event.
    /// @deprecated Use @ref sendKeyPressure(MIDIAddress,uint8_t) instead
    [[deprecated("Use sendKeyPressure() instead")]] void
    sendKP(MIDIAddress address, uint8_t pressure);
    /// Send a MIDI Control Change event.
    /// @deprecated Use @ref sendControlChange(MIDIAddress,uint8_t) instead
    [[deprecated("Use sendControlChange() instead")]] void
    sendCC(MIDIAddress address, uint8_t value);
    /// Send a MIDI Program Change event.
    /// @deprecated Use @ref sendProgramChange(MIDIAddress) instead
    [[deprecated("Use sendProgramChange() instead")]] void
    sendPC(MIDIAddress address);
    /// Send a MIDI Program Change event.
    /// @deprecated Use @ref sendProgramChange(MIDIChannelCable,uint8_t) instead
    [[deprecated("Use sendProgramChange() instead")]] void
    sendPC(MIDIChannelCable address, uint8_t value);
    /// Send a MIDI %Channel Pressure event.
    /// @deprecated Use @ref sendChannelPressure(MIDIChannelCable,uint8_t) instead
    [[deprecated("Use sendChannelPressure() instead")]] void
    sendCP(MIDIChannelCable address, uint8_t pressure);
    /// Send a MIDI Pitch Bend event.
    /// @deprecated Use @ref sendPitchBend(MIDIChannelCable,uint16_t) instead
    [[deprecated("Use sendPitchBend() instead")]] void
    sendPB(MIDIChannelCable address, uint16_t value);

    /// @}
};

END_CS_NAMESPACE

#include "MIDI_Sender.ipp"
