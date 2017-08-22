#ifndef SERIALMIDIOUTPUT_H_
#define SERIALMIDIOUTPUT_H_

#include "MIDIOutput.h"

class StreamMIDIOutput : public MIDIOutput
{
  public:
    StreamMIDIOutput(Stream &stream) : stream(stream) {}

  protected:
    void sendImpl(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2)
    {
        stream.write(m | c); // Send the MIDI message over the stream
        stream.write(d1);
        stream.write(d2);
        stream.flush();
    }
    void sendImpl(uint8_t m, uint8_t c, uint8_t d1)
    {
        stream.write(m | c); // Send the MIDI message over the stream
        stream.write(d1);
        stream.flush();
    }

  private:
    Stream &stream;
};

template <typename T>
class SerialMIDIOutput : public StreamMIDIOutput
{
  public:
    SerialMIDIOutput(T &serial, unsigned long baud) : serial(serial), baud(baud), StreamMIDIOutput(serial) {}
    void begin()
    {
        serial.begin(baud);
    }

  private:
    T &serial;
    const unsigned long baud;
};

class HardwareSerialMIDIOutput : public SerialMIDIOutput<HardwareSerial>
{
  public:
    HardwareSerialMIDIOutput(HardwareSerial &serial, unsigned long baud) : SerialMIDIOutput(serial, baud) {}
};


// The Serial USB connection
// (Why do I have to do this, why don't they all inherit from one single class?)

#if !(defined(USBCON) || defined(CORE_TEENSY)) // Boards without a USB connection (UNO, MEGA, Nano ...)
class USBSerialMIDIOutput : public HardwareSerialMIDIOutput
{
  public:
    USBSerialMIDIOutput(unsigned long baud) : HardwareSerialMIDIOutput(Serial, baud) {}
};
#elif defined(CORE_TEENSY) // Teensies
class USBSerialMIDIOutput : public SerialMIDIOutput<usb_serial_class>
{
  public:
    USBSerialMIDIOutput(unsigned long baud) : SerialMIDIOutput(Serial, baud) {}
};
#elif defined(ARDUINO_ARCH_SAM) // Arduino DUE
class USBSerialMIDIOutput : public SerialMIDIOutput<UARTClass>
{
  public:
    USBSerialMIDIOutput(unsigned long baud) : SerialMIDIOutput(Serial, baud) {}
};
#else // Others (Leonardo, Micro ... )
class USBSerialMIDIOutput : public SerialMIDIOutput<Serial_>
{
  public:
    USBSerialMIDIOutput(unsigned long baud) : SerialMIDIOutput(Serial, baud) {}
};
#endif

// Class for Hairless MIDI
class HairlessMIDIOutput : public USBSerialMIDIOutput
{
    HairlessMIDIOutput() : USBSerialMIDIOutput(HAIRLESS_BAUD) {};
};

#if defined(__AVR__) || defined(CORE_TEENSY)
#include <SoftwareSerial.h>
class SoftwarSerialMIDIOutput : public SerialMIDIOutput<SoftwareSerial>
{
  public:
    SoftwarSerialMIDIOutput(SoftwareSerial &serial, unsigned long baud) : SerialMIDIOutput(serial, baud) {}
};
#endif

#endif // SERIALMIDIOUTPUT_H_