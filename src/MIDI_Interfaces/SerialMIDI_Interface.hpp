#pragma once

#include "MIDI_Interface.hpp"
#include <Arduino.h> // Stream
#include <MIDI_Parsers/SerialMIDI_Parser.hpp>
#include <Settings/SettingsWrapper.hpp>

/**
 * @brief   A class for MIDI interfaces sending and receiving MIDI messages
 *          over a Stream.
 * 
 * @ingroup MIDIInterfaces
 */
class StreamMIDI_Interface : public MIDI_Interface {
  public:
    /**
     * @brief   Construct a StreamMIDI_Interface on the given Stream.
     *
     * @param   stream
     *          The Stream interface.
     */
    StreamMIDI_Interface(Stream &stream)
        : MIDI_Interface(parser), stream(stream) {}

    virtual MIDI_read_t read() override {
        while (stream.available() > 0) {
            uint8_t midiByte = stream.read();
            MIDI_read_t parseResult = parser.parse(midiByte);
            if (parseResult != NO_MESSAGE)
                return parseResult;
        }
        return NO_MESSAGE;
    }

  protected:
    SerialMIDI_Parser parser;

    void sendImpl(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2) override {
        stream.write(m | c); // Send the MIDI message over the stream
        stream.write(d1);
        stream.write(d2);
        stream.flush(); // TODO
    }

    void sendImpl(uint8_t m, uint8_t c, uint8_t d1) override {
        stream.write(m | c); // Send the MIDI message over the stream
        stream.write(d1);
        stream.flush(); // TODO
    }

  protected:
    Stream &stream;
};

/**
 * @brief   A wrapper class for MIDI interfaces sending and receiving
 *          MIDI messages over a Serial port of class T.
 *
 * @note    This is a template class because the class of the Serial object
 *          is completely different on different architectures, and they
 *          do not share a common super-class that has a `begin` method.
 * 
 * @ingroup MIDIInterfaces
 */
template <typename T>
class SerialMIDI_Interface : public StreamMIDI_Interface {
  public:
    /**
     * @brief   Create a new MIDI Interface on the given Serial interface
     *          with the given baud rate.
     *
     * @param   serial
     *          The Serial interface.
     * @param   baud
     *          The baud rate for the Serial interface.
     */
    SerialMIDI_Interface(T &serial, unsigned long baud)
        : StreamMIDI_Interface(serial), serial(serial), baud(baud) {}
    /**
     * @brief   Start the Serial interface at the predefined baud rate.
     */
    virtual void begin() override { serial.begin(baud); }

  private:
    T &serial;
    const unsigned long baud;
};

/**
 * @brief   A class for MIDI interfaces sending and receiving
 *          MIDI messages over a Serial port of class T.
 * 
 * @ingroup MIDIInterfaces
 */
class HardwareSerialMIDI_Interface
    : public SerialMIDI_Interface<HardwareSerial> {
  public:
    /**
     * @brief   Construct a new MIDI Interface on the given HardwareSerial
     *          interface with the given baud rate.
     *
     * @param   serial
     *          The HardwareSerial interface.
     * @param   baud
     *          The baud rate for the serial interface.
     */
    HardwareSerialMIDI_Interface(HardwareSerial &serial, unsigned long baud)
        : SerialMIDI_Interface(serial, baud) {}
};

// The Serial USB connection
// (Why do I have to do this, why don't they all inherit from one single class?)

// Boards without a USB connection (UNO, MEGA, Nano ...)
#if !(defined(USBCON) || defined(CORE_TEENSY))
/**
 * @brief   A class for MIDI interfaces sending and receiving
 *          MIDI messages over the USB CDC connection.
 *
 *          Boards without a native USB connection (UNO, MEGA, Nano ...)
 *          use HardwareSerial0 for USB communcication.
 * 
 * @ingroup MIDIInterfaces
 */
class USBSerialMIDI_Interface : public HardwareSerialMIDI_Interface {
  public:
    /**
     * @brief   Construct a USBSerialMIDI_Interface with the given baud rate.
     *
     * @param   baud
     *          The baud rate to start the USB Serial connection with.
     */
    USBSerialMIDI_Interface(unsigned long baud)
        : HardwareSerialMIDI_Interface(Serial, baud) {}
};

