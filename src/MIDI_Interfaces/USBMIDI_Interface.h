#ifndef USBMIDI_INTERFACE_H_
#define USBMIDI_INTERFACE_H_

#include "../MIDI_Parsers/USBMIDI_Parser.h"
#include "MIDI_Interface.h"

#ifdef CORE_TEENSY
#include <usb_dev.h>
#endif

#if defined(USBCON) && !defined(CORE_TEENSY) // If the main MCU has a USB
                                             // connection but is not a Teensy
#include "MIDIUSB.h"
#endif

// If the main MCU has a USB connection (or is a Teensy)
#if defined(USBCON) || defined(CORE_TEENSY)
class USBMIDI_Interface : public MIDI_Interface {
  public:
    USBMIDI_Interface() : MIDI_Interface(parser) { ; }

  protected:
    USBMIDI_Parser parser;

// If it's a Teensy board
#if defined(CORE_TEENSY)
    void sendImpl(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2) {
        usb_midi_write_packed((m >> 4) | ((m | c) << 8) | (d1 << 16) |
                              (d2 << 24));
    }

// If the main MCU has a USB connection but is not a Teensy
#elif defined(USBCON)
    void sendImpl(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2) {
        midiEventPacket_t msg = {(uint8_t)(m >> 4), (uint8_t)(m | c), d1, d2};
        MidiUSB.sendMIDI(msg);
        MidiUSB.flush();
    }
#endif
    void sendImpl(uint8_t m, uint8_t c, uint8_t d1) { sendImpl(m, c, d1, 0); }

  public:
// If it's a Teensy board
#if defined(CORE_TEENSY)
    MIDI_read_t read() {
        while (1) {
            if (rx_packet == nullptr) { // If there's no previous packet
                if (!usb_configuration) // Check USB configuration
                    return NO_MESSAGE;
                rx_packet = usb_rx(
                    MIDI_RX_ENDPOINT); // Read a new packet from the USB buffer
                if (rx_packet == nullptr) { // If there's no new packet, return
                    return NO_MESSAGE;
                }
                if (rx_packet->len < 4) {
                    // If the lenght is less than 4, it's not a valid MIDI USB
                    // packet
                    usb_free(rx_packet); // Free the packet
                    rx_packet = nullptr; // Read new packet next time around
                    return NO_MESSAGE;
                }
            }

            size_t index = rx_packet->index;
            uint8_t *data = rx_packet->buf + index; // A pointer to this packet

            MIDI_read_t parseResult = parser.parse(data);

            index += 4;
            if (index < rx_packet->len) {
                // If the packet is longer than 4 bytes
                rx_packet->index = index; // Change the index and read the next
                                          // four bytes on next read
            } else {
                // If the packet was only 4 bytes long
                usb_free(rx_packet);                  // Free the packet
                rx_packet = usb_rx(MIDI_RX_ENDPOINT); // Read the next packet
            }
            if (parseResult != NO_MESSAGE)
                return parseResult;
        }
    }

// If the main MCU has a USB connection but is not a Teensy →  MIDIUSB library
#elif defined(USBCON)
    MIDI_read_t read() {
        while (1) {
            midiEventPacket_t midipacket = MidiUSB.read();
            rx_packet = reinterpret_cast<uint8_t *>(&midipacket);
            if (!midipacket.header)
                return NO_MESSAGE;
            MIDI_read_t parseResult = parser.parse(rx_packet);
            if (parseResult != NO_MESSAGE)
                return parseResult;
        }
    }
#endif

  private:
// If it's a Teensy board
#if defined(CORE_TEENSY)
    usb_packet_t *rx_packet = nullptr;

// If the main MCU has a USB connection but is not a Teensy
#elif defined(USBCON)
    uint8_t *rx_packet = nullptr;
#endif
};

// If the main MCU doesn't have a USB connection
#else

#include "SerialMIDI_Interface.h"

class USBMIDI_Interface : public USBSerialMIDI_Interface {
  public:
    USBMIDI_Interface() : USBSerialMIDI_Interface(MIDI_BAUD){};
};

#endif

#endif // USBMIDI_INTERFACE_H_