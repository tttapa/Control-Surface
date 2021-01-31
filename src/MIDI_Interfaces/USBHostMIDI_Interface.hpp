#pragma once

#include "USBMIDI_Interface.hpp"
#include <USBHost_t36.h>

AH_DIAGNOSTIC_WERROR()

BEGIN_CS_NAMESPACE

/// MIDI over USB backend for the Teensy 
/// [USBHost_t36](https://github.com/PaulStoffregen/USBHost_t36) library.
template <size_t MaxPacketSize = 512, size_t RXQueueSize = 400>
class USBHostMIDIBackend : public LowLevelMIDIDeviceBase {
  public:
    USBHostMIDIBackend(USBHost &host)
        : LowLevelMIDIDeviceBase(host, rx, tx1, tx2, MaxPacketSize, queue,
                                 RXQueueSize) {}

  public:
    using MIDIUSBPacket_t = AH::Array<uint8_t, 4>;
    MIDIUSBPacket_t read() {
        MIDIUSBPacket_t packet;
        uint32_t msg = read_packed();
        static_assert(sizeof(msg) == sizeof(packet.data),
                      "incorrect packet size");
        memcpy(packet.data, &msg, sizeof(packet.data));
        return packet;
    }
    void write(uint8_t cn, uint8_t cin, uint8_t d0, uint8_t d1, uint8_t d2) {
        write_packed((cn << 4) | cin | // CN|CIN
                     (d0 << 8) |       // status
                     (d1 << 16) |      // data 1
                     (d2 << 24));      // data 2
    }
    void sendNow() { send_now(); }
    bool preferImmediateSend() { return false; }

  private:
    static_assert(RXQueueSize > MaxPacketSize / 4,
                  "RXQueueSize must be more than MaxPacketSize/4");
    uint32_t rx[MaxPacketSize / 4];
    uint32_t tx1[MaxPacketSize / 4];
    uint32_t tx2[MaxPacketSize / 4];
    uint32_t queue[RXQueueSize];
};

/**
 * @brief   A class for MIDI interfaces sending MIDI messages to a MIDI USB 
 *          device connected to a Teensy's USB host port.
 * 
 * Requires a Teensy 3.6 or 4.1 and the
 * [USBHost_t36](https://github.com/PaulStoffregen/USBHost_t36) library.
 * 
 * This one uses the default small buffer (similar to USBHost_t36's 
 * `MIDIDevice`).
 * 
 * @see @ref USBHostMIDI_Interface_BigBuffer for a bigger buffer.
 * 
 * @ingroup MIDIInterfaces
 */
class USBHostMIDI_Interface
    : public GenericUSBMIDI_Interface<USBHostMIDIBackend<64, 80>> {
  public:
    USBHostMIDI_Interface(USBHost &host) : GenericUSBMIDI_Interface(host) {}
};

/**
 * @brief   A class for MIDI interfaces sending MIDI messages to a MIDI USB 
 *          device connected to a Teensy's USB host port.
 * 
 * Requires a Teensy 3.6 or 4.1 and the
 * [USBHost_t36](https://github.com/PaulStoffregen/USBHost_t36) library.
 * 
 * This one uses a larger buffer (similar to USBHost_t36's 
 * `MIDIDevice_BigBuffer`).
 * 
 * @see @ref USBHostMIDI_Interface for a smaller buffer.
 * 
 * @ingroup MIDIInterfaces
 */
class USBHostMIDI_Interface_BigBuffer
    : public GenericUSBMIDI_Interface<USBHostMIDIBackend<512, 400>> {
  public:
    USBHostMIDI_Interface_BigBuffer(USBHost &host)
        : GenericUSBMIDI_Interface(host) {}
};

END_CS_NAMESPACE

AH_DIAGNOSTIC_POP()
