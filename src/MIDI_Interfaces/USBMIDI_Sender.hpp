#pragma once

#include <MIDI_Parsers/MIDI_MessageTypes.hpp>

AH_DIAGNOSTIC_WERROR()

BEGIN_CS_NAMESPACE

/**
 * @brief   A class for sending MIDI USB messages. Includes support for chunked
 *          MIDI USB SysEx messages.
 */
class USBMIDI_Sender {
  public:
    /// Send a MIDI Channel message using the given sender.
    template <class Send>
    void sendChannelMessage(ChannelMessage, Send &&send);

    /// Send a MIDI System Common message using the given sender.
    template <class Send>
    void sendSysCommonMessage(SysCommonMessage, Send &&send);

    /// Send a MIDI Real-Time message using the given sender.
    template <class Send>
    void sendRealTimeMessage(RealTimeMessage, Send &&send);

    /// Send a MIDI System Exclusive message using the given sender.
    /// Message may be chunked, remaining bytes are stored until next chunk is
    /// sent.
    template <class Send>
    void sendSysEx(SysExMessage, Send &&send);

  private:
    /// Send a single SysEx starts or continues USB packet. Exactly 3 bytes are
    /// sent. The `data` pointer is not incremented.
    template <class Send>
    void sendSysExStartCont1(const uint8_t *data, Cable cable, Send &send);
    /// Send as many SysEx starts or continues USB packets, such that the
    /// remaining length is 3, 2 or 1 byte. The `data` pointer is incremented,
    /// and the `length` is decremented.
    /// The reason for leaving 3, 2 or 1 bytes remaining is so the message can
    /// be finished using a SysExEnd USB packet, which has to have 3, 2 or 1
    /// bytes.
    template <class Send>
    void sendSysExStartCont(const uint8_t *&data, uint16_t &length, Cable cable,
                            Send &send);
    /// Send a SysExEnd USB packet. The `length` should be either 3, 2 or 1
    /// bytes, and the last byte of `data` should be a SysExEnd byte.
    template <class Send>
    void sendSysExEnd(const uint8_t *data, uint16_t length, Cable cable,
                      Send &send);

  private:
    /// Stores remainder of outgoing SysEx chunks. Each USB packet (except the
    /// last one) should contain a multiple of 3 SysEx bytes. If the SysEx chunk
    /// size is not a multiple of 3, there will be remaining bytes that can't be
    /// sent yet, until the next chunk arrives. See the comments in the
    /// @ref USBMIDI_Sender::sendSysEx() implementation for more details.
    uint8_t storedSysExData[16][3];
    /// Number of remaining SysEx bytes stored.
    uint8_t storedSysExLength[16] = {};

    using CIN = MIDICodeIndexNumber;
};

template <class Send>
void USBMIDI_Sender::sendChannelMessage(ChannelMessage msg, Send &&send) {
    send(msg.cable, CIN(msg.header >> 4), // CN|CIN
         msg.header,                      // status
         msg.data1,                       // data 1
         msg.data2);                      // data 2
}

template <class Send>
void USBMIDI_Sender::sendSysCommonMessage(SysCommonMessage msg, Send &&send) {
    auto cn = msg.cable;
    switch (msg.getNumberOfDataBytes()) {
        case 2: // 3B
            send(cn, CIN::SYSTEM_COMMON_3B, msg.header, msg.data1, msg.data2);
            break;
        case 1: // 2B
            send(cn, CIN::SYSTEM_COMMON_2B, msg.header, msg.data1, 0);
            break;
        case 0: // 1B
            send(cn, CIN::SYSTEM_COMMON_1B, msg.header, 0, 0);
            break;
        default: break;
    }
}

template <class Send>
void USBMIDI_Sender::sendRealTimeMessage(RealTimeMessage msg, Send &&send) {
    send(msg.cable, CIN::SINGLE_BYTE, msg.message, 0, 0);
}

