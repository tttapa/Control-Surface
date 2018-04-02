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

  virtual MIDI_read_t read()
  {
    while (stream.available() > 0)
    {
      char data = stream.read();

      if (isHexChar(toLowerCase(data)))
      {
        data = toLowerCase(data);
        if (firstChar == '\0')
          firstChar = data;
        else if (secondChar == '\0')
          secondChar = data;
        else
        {
          firstChar = secondChar;
          secondChar = data;
        }
      }
      else if (isWhiteSpace(data) && firstChar && secondChar) // if we received two hex characters followed by whitespace
      {
        uint8_t midiByte = hexCharToNibble(firstChar) << 4 | hexCharToNibble(secondChar);
#ifdef DEBUG
        Serial.print("New byte:\t");
        Serial.println(midiByte, HEX);
#endif
        firstChar = '\0';
        secondChar = '\0';
        MIDI_read_t parseResult = parser.parse(midiByte);
        if (parseResult != NO_MESSAGE)
          return parseResult;
      }
    }
    return NO_MESSAGE;
  }

private:
  char firstChar;
  char secondChar;

  bool isHexChar(char hex) // check if a given character is a hexadecimal number (0-9 or a-f)
  {
    return (hex >= '0' && hex <= '9') || (hex >= 'a' && hex <= 'f');
  }
  char toLowerCase(char x) // convert a given letter to lower case, no effect if it's a digit (0 - 9) or if it's already lower case
  {
    return x | 0b0100000;
  }
  uint8_t hexCharToNibble(char hex) // convert a hexadecimal character to a 4-bit nibble
  {
    return hex < 'a' ? hex - '0' : hex - 'a' + 10;
  }
  inline bool isWhiteSpace(char x)
  {
    return x == ' ' || x == '\r' || x == '\n';
  }
};

template <typename T>
class SerialDebugMIDI_Interface : public StreamDebugMIDI_Interface
{
public:
  SerialDebugMIDI_Interface(T &serial, unsigned long baud) : StreamDebugMIDI_Interface(serial), serial(serial), baud(baud) {}
  virtual void begin()
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