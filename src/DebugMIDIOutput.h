#ifndef DEBUGMIDIOUTPUT_H_
#define DEBUGMIDIOUTPUT_H_
#include "MIDIOutput.h"

const char *MIDI_STATUS_TYPE_NAMES[] = {"Note Off\t",
                                        "Note On\t\t",
                                        "Key Pressure\t",
                                        "Control Change\t",
                                        "Program Change\t",
                                        "Channel Pressure",
                                        "Pitch Bend\t"};

class StreamDebugMIDIOutput : public MIDIOutput
{
  public:
    StreamDebugMIDIOutput(Stream &stream) : stream(stream) {}

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

  private:
    Stream &stream;
};

template <typename T>
class SerialDebugMIDIOutput : public StreamDebugMIDIOutput
{
  public:
    SerialDebugMIDIOutput(T &serial, unsigned long baud) : serial(serial), baud(baud), StreamDebugMIDIOutput(serial) {}
    void begin()
    {
        serial.begin(baud);
    }

  private:
    T &serial;
    const unsigned long baud;
};

class HardwareSerialDebugMIDIOutput : public SerialDebugMIDIOutput<HardwareSerial>
{
  public:
    HardwareSerialDebugMIDIOutput(HardwareSerial &serial, unsigned long baud) : SerialDebugMIDIOutput(serial, baud) {}
};

// The Serial USB connection
// (Why do I have to do this, why don't they all inherit from one single class?)

#if !(defined(USBCON) || defined(CORE_TEENSY)) // Boards without a USB connection (UNO, MEGA, Nano ...)
class USBDebugMIDIOutput : public HardwareSerialDebugMIDIOutput
{
  public:
    USBDebugMIDIOutput(unsigned long baud) : HardwareSerialDebugMIDIOutput(Serial, baud) {}
};
#elif defined(CORE_TEENSY)      // Teensies
class USBDebugMIDIOutput : public SerialDebugMIDIOutput<usb_serial_class>
{
  public:
    USBDebugMIDIOutput(unsigned long baud) : SerialDebugMIDIOutput(Serial, baud) {}
};
#elif defined(ARDUINO_ARCH_SAM) // Arduino DUE
class USBDebugMIDIOutput : public SerialDebugMIDIOutput<UARTClass>
{
  public:
    USBDebugMIDIOutput(unsigned long baud) : SerialDebugMIDIOutput(Serial, baud) {}
};
#else                           // Others (Leonardo, Micro ... )
class USBDebugMIDIOutput : public SerialDebugMIDIOutput<Serial_>
{
  public:
    USBDebugMIDIOutput(unsigned long baud) : SerialDebugMIDIOutput(Serial, baud) {}
};
#endif

#if defined(__AVR__) || defined(CORE_TEENSY)
#include <SoftwareSerial.h>
class SoftwarSerialDebugMIDIOutput : public SerialDebugMIDIOutput<SoftwareSerial>
{
  public:
    SoftwarSerialDebugMIDIOutput(SoftwareSerial &serial, unsigned long baud) : SerialDebugMIDIOutput(serial, baud) {}
};
#endif

#endif // DEBUGMIDIOUTPUT_H_