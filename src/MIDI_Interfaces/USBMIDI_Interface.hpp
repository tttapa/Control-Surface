#pragma once

#include "MIDI_Interface.hpp"
#include "USBMIDI/USBMIDI.hpp"
#include <AH/Error/Error.hpp>
#include <AH/Teensy/TeensyUSBTypes.hpp>
#include <MIDI_Parsers/USBMIDI_Parser.hpp>

AH_DIAGNOSTIC_WERROR()

#if defined(TEENSYDUINO) && !defined(TEENSY_MIDIUSB_ENABLED)
#warning                                                                       \
    "Teensy: USB MIDI not enabled. Enable it from the Tools > USB Type menu."
#endif

#ifndef ARDUINO
#include <gmock-wrapper.h>
#endif

// If the main MCU has a USB connection or is a Teensy with MIDI USB type
#if defined(USBCON) || defined(TEENSY_MIDIUSB_ENABLED) || !defined(ARDUINO)

BEGIN_CS_NAMESPACE

/**
 * @brief   A class for MIDI interfaces sending MIDI messages over a USB MIDI
 *          connection.
 * 
 * On boards that support it, this will create a native MIDI over USB interface
 * using the platform-specific libraries (e.g. MIDIUSB for Arduino Leonardo, or 
 * the Core usbMIDI library for Teensy).  
 * On boards without native USB support, it'll fall back to a serial MIDI 
 * interface at the default @ref MIDI_BAUD "MIDI baud rate" on the UART 
 * connected to the Serial to USB chip. This can be used with custom 
 * MIDI over USB firmware for the Serial to USB chip.
 * 
 * @note    See @ref md_pages_MIDI-over-USB for more information.
 * 
 * @ingroup MIDIInterfaces
 */
class USBMIDI_Interface : public Parsing_MIDI_Interface {
  public:
    /**
     * @brief   Construct a new USBMIDI_Interface.
     */
    USBMIDI_Interface() : Parsing_MIDI_Interface(parser) {}

    using MIDIUSBPacket_t = USBMIDI::MIDIUSBPacket_t;

  private:
    USBMIDI_Parser parser;

#ifndef ARDUINO
  public:
    MOCK_METHOD(void, writeUSBPacket,
                (uint8_t, uint8_t, uint8_t, uint8_t, uint8_t));
    MOCK_METHOD(MIDIUSBPacket_t, readUSBPacket, ());
    void flushUSB() {}

  private:
#else
    void writeUSBPacket(uint8_t cn, uint8_t cin, uint8_t d0, uint8_t d1,
                        uint8_t d2) {
        USBMIDI::write(cn, cin, d0, d1, d2);
    }
    MIDIUSBPacket_t readUSBPacket() { return USBMIDI::read(); }
    void flushUSB() { USBMIDI::flush(); }
#endif

    void sendImpl(uint8_t header, uint8_t d1, uint8_t d2, uint8_t cn) override {
        writeUSBPacket(cn, header >> 4, // CN|CIN
                       header,          // status
                       d1,              // data 1
                       d2);             // data 2
        flushUSB();
    }

    void sendImpl(uint8_t header, uint8_t d1, uint8_t cn) override {
        sendImpl(header, d1, 0, cn);
    }

    void sendImpl(const uint8_t *data, size_t length, uint8_t cn) override {
        while (length > 3) {
            writeUSBPacket(cn, 0x4, data[0], data[1], data[2]);
            data += 3;
            length -= 3;
        }
        switch (length) {
            case 3: writeUSBPacket(cn, 0x7, data[0], data[1], data[2]); break;
            case 2: writeUSBPacket(cn, 0x6, data[0], data[1], 0); break;
            case 1: writeUSBPacket(cn, 0x5, data[0], 0, 0); break;
            default: break;
        }
        flushUSB();
    }

    void sendImpl(uint8_t rt, uint8_t cn) override {
        writeUSBPacket(cn, 0xF, // CN|CIN
                       rt,      // single byte
                       0,       // no data
                       0);      // no data
        flushUSB();
    }

  public:
    MIDIReadEvent read() override {
        for (uint8_t i = 0; i < (SYSEX_BUFFER_SIZE + 2) / 3; ++i) {
            MIDIUSBPacket_t midi_packet = readUSBPacket();
            if (midi_packet.data[0] == 0)
                return MIDIReadEvent::NO_MESSAGE;

            MIDIReadEvent parseResult = parser.parse(midi_packet.data);

            if (parseResult != MIDIReadEvent::NO_MESSAGE)
                return parseResult;
        }
        return MIDIReadEvent::NO_MESSAGE;
    }
};

END_CS_NAMESPACE

// If the main MCU doesn't have a USB connection:
// Fall back on Serial connection at the hardware MIDI baud rate.
// (Can be used with HIDUINO or USBMidiKliK.)
#else

#include "SerialMIDI_Interface.hpp"

BEGIN_CS_NAMESPACE

/**
 * @brief   A class for MIDI interfaces sending MIDI messages over a USB MIDI
 *          connection.
 * 
 * @note    See @ref md_pages_MIDI-over-USB for more information.
 * 
 * @ingroup MIDIInterfaces
 */
class USBMIDI_Interface : public USBSerialMIDI_Interface {
  public:
    /**
     * @brief   Construct a new USBMIDI_Interface.
     */
    USBMIDI_Interface() : USBSerialMIDI_Interface(MIDI_BAUD) {}
};

END_CS_NAMESPACE

#endif

AH_DIAGNOSTIC_POP()