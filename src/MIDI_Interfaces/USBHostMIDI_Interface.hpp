#pragma once

#include "USBMIDI/Teensy-host/TeensyHostMIDI.hpp"
#include "USBMIDI_Interface.hpp"

AH_DIAGNOSTIC_WERROR()

BEGIN_CS_NAMESPACE

/// MIDI over USB backend for the Teensy
/// [USBHost_t36](https://github.com/PaulStoffregen/USBHost_t36) library.
template <size_t MaxPacketSize = 512>
class USBHostMIDIBackend {
  public:
    USBHostMIDIBackend(USBHost &host) : backend(host) {}

  public:
    using MIDIUSBPacket_t = AH::Array<uint8_t, 4>;
    MIDIUSBPacket_t read() { return u32_to_bytes(backend.read()); }
    void write(uint8_t cn_cin, uint8_t midi_0, uint8_t midi_1, uint8_t midi_2) {
        backend.write(bytes_to_u32(cn_cin, midi_0, midi_1, midi_2));
    }
    void sendNow() { backend.send_now(); }
    bool preferImmediateSend() { return false; }

  public:
    TeensyHostMIDI<MaxPacketSize> backend;
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
    : public GenericUSBMIDI_Interface<USBHostMIDIBackend<64>> {
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
    : public GenericUSBMIDI_Interface<USBHostMIDIBackend<512>> {
  public:
    USBHostMIDI_Interface_BigBuffer(USBHost &host)
        : GenericUSBMIDI_Interface(host) {}
};

END_CS_NAMESPACE

AH_DIAGNOSTIC_POP()
