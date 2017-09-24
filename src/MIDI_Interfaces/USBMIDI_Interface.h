#ifndef USBMIDI_INTERFACE_H_
#define USBMIDI_INTERFACE_H_

#include "MIDI_Interface.h"

#ifdef CORE_TEENSY
#include <usb_dev.h>
#endif

#if defined(USBCON) && !defined(CORE_TEENSY) // If the main MCU has a USB connection but is not a Teensy
#include "MIDIUSB.h"
#endif

#if defined(USBCON) || defined(CORE_TEENSY) // If the main MCU has a USB connection (or is a Teensy)
class USBMIDI_Interface : public MIDI_Interface
{
  public:
    USBMIDI_Interface()
    {
        ;
    }

    bool refresh() // Ignore MIDI input
    {
#if defined(CORE_TEENSY)        // If it's a Teensy board
        if (!usb_configuration) // Check USB configuration
            return false;
        usb_packet_t *rx_packet = usb_rx(MIDI_RX_ENDPOINT); // Read a new packet from the USB buffer
        if (rx_packet == nullptr)                           // If there's no new packet, return
            return false;

        usb_free(rx_packet); // Free the packet
        return true;         // repeat

#elif defined(USBCON) // If the main MCU has a USB connection but is not a Teensy
        return MidiUSB.read().header != 0; // if there's a packet to read, discard it, and read again next time
#endif
    }

  protected:
    void sendImpl(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2)
    {
#if defined(CORE_TEENSY) // If it's a Teensy board
        usb_midi_write_packed((m >> 4) | ((m | c) << 8) | (d1 << 16) | (d2 << 24));
#elif defined(USBCON) // If the main MCU has a USB connection but is not a Teensy
        midiEventPacket_t msg = {m >> 4, m | c, d1, d2};
        MidiUSB.sendMIDI(msg);
        MidiUSB.flush();
#endif
    }
    void sendImpl(uint8_t m, uint8_t c, uint8_t d1)
    {
        sendImpl(m, c, d1, 0);
    }
};

#else // If the main MCU doesn't have a USB connection

#include "SerialMIDI_Interface.h"

class USBMIDI_Interface : public USBSerialMIDI_Interface
{
  public:
    USBMIDI_Interface() : USBSerialMIDI_Interface(MIDI_BAUD){};
};

#endif

#endif // USBMIDI_INTERFACE_H_