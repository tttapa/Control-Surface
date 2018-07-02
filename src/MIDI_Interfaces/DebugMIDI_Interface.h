#ifndef DEBUGMIDI_INTERFACE_H_
#define DEBUGMIDI_INTERFACE_H_

#include "SerialMIDI_Interface.h"

const static char *MIDI_STATUS_TYPE_NAMES[] = { // TODO: PROGMEM
    "Note Off\t",       "Note On\t\t",      "Key Pressure\t",
    "Control Change\t", "Program Change\t", "Channel Pressure",
    "Pitch Bend\t"};

class StreamDebugMIDI_Interface : public StreamMIDI_Interface {
public:
  StreamDebugMIDI_Interface(Stream &stream) : StreamMIDI_Interface(stream) {}

protected:
  void sendImpl(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2) {
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
    stream.flush();
  }
  void sendImpl(uint8_t m, uint8_t c, uint8_t d1) {
    uint8_t messageType = (m >> 4) - 8;
    if (messageType >= 7)
      return;
    stream.print(MIDI_STATUS_TYPE_NAMES[messageType]);
    stream.print("\tChannel: ");
    stream.print(c + 1);
    stream.print("\tData 1: 0x");
    stream.print(d1, HEX);
    stream.print("\r\n");
    stream.flush();
  }

  virtual MIDI_read_t read() {
    while (stream.available() > 0) {
      char data = stream.read();

      if (isHexChar(toLowerCase(data))) {
        data = toLowerCase(data);
        if (firstChar == '\0')
          firstChar = data;
        else if (secondChar == '\0')
          secondChar = data;
        else {
          firstChar = secondChar;
          secondChar = data;
        }
      } else if (isWhiteSpace(data) && firstChar && secondChar) {
        // if we received two hex characters followed by
        // whitespace
        uint8_t midiByte =
            hexCharToNibble(firstChar) << 4 | hexCharToNibble(secondChar);
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

  /**
   * @brief Check if a given character is a hexadecimal number (0-9 or a-f)
   */
  bool isHexChar(char hex) {
    return (hex >= '0' && hex <= '9') || (hex >= 'a' && hex <= 'f');
  }
  /**
   * @brief Convert a given letter to lower case, no effect if
   * it's a digit (0 - 9) or if it's already lower case.
   */
  char toLowerCase(char x) { return x | 0b0100000; }
  /**
   * @brief Convert a hexadecimal character to a 4-bit nibble.
   */
  uint8_t hexCharToNibble(char hex) {
    return hex < 'a' ? hex - '0' : hex - 'a' + 10;
  }
  inline bool isWhiteSpace(char x) {
    return x == ' ' || x == '\r' || x == '\n';
  }
};

template <typename T>
class SerialDebugMIDI_Interface : public StreamDebugMIDI_Interface {
public:
  SerialDebugMIDI_Interface(T &serial, unsigned long baud)
      : StreamDebugMIDI_Interface(serial), serial(serial), baud(baud) {}
  virtual void begin() { serial.begin(baud); }

private:
  T &serial;
  const unsigned long baud;
};

class HardwareSerialDebugMIDI_Interface
    : public SerialDebugMIDI_Interface<HardwareSerial> {
public:
  HardwareSerialDebugMIDI_Interface(HardwareSerial &serial, unsigned long baud)
      : SerialDebugMIDI_Interface(serial, baud) {}
};

// The Serial USB connection
// (Why do I have to do this, why don't they all inherit from one single class?)

 // Boards without a USB connection (UNO, MEGA, Nano ...)
#if !( defined(USBCON) || defined(CORE_TEENSY))
class USBDebugMIDI_Interface : public HardwareSerialDebugMIDI_Interface {
public:
  USBDebugMIDI_Interface(unsigned long baud)
      : HardwareSerialDebugMIDI_Interface(Serial, baud) {}
};

// Teensies
#elif defined(CORE_TEENSY)
class USBDebugMIDI_Interface
    : public SerialDebugMIDI_Interface<usb_serial_class> {
public:
  USBDebugMIDI_Interface(unsigned long baud)
      : SerialDebugMIDI_Interface(Serial, baud) {}
};

// Arduino DUE
#elif defined(ARDUINO_ARCH_SAM)
class USBDebugMIDI_Interface : public SerialDebugMIDI_Interface<UARTClass> {
public:
  USBDebugMIDI_Interface(unsigned long baud)
      : SerialDebugMIDI_Interface(Serial, baud) {}
};

// Others (Leonardo, Micro ... )
#else
class USBDebugMIDI_Interface : public SerialDebugMIDI_Interface<Serial_> {
public:
  USBDebugMIDI_Interface(unsigned long baud)
      : SerialDebugMIDI_Interface(Serial, baud) {}
};
#endif

#if defined(__AVR__) || defined(CORE_TEENSY)
#include <SoftwareSerial.h>
class SoftwareSerialDebugMIDI_Interface
    : public SerialDebugMIDI_Interface<SoftwareSerial> {
public:
  SoftwareSerialDebugMIDI_Interface(SoftwareSerial &serial, unsigned long baud)
      : SerialDebugMIDI_Interface(serial, baud) {}
};
#endif

#endif // DEBUGMIDI_INTERFACE_H_