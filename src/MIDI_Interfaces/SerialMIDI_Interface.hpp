#pragma once

#include "MIDI_Interface.hpp"
#include <AH/Arduino-Wrapper.h> // Stream
#include <AH/STL/utility>
#include <AH/Teensy/TeensyUSBTypes.hpp>
#include <MIDI_Parsers/SerialMIDI_Parser.hpp>
#include <Settings/SettingsWrapper.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   A class for MIDI interfaces sending and receiving MIDI messages
 *          over a Stream.
 * 
 * @ingroup MIDIInterfaces
 */
class StreamMIDI_Interface : public MIDI_Interface {
  public:
    /// Constructor.
    /// @param   stream
    ///          Reference to the Stream interface to read and send MIDI data
    ///          from/to.
    StreamMIDI_Interface(Stream &stream) : stream(stream) {}

    /// Try reading and parsing a single incoming MIDI message.
    /// @return  Returns the type of the read message, or
    ///          `MIDIReadEvent::NO_MESSAGE` if no MIDI message was available.
    MIDIReadEvent read();

    /// Return the received channel voice message.
    ChannelMessage getChannelMessage() const;
    /// Return the received system common message.
    SysCommonMessage getSysCommonMessage() const;
    /// Return the received real-time message.
    RealTimeMessage getRealTimeMessage() const;
    /// Return the received system exclusive message.
    SysExMessage getSysExMessage() const;

    void update() override;

  protected:
    void sendChannelMessageImpl(ChannelMessage) override;
    void sendSysCommonImpl(SysCommonMessage) override;
    void sendSysExImpl(SysExMessage) override;
    void sendRealTimeImpl(RealTimeMessage) override;
    void sendNowImpl() override {}

  protected:
    void handleStall() override;

  protected:
    Stream &stream;
    SerialMIDI_Parser parser;
};

// -------------------------------------------------------------------------- //

/**
 * @brief   A wrapper class for MIDI interfaces sending and receiving
 *          MIDI messages over a Serial port of generic class S.
 *
 * @note    This is a template class because the type of the Serial object
 *          is completely different on different architectures, and they
 *          do not share a common super-class that has a `begin` method.
 * 
 * @ingroup MIDIInterfaces
 */
template <class S>
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
    SerialMIDI_Interface(S &serial, unsigned long baud = MIDI_BAUD)
        : StreamMIDI_Interface(serial), baud(baud) {}

    /**
     * @brief   Start the Serial interface at the predefined baud rate.
     */
    void begin() override { static_cast<S &>(stream).begin(baud); }

  private:
    const unsigned long baud;
};

// -------------------------------------------------------------------------- //

/**
 * @brief   A class for MIDI interfaces sending and receiving
 *          MIDI messages over a Hardware Serial port.
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
    HardwareSerialMIDI_Interface(HardwareSerial &serial,
                                 unsigned long baud = MIDI_BAUD)
        : SerialMIDI_Interface(serial, baud) {}
};

// -------------------------------------------------------------------------- //

/**
 * @brief   A class for MIDI interfaces sending and receiving
 *          MIDI messages over the Serial port of the USB connection.
 *
 * @ingroup MIDIInterfaces
 */
class USBSerialMIDI_Interface : public SerialMIDI_Interface<decltype(Serial)> {
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

// -------------------------------------------------------------------------- //

#if !defined(TEENSYDUINO) ||                                                   \
    (defined(TEENSYDUINO) && defined(TEENSY_SERIALUSB_ENABLED))
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
    HairlessMIDI_Interface(unsigned long baud = HAIRLESS_BAUD)
        : USBSerialMIDI_Interface(baud){};
};
#endif

END_CS_NAMESPACE