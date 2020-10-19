#include "USBMIDI_Interface.hpp"
#include <MIDI_Parsers/LambdaPuller.hpp>

// If the main MCU has a USB connection or is a Teensy with MIDI USB type
#if defined(USBCON) || defined(TEENSY_MIDIUSB_ENABLED) || !defined(ARDUINO)

BEGIN_CS_NAMESPACE

// -------------------------------------------------------------------------- //

#ifdef ARDUINO
USBMIDI_Interface::USBMIDI_Interface()
    : alwaysSendImmediately_(USBMIDI::preferImmediateSend()) {}
#else
USBMIDI_Interface::USBMIDI_Interface() = default;
#endif

// -------------------------------------------------------------------------- //

// Reading MIDI

MIDIReadEvent USBMIDI_Interface::read() {
    auto pullpacket = [this](MIDIUSBPacket_t &packet) {
        packet = readUSBPacket();
        return packet[0] != 0x00;
    };
    return parser.pull(LambdaPuller(std::move(pullpacket)));
}

void USBMIDI_Interface::update() { MIDI_Interface::updateIncoming(this); }

void USBMIDI_Interface::handleStall() { MIDI_Interface::handleStall(this); }

// -------------------------------------------------------------------------- //

// Sending MIDI

void USBMIDI_Interface::sendChannelMessageImpl(ChannelMessage msg) {
    writeUSBPacket(msg.cable, msg.header >> 4, // CN|CIN
                   msg.header,                 // status
                   msg.data1,                  // data 1
                   msg.data2);                 // data 2
    if (alwaysSendImmediately_)
        sendUSBNow();
}

// This is the readable documentation version for sending full SysEx messages
#if 0 
void USBMIDI_Interface::sendFullSysEx(SysExMessage msg) {
    size_t length = msg.length;
    const uint8_t *data = msg.data;
    Cable cable = msg.cable;
    while (length > 3) {
        writeUSBPacket(cable, 0x4, data[0], data[1], data[2]);
        data += 3;
        length -= 3;
    }
    switch (length) {
        case 3: writeUSBPacket(cable, 0x7, data[0], data[1], data[2]); break;
        case 2: writeUSBPacket(cable, 0x6, data[0], data[1], 0); break;
        case 1: writeUSBPacket(cable, 0x5, data[0], 0, 0); break;
        default: break;
    }
    if (alwaysSendImmediately_)
        sendUSBNow();
}
#endif // 0

void USBMIDI_Interface::sendSysExStartCont1(const uint8_t *data, Cable cable) {
    writeUSBPacket(cable, 0x4, data[0], data[1], data[2]);
}

void USBMIDI_Interface::sendSysExStartCont(const uint8_t *&data,
                                           uint16_t &length, Cable cable) {
    while (length > 3) {
        sendSysExStartCont1(data, cable);
        data += 3;
        length -= 3;
    }
}

void USBMIDI_Interface::sendSysExEnd(const uint8_t *data, uint16_t length,
                                     Cable cable) {
    switch (length) {
        case 3: writeUSBPacket(cable, 0x7, data[0], data[1], data[2]); break;
        case 2: writeUSBPacket(cable, 0x6, data[0], data[1], 0); break;
        case 1: writeUSBPacket(cable, 0x5, data[0], 0, 0); break;
        default: break; // LCOV_EXCL_LINE
    }
}

void USBMIDI_Interface::sendSysExImpl(const SysExMessage msg) {
    // Don't bother trying to send empty messages
    if (msg.length == 0)
        return;

    Cable cable = msg.cable;
    uint16_t length = msg.length;
    const uint8_t *data = msg.data;

    // Even if the previous chunk wasn't terminated correctly, if this is a new
    // SysEx message, always forget the previous unsent chunk.
    if (msg.isFirstChunk()) {
        // TODO: send a SysExEnd for previous chunk?
        storedSysExLength = 0;
    }

    // Complete the previous unsent chunk to (at most) 3 bytes
    while (length > 0 && storedSysExLength < 3) {
        storedSysExData[storedSysExLength++] = *data++;
        --length;
    }

    // If all bytes of the new chunk were used, there are <= 3 stored bytes
    if (length == 0) {
        // If this chunk is the last one, terminate the SysEx (termination can
        // have 1, 2 or 3 bytes, so no need to store anything)
        if (msg.isLastChunk()) {
            sendSysExEnd(storedSysExData, storedSysExLength, cable);
            storedSysExLength = 0;
        }
        // If it's the end of the chunk but not the end of the SysEx, and if we
        // have exactly 3 bytes left, we can send them immediately
        else if (storedSysExLength == 3) {
            sendSysExStartCont1(storedSysExData, cable);
            storedSysExLength = 0;
        }
        // If we have less than 3 bytes, we cannot send them now, we have to
        // store them and wait for the next chunk
    }
    // If there are new bytes left in the chunk, there are exactly 3 stored
    // bytes
    else {
        // First send the 3 stored bytes
        sendSysExStartCont1(storedSysExData, cable);
        // Then send whatever new data is left in the new chunk (but leave at
        // least 3 bytes)
        sendSysExStartCont(data, length, cable);
        // If this chunk is the last one, terminate the SysEx (termination can
        // have 1, 2 or 3 bytes, so no need to store anything)
        if (msg.isLastChunk()) {
            sendSysExEnd(data, length, cable);
            storedSysExLength = 0;
        }
        // If it's the end of the chunk but not the end of the SysEx, and if we
        // have exactly 3 bytes left, we can send them immediately
        else if (length == 3) {
            sendSysExStartCont1(data, cable);
            storedSysExLength = 0;
        }
        // If we have 1 or 2 bytes left, we cannot send them now, so store them
        // until we get enough data to fill a 3-byte packet
        else {
            storedSysExLength = length;
            memcpy(storedSysExData, data, length);
        }
    }
    if (alwaysSendImmediately_)
        sendUSBNow();
}

void USBMIDI_Interface::sendRealTimeImpl(RealTimeMessage msg) {
    writeUSBPacket(msg.cable, 0xF, // CN|CIN
                   msg.message,    // single byte
                   0,              // no data
                   0);             // no data
    if (alwaysSendImmediately_)
        sendUSBNow();
}

END_CS_NAMESPACE

#endif
