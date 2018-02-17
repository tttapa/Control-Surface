#ifndef SERIALMIDI_INTERFACE_H_
#define SERIALMIDI_INTERFACE_H_

#include "MIDI_Interface.h"

class StreamMIDI_Interface : public MIDI_Interface
{
public:
  StreamMIDI_Interface(Stream &stream) : stream(stream) {}

#ifndef NO_MIDI_INPUT
  virtual MIDI_read_t read()
  {
    while (stream.available() > 0)
    {
      uint8_t midiByte = stream.read();
#ifdef DEBUG
      Serial.print("New byte:\t");
      Serial.println(midiByte, HEX);
#endif
      MIDI_read_t parseResult = parseSingleMIDIByte(midiByte);
      if (parseResult != NO_MESSAGE)
        return parseResult;
    }
    return NO_MESSAGE;
  }
#endif // #ifndef NO_MIDI_INPUT

protected:
  virtual void sendImpl(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2)
  {
    stream.write(m | c); // Send the MIDI message over the stream
    stream.write(d1);
    stream.write(d2);
    stream.flush();
  }
  virtual void sendImpl(uint8_t m, uint8_t c, uint8_t d1)
  {
    stream.write(m | c); // Send the MIDI message over the stream
    stream.write(d1);
    stream.flush();
  }

protected:
  Stream &stream;

#ifndef NO_MIDI_INPUT

  bool thirdByte = false;

  MIDI_read_t parseSingleMIDIByte(uint8_t midiByte)
  {
    if (midiByte & (1 << 7)) // If it's a header byte (first byte)
    {
      if ((midiByte & 0xF8) == 0xF8) // If it's a Real-Time message (not implemented)
      {
        ; // Handle Real-Time stuff
#ifdef DEBUG
        Serial.println("Real-Time");
#endif
      }
      else // Normal header
      {
#ifndef IGNORE_SYSEX
        if (midimsg.header == SysExStart) // if we're currently receiving a SysEx message
        {
#ifdef DEBUG
          Serial.println("SysExEnd");
#endif
          midimsg.header = SysExEnd;
          if (addSysExByte(SysExEnd)) // add SysExEnd byte to buffer
            return SYSEX_MESSAGE;
        }
#endif
#ifdef DEBUG
        Serial.println("Header");
#endif
        midimsg.header = midiByte;
        thirdByte = false;

#ifndef IGNORE_SYSEX
        if (midimsg.header == SysExStart) // if the first byte of a SysEx message
        {
#ifdef DEBUG
          Serial.println("SysExStart");
#endif
          startSysEx();
        }
#endif // IGNORE_SYSEX
      }
    }
    else // If it's a data byte
    {
      if (midimsg.header == 0)
      {
#ifdef DEBUG
        Serial.println("Error: No header");
#endif
        ; // Ignore
      }
      else if (thirdByte) // third byte of three
      {
#ifdef DEBUG
        Serial.println("Second data byte");
#endif
        midimsg.data2 = midiByte;
#ifdef DEBUG
        Serial.println("Message finished");
#endif
        thirdByte = false;
        return CHANNEL_MESSAGE;
      }
      else // second byte or SysEx data
      {
        if (midimsg.header < 0xC0 || midimsg.header == 0xE0) // Note, Aftertouch, CC or Pitch Bend
        {
#ifdef DEBUG
          Serial.println("First data byte (of two)");
#endif
          midimsg.data1 = midiByte;
          thirdByte = true;
        }
        else if (midimsg.header < 0xE0) // Program Change or Channel Pressure
        {
#ifdef DEBUG
          Serial.println("First data byte");
#endif
          midimsg.data1 = midiByte;
          return CHANNEL_MESSAGE;
        }
#ifndef IGNORE_SYSEX
        else if (midimsg.header == SysExStart) // SysEx data byte
        {
#ifdef DEBUG
          Serial.println("SysEx data byte");
#endif
          addSysExByte(midiByte);
        }
#endif // IGNORE_SYSEX
#ifdef DEBUG
        else
        {
          Serial.println("Data byte ignored");
        }
#endif
      }
    }
    return NO_MESSAGE;
  }

#endif // #ifndef NO_MIDI_INPUT
};

template <typename T>
class SerialMIDI_Interface : public StreamMIDI_Interface
{
public:
  SerialMIDI_Interface(T &serial, unsigned long baud) : serial(serial), baud(baud), StreamMIDI_Interface(serial) {}
  virtual void begin()
  {
    serial.begin(baud);
  }

private:
  T &serial;
  const unsigned long baud;
};

class HardwareSerialMIDI_Interface : public SerialMIDI_Interface<HardwareSerial>
{
public:
  HardwareSerialMIDI_Interface(HardwareSerial &serial, unsigned long baud) : SerialMIDI_Interface(serial, baud) {}
};

// The Serial USB connection
// (Why do I have to do this, why don't they all inherit from one single class?)

#if !(defined(USBCON) || defined(CORE_TEENSY)) // Boards without a USB connection (UNO, MEGA, Nano ...)
class USBSerialMIDI_Interface : public HardwareSerialMIDI_Interface
{
public:
  USBSerialMIDI_Interface(unsigned long baud) : HardwareSerialMIDI_Interface(Serial, baud) {}
};
#elif defined(CORE_TEENSY)      // Teensies
class USBSerialMIDI_Interface : public SerialMIDI_Interface<usb_serial_class>
{
public:
  USBSerialMIDI_Interface(unsigned long baud) : SerialMIDI_Interface(Serial, baud) {}
};
#elif defined(ARDUINO_ARCH_SAM) // Arduino DUE
class USBSerialMIDI_Interface : public SerialMIDI_Interface<UARTClass>
{
public:
  USBSerialMIDI_Interface(unsigned long baud) : SerialMIDI_Interface(Serial, baud) {}
};
#else                           // Others (Leonardo, Micro ... )
class USBSerialMIDI_Interface : public SerialMIDI_Interface<Serial_>
{
public:
  USBSerialMIDI_Interface(unsigned long baud) : SerialMIDI_Interface(Serial, baud) {}
};
#endif

// Class for Hairless MIDI
class HairlessMIDI_Interface : public USBSerialMIDI_Interface
{
  HairlessMIDI_Interface() : USBSerialMIDI_Interface(HAIRLESS_BAUD){};
};

#if defined(__AVR__) || defined(CORE_TEENSY)
#include <SoftwareSerial.h>
class SoftwarSerialMIDI_Interface : public SerialMIDI_Interface<SoftwareSerial>
{
public:
  SoftwarSerialMIDI_Interface(SoftwareSerial &serial, unsigned long baud) : SerialMIDI_Interface(serial, baud) {}
};
#endif

#endif // SERIALMIDI_INTERFACE_H_