// This is the readable documentation version for sending full SysEx messages
#if 0
template <class Send>
void USBMIDI_Sender::sendFullSysEx(SysExMessage msg, Send &send) {
    size_t length = msg.length;
    const uint8_t *data = msg.data;
    Cable cn = msg.cable;
    while (length > 3) {
        send(cn, CIN::SYSEX_START_CONT, data[0], data[1], data[2]);
        data += 3;
        length -= 3;
    }
    switch (length) {
        case 3: send(cn, CIN::SYSEX_END_3B, data[0], data[1], data[2]); break;
        case 2: send(cn, CIN::SYSEX_END_2B, data[0], data[1], 0); break;
        case 1: send(cn, CIN::SYSEX_END_1B, data[0], 0, 0); break;
        default: break;
    }
}
#endif // 0

template <class Send>
void USBMIDI_Sender::sendSysExStartCont1(const uint8_t *data, Cable cable,
                                         Send &send) {
    send(cable, CIN::SYSEX_START_CONT, data[0], data[1], data[2]);
}

template <class Send>
void USBMIDI_Sender::sendSysExStartCont(const uint8_t *&data, uint16_t &length,
                                        Cable cable, Send &send) {
    while (length > 3) {
        sendSysExStartCont1(data, cable, send);
        data += 3;
        length -= 3;
    }
}

template <class Send>
void USBMIDI_Sender::sendSysExEnd(const uint8_t *data, uint16_t length,
                                  Cable cn, Send &send) {
    switch (length) {
        case 3: send(cn, CIN::SYSEX_END_3B, data[0], data[1], data[2]); break;
        case 2: send(cn, CIN::SYSEX_END_2B, data[0], data[1], 0); break;
        case 1: send(cn, CIN::SYSEX_END_1B, data[0], 0, 0); break;
        default: break; // LCOV_EXCL_LINE
    }
}

template <class Send>
void USBMIDI_Sender::sendSysEx(const SysExMessage msg, Send &&send) {
    // Don't bother trying to send empty messages
    if (msg.length == 0)
        return;

    Cable cable = msg.cable;
    uint8_t c = cable.getRaw();
    uint16_t length = msg.length;
    const uint8_t *data = msg.data;

    // Even if the previous chunk wasn't terminated correctly, if this is a new
    // SysEx message, always forget the previous unsent chunk.
    if (msg.isFirstChunk()) {
        // TODO: send a SysExEnd for previous chunk?
        storedSysExLength[c] = 0;
    }

    // Complete the previous unsent chunk to (at most) 3 bytes
    while (length > 0 && storedSysExLength[c] < 3) {
        storedSysExData[c][storedSysExLength[c]++] = *data++;
        --length;
    }

    // If all bytes of the new chunk were used, there are <= 3 stored bytes
    if (length == 0) {
        // If this chunk is the last one, terminate the SysEx (termination can
        // have 1, 2 or 3 bytes, so no need to store anything)
        if (msg.isLastChunk()) {
            sendSysExEnd(storedSysExData[c], storedSysExLength[c], cable, send);
            storedSysExLength[c] = 0;
        }
        // If it's the end of the chunk but not the end of the SysEx, and if we
        // have exactly 3 bytes left, we can send them immediately
        else if (storedSysExLength[c] == 3) {
            sendSysExStartCont1(storedSysExData[c], cable, send);
            storedSysExLength[c] = 0;
        }
        // If we have less than 3 bytes, we cannot send them now, we have to
        // store them and wait for the next chunk
    }
    // If there are new bytes left in the chunk, there are exactly 3 stored
    // bytes
    else {
        // First send the 3 stored bytes
        sendSysExStartCont1(storedSysExData[c], cable, send);
        // Then send whatever new data is left in the new chunk (but leave at
        // least 3 bytes)
        sendSysExStartCont(data, length, cable, send);
        // If this chunk is the last one, terminate the SysEx (termination can
        // have 1, 2 or 3 bytes, so no need to store anything)
        if (msg.isLastChunk()) {
            sendSysExEnd(data, length, cable, send);
            storedSysExLength[c] = 0;
        }
        // If it's the end of the chunk but not the end of the SysEx, and if we
        // have exactly 3 bytes left, we can send them immediately
        else if (length == 3) {
            sendSysExStartCont1(data, cable, send);
            storedSysExLength[c] = 0;
        }
        // If we have 1 or 2 bytes left, we cannot send them now, so store them
        // until we get enough data to fill a 3-byte packet
        else {
            storedSysExLength[c] = length;
            memcpy(storedSysExData[c], data, length);
        }
    }
}

END_CS_NAMESPACE

AH_DIAGNOSTIC_POP()