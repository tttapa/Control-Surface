#pragma once

#include "../MIDI_Parsers/SerialMIDI_Parser.h"
#include "../Settings/SettingsWrapper.h"
#include "MIDI_Interface.h"
#include <Arduino.h> // Stream

class StreamMIDI_Interface : public MIDI_Interface {
  public:
    StreamMIDI_Interface(Stream &stream)
        : MIDI_Interface(parser), stream(stream) {}

    virtual MIDI_read_t read() {
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

    virtual void sendImpl(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2) {
        stream.write(m | c); // Send the MIDI message over the stream
        stream.write(d1);
        stream.write(d2);
        stream.flush(); // TODO
    }
    virtual void sendImpl(uint8_t m, uint8_t c, uint8_t d1) {
        stream.write(m | c); // Send the MIDI message over the stream
        stream.write(d1);
        stream.flush(); // TODO
    }

  protected:
    Stream &stream;
};

template <typename T>
class SerialMIDI_Interface : public StreamMIDI_Interface {
  public:
    SerialMIDI_Interface(T &serial, unsigned long baud)
        : StreamMIDI_Interface(serial), serial(serial), baud(baud) {}
    virtual void begin() { serial.begin(baud); }

  private:
    T &serial;
    const unsigned long baud;
};

class HardwareSerialMIDI_Interface
    : public SerialMIDI_Interface<HardwareSerial> {
  public:
    HardwareSerialMIDI_Interface(HardwareSerial &serial, unsigned long baud)
        : SerialMIDI_Interface(serial, baud) {}
};

// The Serial USB connection
// (Why do I have to do this, why don't they all inherit from one single class?)

// Boards without a USB connection (UNO, MEGA, Nano ...)
#if !(defined(USBCON) || defined(CORE_TEENSY))
class USBSerialMIDI_Interface : public HardwareSerialMIDI_Interface {
  public:
    USBSerialMIDI_Interface(unsigned long baud)
        : HardwareSerialMIDI_Interface(Serial, baud) {}
};

// Teensies
#elif defined(CORE_TEENSY)
class USBSerialMIDI_Interface : public SerialMIDI_Interface<usb_serial_class> {
  public:
    USBSerialMIDI_Interface(unsigned long baud)
        : SerialMIDI_Interface(Serial, baud) {}
};

// Arduino DUE
#elif defined(ARDUINO_ARCH_SAM)
class USBSerialMIDI_Interface : public SerialMIDI_Interface<UARTClass> {
  public:
    USBSerialMIDI_Interface(unsigned long baud)
        : SerialMIDI_Interface(Serial, baud) {}
};

// Others (Leonardo, Micro ... )
#else
class USBSerialMIDI_Interface : public SerialMIDI_Interface<Serial_> {
  public:
    USBSerialMIDI_Interface(unsigned long baud)
        : SerialMIDI_Interface(Serial, baud) {}
};
#endif

// Class for Hairless MIDI
class HairlessMIDI_Interface : public USBSerialMIDI_Interface {
  public:
    HairlessMIDI_Interface() : USBSerialMIDI_Interface(HAIRLESS_BAUD){};
};

#if defined(__AVR__) || defined(CORE_TEENSY)
#include <SoftwareSerial.h>
class SoftwareSerialMIDI_Interface
    : public SerialMIDI_Interface<SoftwareSerial> {
  public:
    SoftwareSerialMIDI_Interface(SoftwareSerial &serial, unsigned long baud)
        : SerialMIDI_Interface(serial, baud) {}
};
#endif