// Teensies
#elif defined(TEENSYDUINO)
#if defined(TEENSY_SERIALUSB_ENABLED)
/**
 * @brief   A class for MIDI interfaces sending and receiving
 *          MIDI messages over the USB Serial CDC connection.
 *
 *          The USB Serial connection `Serial` on Teensies is an instance of
 *          the `usb_serial_class`.
 * 
 * @ingroup MIDIInterfaces
 */
class USBSerialMIDI_Interface : public SerialMIDI_Interface<usb_serial_class> {
  public:
    /**
     * @brief   Construct a USBSerialMIDI_Interface with the given baud rate.
     *
     * @param   baud
     *          The baud rate to start the USB Serial connection with.
     */
    USBSerialMIDI_Interface(unsigned long baud)
        : SerialMIDI_Interface(Serial, baud) {}
};
#endif

// Arduino DUE
#elif defined(ARDUINO_ARCH_SAM)
/**
 * @brief   A class for MIDI interfaces sending and receiving
 *          MIDI messages over the USB Serial CDC connection.
 *
 *          The USB Serial connection `Serial` on the Arduino DUE is an
 *          instance of the `UARTClass`.
 * 
 * @ingroup MIDIInterfaces
 */
class USBSerialMIDI_Interface : public SerialMIDI_Interface<UARTClass> {
  public:
    /**
     * @brief   Construct a USBSerialMIDI_Interface with the given baud rate.
     *
     * @param   baud
     *          The baud rate to start the USB Serial connection with.
     */
    USBSerialMIDI_Interface(unsigned long baud)
        : SerialMIDI_Interface(Serial, baud) {}
};

// Others (Leonardo, Micro ... )
#else
/**
 * @brief   A class for MIDI interfaces sending and receiving
 *          MIDI messages over the USB Serial CDC connection.
 *
 *          The USB Serial connection `Serial` on the Arduino Leonardo, Micro,
 *          etc. is an instance of the `Serial_` class.
 * 
 * @ingroup MIDIInterfaces
 */
class USBSerialMIDI_Interface : public SerialMIDI_Interface<Serial_> {
  public:
    /**
     * @brief   Construct a USBSerialMIDI_Interface with the given baud rate.
     *
     * @param   baud
     *          The baud rate to start the USB Serial connection with.
     */
    USBSerialMIDI_Interface(unsigned long baud)
        : SerialMIDI_Interface(Serial, baud) {}
};
#endif

#if defined(TEENSYDUINO) && defined(TEENSY_SERIALUSB_ENABLED)
/**
 * @brief   A class for MIDI Interfaces sending and receiving
 *          data over the USB Serial CDC connection for the use
 *          with the [Hairless MIDI<->Serial Bridge]
 *          (http://projectgus.github.io/hairless-midiserial/).
 * 
 * @ingroup MIDIInterfaces
 */
class HairlessMIDI_Interface : public USBSerialMIDI_Interface {
  public:
    /**
     * @brief   Construct a HairlessMIDI_Interface.
     *
     * The default Hairless baud rate of 115200 baud is used.
     * This can be changed in the Settings.hpp file.
     */
    HairlessMIDI_Interface() : USBSerialMIDI_Interface(HAIRLESS_BAUD){};
};
#endif

#if defined(__AVR__) || defined(TEENSYDUINO)
#include <SoftwareSerial.h>
/**
 * @brief   A class for MIDI interfaces sending and receiving
 *          MIDI messages over a SoftwareSerial interface.
 * 
 * @ingroup MIDIInterfaces
 */
class SoftwareSerialMIDI_Interface
    : public SerialMIDI_Interface<SoftwareSerial> {
  public:
    /**
     * @brief   Create a SoftwareSerialMIDI_Interface on the given
     *          SoftwareSerial interface with the given baud rate.
     *
     * @param   serial
     *          The SoftwareSerial interface.
     * @param   baud
     *          The baud rate for the serial interface.
     */
    SoftwareSerialMIDI_Interface(SoftwareSerial &serial, unsigned long baud)
        : SerialMIDI_Interface(serial, baud) {}
};
#endif