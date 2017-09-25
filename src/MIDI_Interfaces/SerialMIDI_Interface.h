#ifndef SERIALMIDI_INTERFACE_H_
#define SERIALMIDI_INTERFACE_H_

#include "MIDI_Interface.h"

class StreamMIDI_Interface : public MIDI_Interface
{
public:
  StreamMIDI_Interface(Stream &stream) : stream(stream) {}

#ifndef NO_MIDI_INPUT
  virtual bool refresh()
  {
    if (handlePreviousByte)
    {
      parseSingleMIDIByte(midiByte);
      handlePreviousByte = false;
    }

    if (stream.available() <= 0)
      return false;

    midiByte = stream.read();
#ifdef DEBUG
    Serial.printf("New byte:\t%02Xh\r\n", midiByte);
#endif
    if (!parseSingleMIDIByte(midiByte)) // if the MIDI buffer is full
    {
      handlePreviousByte = true; // handle this byte next time (after emptying the buffer), before reading the next byte
      return false;
    }

    return true;
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

  uint8_t runningStatusBuffer = 0;
  uint8_t midiByte = 0;
  bool thirdByte = false;
  bool handlePreviousByte = false;

  bool parseSingleMIDIByte(uint8_t midiByte)
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
        if (runningStatusBuffer == SysExStart) // if we're currently receiving a SysEx message
        {
#ifdef DEBUG
          Serial.println("SysExEnd");
#endif
          if (SysExLength >= bufferSize) // SysEx is larger than the buffer
          {
            startMessage(); // Discard message
#ifdef DEBUG
            Serial.println("SysEx is larger than buffer");
#endif
            runningStatusBuffer = SysExEnd;
            return true; // ignore byte
          }
          if (!addToMessage(SysExEnd)) // add SysExEnd byte to buffer
            return false;              // if it failed, return, buffer is full, try same byte again after parsing the buffer
          finishMessage();
        }
#endif
#ifdef DEBUG
        Serial.println("Header");
#endif
        runningStatusBuffer = midiByte;
        thirdByte = false;

#ifndef IGNORE_SYSEX
        if (runningStatusBuffer == SysExStart) // if the first byte of a SysEx message
        {
#ifdef DEBUG
          Serial.println("SysExStart");
#endif
          startMessage();
          if (!addToMessage(SysExStart)) // if the buffer is full
            return false;                // return, but read same byte again next time (after emptying the buffer)
          SysExLength = 1;
        }
#endif // IGNORE_SYSEX
      }
    }
    else // If it's a data byte
    {
      if (thirdByte) // third data byte of three
      {
#ifdef DEBUG
        Serial.println("Second data byte");
#endif
        if (!addToMessage(midiByte)) // if the buffer is full
          return false;              // return, but read same byte again next time (after emptying the buffer)
        finishMessage();
#ifdef DEBUG
        Serial.println("Message finished");
#endif
        thirdByte = false;
      }
      else // second byte or SysEx data
      {
        if (runningStatusBuffer == 0)
        {
#ifdef DEBUG
          Serial.println("Error: No header");
#endif
          ; // Ignore
        }
        else if (runningStatusBuffer < 0xC0 || runningStatusBuffer == 0xE0) // Note, Aftertouch, CC or Pitch Bend
        {
#ifdef DEBUG
          Serial.println("First data byte (of two)");
#endif

          if (!hasSpaceLeft(2))              // if the buffer is full
            return false;                    // return, but read same byte again next time (after emptying the buffer)
          addToMessage(runningStatusBuffer); // add the header to the buffer
          addToMessage(midiByte);            // add the first data byte to the buffer
          thirdByte = true;                  // message is not finished yet
        }
        else if (runningStatusBuffer < 0xE0) // Program Change or Channel Pressure
        {
#ifdef DEBUG
          Serial.println("First data byte");
#endif
          if (!hasSpaceLeft(2))              // if the buffer is full
            return false;                    // return, but read same byte again next time (after emptying the buffer)
          addToMessage(runningStatusBuffer); // add the header to the buffer
          addToMessage(midiByte);            // add the data byte to the buffer
#ifdef DEBUG
          Serial.println("Message finished");
#endif
          finishMessage();
        }
#ifndef IGNORE_SYSEX
        else if (runningStatusBuffer == SysExStart) // SysEx data byte
        {
#ifdef DEBUG
          Serial.println("SysEx data byte");
#endif
          if (SysExLength >= bufferSize) // SysEx is larger than the buffer
          {
            startMessage(); // Discard message
#ifdef DEBUG
            Serial.println("SysEx is larger than buffer");
#endif
            return true;
          }
          if (!addToMessage(midiByte)) // add data byte to buffer
            return false;              // if it failed, return, buffer is full, parse same byte again after parsing the buffer
          SysExLength += 1;
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
    return true; // successfully added to buffer, continue with next MIDI byte
  }

#endif // #ifndef NO_MIDI_INPUT
};

template <typename T>
class SerialMIDI_Interface : public StreamMIDI_Interface
{
public:
  SerialMIDI_Interface(T &serial, unsigned long baud) : serial(serial), baud(baud), StreamMIDI_Interface(serial) {}
  void begin()
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