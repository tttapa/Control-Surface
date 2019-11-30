#pragma once

#include "SerialMIDI_Interface.hpp"
#include <ctype.h>

BEGIN_CS_NAMESPACE

/**
 * @brief   A class for MIDI interfaces sending and receiving 
 *          human-readable MIDI messages over a Stream.
 * 
 * @ingroup MIDIInterfaces
 */
class StreamDebugMIDI_Interface : public StreamMIDI_Interface {
  public:
    /**
     * @brief   Construct a debug MIDI interface on the given Stream.
     * 
     * @param   stream
     *          The Stream interface.
     */
    StreamDebugMIDI_Interface(Stream &stream) : StreamMIDI_Interface(stream) {}

    MIDI_read_t read() override;

  protected:
    void sendImpl(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2,
                  uint8_t cn) override;

    void sendImpl(uint8_t m, uint8_t c, uint8_t d1, uint8_t cn) override;

    void sendImpl(const uint8_t *data, size_t length, uint8_t cn) override;

    void sendImpl(uint8_t rt, uint8_t cn) override;

  private:
    char firstChar = '\0';
    char secondChar = '\0';

    /**
     * @brief   Convert a hexadecimal character to a 4-bit nibble.
     */
    static uint8_t hexCharToNibble(char hex) {
        return hex < 'a' ? hex - '0' : hex - 'a' + 10;
    }
};

/**
 * @brief   A wrapper class for debug MIDI interfaces sending and receiving 
 *          human-readable MIDI messages over a Serial port of class T.
 * 
 * @note    This is a template class because the class of the Serial object
 *          is completely different on different architectures, and they
 *          do not share a common super-class that has a `begin` method.
 * 
 * @ingroup MIDIInterfaces
 */
template <typename T>
class SerialDebugMIDI_Interface : public StreamDebugMIDI_Interface {
  public:
    /**
     * @brief   Construct a new Debug MIDI Interface on the given Serial 
     *          interface with the given baud rate.
     * 
     * @param   serial
     *          The Serial interface.
     * @param   baud
     *          The baud rate for the Serial interface.
     */
    SerialDebugMIDI_Interface(T &serial,
                              unsigned long baud = AH::defaultBaudRate)
        : StreamDebugMIDI_Interface(serial), serial(serial), baud(baud) {}
    /**
     * @brief   Start the Serial interface at the predefined baud rate.
     */
    void begin() override { serial.begin(baud); }

  private:
    T &serial;
    const unsigned long baud;
};

/**
 * @brief   A class for debug MIDI interfaces sending and receiving 
 *          human-readable MIDI messages over a HardwareSerial port.
 * 
 * @ingroup MIDIInterfaces
 */
class HardwareSerialDebugMIDI_Interface
    : public SerialDebugMIDI_Interface<HardwareSerial> {
  public:
    /**
     * @brief   Construct a new Debug MIDI Interface on the given HardwareSerial 
     *          interface with the given baud rate.
     * 
     * @param   serial
     *          The HardwareSerial interface.
     * @param   baud
     *          The baud rate for the serial interface.
     */
    HardwareSerialDebugMIDI_Interface(HardwareSerial &serial,
                                      unsigned long baud = AH::defaultBaudRate)
        : SerialDebugMIDI_Interface(serial, baud) {}
};

/**
 * @brief   A class for debug MIDI interfaces sending and receiving 
 *          human-readable MIDI messages over the USB CDC connection.
 * 
 *          Boards without a native USB connection (UNO, MEGA, Nano ...)
 *          use HardwareSerial0 for USB communication.
 * 
 * @ingroup MIDIInterfaces
 */
class USBDebugMIDI_Interface
    : public SerialDebugMIDI_Interface<decltype(Serial)> {
  public:
    /**
     * @brief   Construct a USBDebugMIDI_Interface with the given baud rate.
     * 
     * @param   baud
     *          The baud rate to start the USB Serial connection with.
     */
    USBDebugMIDI_Interface(unsigned long baud = AH::defaultBaudRate)
        : SerialDebugMIDI_Interface(Serial, baud) {}
};

// TODO: Teensy 4.0 SoftwareSerial bug
#if defined(__AVR__) || (defined(TEENSYDUINO) && TEENSYDUINO != 147) ||        \
    (defined(TEENSYDUINO) && !defined(__IMXRT1052__) &&                        \
     !defined(__IMXRT1062__))
#include <SoftwareSerial.h>
/**
 * @brief   A class for debug MIDI interfaces sending and receiving 
 *          human-readable MIDI messages over a SoftwareSerial interface.
 * 
 * @ingroup MIDIInterfaces
 */
class SoftwareSerialDebugMIDI_Interface
    : public SerialDebugMIDI_Interface<SoftwareSerial> {
  public:
    /**
     * @brief   Construct a SoftwareSerialDebugMIDI_Interface on the given 
     *          SoftwareSerial interface with the given baud rate.
     * 
     * @param   serial
     *          The SoftwareSerial interface.
     * @param   baud
     *          The baud rate for the serial interface.
     */
    SoftwareSerialDebugMIDI_Interface(SoftwareSerial &serial,
                                      unsigned long baud)
        : SerialDebugMIDI_Interface(serial, baud) {}
};
#endif

END_CS_NAMESPACE