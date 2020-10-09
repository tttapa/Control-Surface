#pragma once

#include <MIDI_Parsers/MIDI_MessageTypes.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   Statically polymorphic template for classes that send MIDI messages.
 */
template <class Derived>
class MIDI_Sender {
  public:
    /// @name Sending MIDI
    /// @{

    /**
     * @brief   Send a 3-byte MIDI packet.
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
     *          The MIDI Cable Number. [1, 16]
     */
    void send(MIDIMessageType m, Channel c, uint8_t d1, uint8_t d2,
              Cable cable = CABLE_1);

    /**
     * @brief   Send a 2-byte MIDI packet.
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
    void send(MIDIMessageType m, Channel c, uint8_t d1, Cable cable = CABLE_1);

    /**
     * @brief   Send a 3-byte MIDI packet with cable number.
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
     *          The MIDI Cable Number. [1, 16]
     */
    void sendOnCable(MIDIMessageType m, Channel c, uint8_t d1, uint8_t d2,
                     Cable cable);

    /**
     * @brief   Send a 2-byte MIDI packet with cable number.
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
    void sendOnCable(MIDIMessageType m, Channel c, uint8_t d1, Cable cable);

    /**
     * @brief   Send a single-byte MIDI packet with cable number.
     *
     * @param   rt
     *          The MIDI byte to send.
     * @param   cable
     *          The MIDI Cable Number. [1, 16]
     */
    void sendOnCable(MIDIMessageType rt, Cable cable);

    /// Send a MIDI Note On event.
    void sendNoteOn(MIDIAddress address, uint8_t velocity);
    /// Send a MIDI Note Off event.
    void sendNoteOff(MIDIAddress address, uint8_t velocity);
    /// Send a MIDI Key Pressure event.
    void sendKP(MIDIAddress address, uint8_t pressure);
    /// Send a MIDI Control Change event.
    void sendCC(MIDIAddress address, uint8_t value);
    /// Send a MIDI Program Change event.
    void sendPC(MIDIAddress address);
    /// Send a MIDI Program Change event.
    void sendPC(MIDIChannelCable address, uint8_t value);
    /// Send a MIDI Channel Pressure event.
    void sendCP(MIDIChannelCable address, uint8_t pressure);
    /// Send a MIDI Pitch Bend event.
    void sendPB(MIDIChannelCable address, uint16_t value);
    /// Send a MIDI Channel Message
    void send(ChannelMessage message);
    /// Send a MIDI System Exclusive message.
    void send(SysExMessage message);
    /// Send a MIDI System Exclusive message.
    template <size_t N>
    void sendSysEx(const uint8_t (&sysexdata)[N], Cable cable = CABLE_1) {
        send(SysExMessage{sysexdata, N, cable});
    }
    /// Send a MIDI System Exclusive message.
    void sendSysEx(const uint8_t *data, size_t length) {
        send(SysExMessage{data, length});
    }
    /// Send a MIDI Real-Time message.
    void send(RealTimeMessage message);
    /// Send a single-byte MIDI message.
    void sendRealTime(MIDIMessageType rt, Cable cable = CABLE_1) {
        send(RealTimeMessage{rt, cable});
    }

    /// @}
};

END_CS_NAMESPACE

#include "MIDI_Sender.ipp"
