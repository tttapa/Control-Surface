#ifndef DEBUGMIDI_INTERFACE_H_
#define DEBUGMIDI_INTERFACE_H_
#include "MIDI_Interface.h"

const char *MIDI_STATUS_TYPE_NAMES[] = {"Note Off\t",
                                        "Note On\t\t",
                                        "Key Pressure\t",
                                        "Control Change\t",
                                        "Program Change\t",
                                        "Channel Pressure",
                                        "Pitch Bend\t"};

class StreamDebugMIDI_Interface : public MIDI_Interface
{
public:
  StreamDebugMIDI_Interface(Stream &stream) : stream(stream) {}

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
  bool refresh()
  {
    if (handlePreviousByte)
    {
      parseSingleMIDIByte(midiByte);
      handlePreviousByte = false;
    }

    {
      if (stream.available() <= 0)
        return false;

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
        midiByte = hexCharToNibble(firstChar) << 4 | hexCharToNibble(secondChar);
        Serial.printf("New byte:\t%02Xh\r\n", midiByte);
        firstChar = '\0';
        secondChar = '\0';
        if (!parseSingleMIDIByte(midiByte)) // if the MIDI buffer is full
        {
          handlePreviousByte = true; // handle this byte next time (after emptying the buffer), before reading the next byte
          return false;
        }
      }
    }
    return true;
  }

private:
  Stream &stream;
  bool thirdByte = false;
  char firstChar;
  char secondChar;
  uint8_t runningStatusBuffer = 0;
  uint8_t midiByte = 0;
  bool handlePreviousByte = false;

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
  bool isWhiteSpace(char x)
  {
    return x == ' ' || x == '\r' || x == '\n';
  }

  bool parseSingleMIDIByte(uint8_t midiByte)
  {
    if (midiByte & (1 << 7)) // If it's a header byte (first byte)
    {
      if ((midiByte & 0xF8) == 0xF8) // If it's a Real-Time message (not implemented)
      {
        ; // Handle Real-Time stuff
        Serial.println("Real-Time");
      }
      else // Normal header
      {
        if (runningStatusBuffer == SysExStart) // if we're currently receiving a SysEx message
        {
          Serial.println("SysExEnd");
          if (SysExLength >= bufferSize) // SysEx is larger than the buffer
          {
            startMessage(); // Discard message
            Serial.println("SysEx is larger than buffer");
            runningStatusBuffer = SysExEnd;
            return true; // ignore byte
          }
          if (!addToMessage(SysExEnd)) // add SysExEnd byte to buffer
            return false;              // if it failed, return, buffer is full, try same byte again after parsing the buffer
          finishMessage();
        }
        Serial.println("Header");
        runningStatusBuffer = midiByte;
        thirdByte = false;
        if (runningStatusBuffer == SysExStart) // if the first byte of a SysEx message
        {
          Serial.println("SysExStart");
          startMessage();
          if (!addToMessage(SysExStart)) // if the buffer is full
            return false;                // return, but read same byte again next time (after emptying the buffer)
          SysExLength = 1;
        }
      }
    }
    else // If it's a data byte
    {
      if (thirdByte) // third data byte of three
      {
        Serial.println("Second data byte");
        if (!addToMessage(midiByte)) // if the buffer is full
          return false;              // return, but read same byte again next time (after emptying the buffer)
        finishMessage();
        Serial.println("Message finished");
        thirdByte = false;
      }
      else // second byte or SysEx data
      {
        if (runningStatusBuffer == 0)
        {
          Serial.println("Error: No header");
          ; // Ignore
        }
        else if (runningStatusBuffer < 0xC0 || runningStatusBuffer == 0xE0) // Note, Aftertouch, CC or Pitch Bend
        {
          Serial.println("First data byte (of two)");

          if (!hasSpaceLeft(2))              // if the buffer is full
            return false;                    // return, but read same byte again next time (after emptying the buffer)
          addToMessage(runningStatusBuffer); // add the header to the buffer
          addToMessage(midiByte);            // add the first data byte to the buffer
          thirdByte = true;                  // message is not finished yet
        }
        else if (runningStatusBuffer < 0xE0) // Program Change or Channel Pressure
        {
          Serial.println("First data byte");
          if (!hasSpaceLeft(2))              // if the buffer is full
            return false;                    // return, but read same byte again next time (after emptying the buffer)
          addToMessage(runningStatusBuffer); // add the header to the buffer
          addToMessage(midiByte);            // add the data byte to the buffer
          Serial.println("Message finished");
          finishMessage();
        }
        else if (runningStatusBuffer == SysExStart) // SysEx data byte
        {
          ; // add data to SysEx buffer
          Serial.println("SysEx data byte");

          if (SysExLength >= bufferSize) // SysEx is larger than the buffer
          {
            startMessage(); // Discard message
            Serial.println("SysEx is larger than buffer");
            return true;
          }
          if (!addToMessage(midiByte)) // add data byte to buffer
            return false;              // if it failed, return, buffer is full, parse same byte again after parsing the buffer
          SysExLength += 1;
        }
        else
        {
          Serial.println("Data byte ignored");
        }
      }
    }
    return true; // successfully added to buffer, continue with next MIDI byte
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