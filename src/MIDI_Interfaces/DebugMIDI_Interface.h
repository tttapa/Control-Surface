#ifndef DEBUGMIDI_INTERFACE_H_
#define DEBUGMIDI_INTERFACE_H_

#include "SerialMIDI_Interface.h"

const static char *MIDI_STATUS_TYPE_NAMES[] = {"Note Off\t",
                                               "Note On\t\t",
                                               "Key Pressure\t",
                                               "Control Change\t",
                                               "Program Change\t",
                                               "Channel Pressure",
                                               "Pitch Bend\t"};

class StreamDebugMIDI_Interface : public StreamMIDI_Interface
{
public:
  StreamDebugMIDI_Interface(Stream &stream) : StreamMIDI_Interface(stream) {}

protected:
  void sendImpl(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2)
  {
    uint8_t messageType = (m >> 4) - 8;
    if (messageType >= 7)
      return;
    stream.print(MIDI_STATUS_TYPE_NAMES[messageType]);
    stream.print("\tChannel: ");
    stream.print(c + 1);
    stream.print("\tData 1: 0x");
    stream.print(d1, HEX);
    stream.print("\tData 2: 0x");
    stream.print(d2, HEX);
    stream.print("\r\n");
    // stream.print((unsigned long)(m >> 4) | ((m | c) << 8) | (d1 << 16) | (d2 << 24), HEX);
    stream.flush();
  }
  void sendImpl(uint8_t m, uint8_t c, uint8_t d1)
  {
    uint8_t messageType = (m >> 4) - 8;
    if (messageType >= 7)
      return;
    stream.print(MIDI_STATUS_TYPE_NAMES[messageType]);
    stream.print("\tChannel: ");
    stream.print(c + 1);
    stream.print("\tData 1: 0x");
    stream.print(d1, HEX);
    stream.print("\r\n");
    // stream.print((unsigned long)(m >> 4) | ((m | c) << 8) | (d1 << 16), HEX);
    stream.flush();
  }
};

template <typename T>
class SerialDebugMIDI_Interface : public StreamDebugMIDI_Interface
{
public:
  SerialDebugMIDI_Interface(T &serial, unsigned long baud) : serial(serial), baud(baud), StreamDebugMIDI_Interface(serial) {}
  void begin()
  {
    serial.begin(baud);
  }

private:
  T &serial;
  const unsigned long baud;
};

class HardwareSerialDebugMIDI_Interface : public SerialDebugMIDI_Interface<HardwareSerial>
{
public:
  HardwareSerialDebugMIDI_Interface(HardwareSerial &serial, unsigned long baud) : SerialDebugMIDI_Interface(serial, baud) {}
};

// The Serial USB connection
// (Why do I have to do this, why don't they all inherit from one single class?)

#if !(defined(USBCON) || defined(CORE_TEENSY)) // Boards without a USB connection (UNO, MEGA, Nano ...)
class USBDebugMIDI_Interface : public HardwareSerialDebugMIDI_Interface
{
public:
  USBDebugMIDI_Interface(unsigned long baud) : HardwareSerialDebugMIDI_Interface(Serial, baud) {}
};
#elif defined(CORE_TEENSY)      // Teensies
class USBDebugMIDI_Interface : public SerialDebugMIDI_Interface<usb_serial_class>
{
public:
  USBDebugMIDI_Interface(unsigned long baud) : SerialDebugMIDI_Interface(Serial, baud) {}
};
#elif defined(ARDUINO_ARCH_SAM) // Arduino DUE
class USBDebugMIDI_Interface : public SerialDebugMIDI_Interface<UARTClass>
{
public:
  USBDebugMIDI_Interface(unsigned long baud) : SerialDebugMIDI_Interface(Serial, baud) {}
};
#else                           // Others (Leonardo, Micro ... )
class USBDebugMIDI_Interface : public SerialDebugMIDI_Interface<Serial_>
{
public:
  USBDebugMIDI_Interface(unsigned long baud) : SerialDebugMIDI_Interface(Serial, baud) {}
};
#endif

#if defined(__AVR__) || defined(CORE_TEENSY)
#include <SoftwareSerial.h>
class SoftwarSerialDebugMIDI_Interface : public SerialDebugMIDI_Interface<SoftwareSerial>
{
public:
  SoftwarSerialDebugMIDI_Interface(SoftwareSerial &serial, unsigned long baud) : SerialDebugMIDI_Interface(serial, baud) {}
};
#endif

#endif // DEBUGMIDI_INTERFACE_H_