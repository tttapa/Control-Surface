#pragma once

#include "MIDI_Interface.hpp"
#include <AH/Teensy/TeensyUSBTypes.hpp>
#include <MIDI_Parsers/HexPuller.hpp>
#include <MIDI_Parsers/SerialMIDI_Parser.hpp>
#include <MIDI_Parsers/StreamPuller.hpp>
#include <Settings/SettingsWrapper.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief A class that implements MIDI debug send functions.
 */
class PrintDebugMIDI_Base {
  protected:
    void sendChannelMessageImpl(Print &, ChannelMessage);
    void sendSysCommonImpl(Print &, SysCommonMessage);
    void sendSysExImpl(Print &, SysExMessage);
    void sendRealTimeImpl(Print &, RealTimeMessage);
    void sendNowImpl(Print &) {}

  public:
    PrintDebugMIDI_Base(const char *prefix) : prefix(prefix) {}

    /// Set the prefix to be printed before each message.
    void setPrefix(const char *prefix) { this->prefix = prefix; }
    const char *getPrefix() const { return prefix; }

  private:
    const char *prefix = nullptr;
};

/**
 * @brief   A class for MIDI outputs sending human-readable MIDI messages
 *          over a Stream.
 * 
 * @ingroup MIDIInterfaces
 */
class StreamDebugMIDI_Output : public PrintDebugMIDI_Base,
                               public MIDI_Sender<StreamDebugMIDI_Output>,
                               public TrueMIDI_Sink {
  public:
    StreamDebugMIDI_Output(Print &stream, const char *prefix = nullptr)
        : PrintDebugMIDI_Base(prefix), stream(stream) {}

    Print &getStream() const { return stream; }

  protected:
    void sendChannelMessageImpl(ChannelMessage);
    void sendSysCommonImpl(SysCommonMessage);
    void sendSysExImpl(SysExMessage);
    void sendRealTimeImpl(RealTimeMessage);
    void sendNowImpl();

#if !DISABLE_PIPES
    void sinkMIDIfromPipe(ChannelMessage m) override { send(m); }
    void sinkMIDIfromPipe(SysExMessage m) override { send(m); }
    void sinkMIDIfromPipe(SysCommonMessage m) override { send(m); }
    void sinkMIDIfromPipe(RealTimeMessage m) override { send(m); }
#endif

    Print &stream;

    friend MIDI_Sender<StreamDebugMIDI_Output>;
};

class MIDIOutputOnly : public AH::UpdatableCRTP<MIDIOutputOnly> {
  public:
    virtual ~MIDIOutputOnly() = default;
    virtual void begin() = 0;
    static void beginAll() { applyToAll(&MIDIOutputOnly::begin); }
};

/**
 * @brief   A wrapper class for debug MIDI outputs sending 
 *          human-readable MIDI messages over a Serial port of class T.
 * 
 * @note    This is a template class because the class of the Serial object
 *          is completely different on different architectures, and they
 *          do not share a common super-class that has a `begin` method.
 * 
 * @ingroup MIDIInterfaces
 */
template <typename T>
class SerialDebugMIDI_Output : public MIDIOutputOnly,
                               public StreamDebugMIDI_Output {
  public:
    /**
     * @brief   Construct a new Debug MIDI Output on the given Serial 
     *          interface with the given baud rate.
     * 
     * @param   serial
     *          The Serial interface.
     * @param   baud
     *          The baud rate for the Serial interface.
     * @param   prefix
     *          An optional string to print before each message.
     */
    SerialDebugMIDI_Output(T &serial, unsigned long baud = AH::defaultBaudRate,
                           const char *prefix = nullptr)
        : StreamDebugMIDI_Output(serial, prefix), baud(baud) {}
    /// @see @ref SerialDebugMIDI_Output(T &,unsigned long,const char *)
    SerialDebugMIDI_Output(T &serial, const char *prefix)
        : SerialDebugMIDI_Output(serial, AH::defaultBaudRate, prefix) {}

    /**
     * @brief   Start the Serial interface at the predefined baud rate.
     */
    void begin() override { static_cast<T &>(getStream()).begin(baud); }

  private:
    const unsigned long baud;
};

/**
 * @brief   A class for debug MIDI outputs sending 
 *          human-readable MIDI messages over a HardwareSerial port.
 * 
 * @ingroup MIDIInterfaces
 */
class HardwareSerialDebugMIDI_Output
    : public SerialDebugMIDI_Output<HardwareSerial> {
  public:
    /**
     * @brief   Construct a new Debug MIDI Output on the given HardwareSerial
     *          interface with the given baud rate.
     * 
     * @param   serial
     *          The HardwareSerial interface.
     * @param   baud
     *          The baud rate for the serial interface.
     * @param   prefix
     *          An optional string to print before each message.
     */
    HardwareSerialDebugMIDI_Output(HardwareSerial &serial,
                                   unsigned long baud = AH::defaultBaudRate,
                                   const char *prefix = nullptr)
        : SerialDebugMIDI_Output(serial, baud, prefix) {}
    /// @see @ref HardwareSerialDebugMIDI_Output(HardwareSerial &,unsigned long,const char *)
    HardwareSerialDebugMIDI_Output(HardwareSerial &serial, const char *prefix)
        : SerialDebugMIDI_Output(serial, prefix) {}
};

/**
 * @brief   A class for debug MIDI outputs sending 
 *          human-readable MIDI messages over the USB CDC connection.
 * 
 *          Boards without a native USB connection (UNO, MEGA, Nano ...)
 *          use HardwareSerial0 for USB communication.
 * 
 * @ingroup MIDIInterfaces
 */
class USBDebugMIDI_Output : public SerialDebugMIDI_Output<decltype(Serial)> {
  public:
    /**
     * @brief   Construct a USBDebugMIDI_Output with the given baud rate.
     * 
     * @param   baud
     *          The baud rate to start the USB Serial connection with.
     * @param   prefix
     *          An optional string to print before each message.
     */
    USBDebugMIDI_Output(unsigned long baud = AH::defaultBaudRate,
                        const char *prefix = nullptr)
        : SerialDebugMIDI_Output(Serial, baud, prefix) {}
    /// @see @ref USBDebugMIDI_Output(unsigned long,const char *)
    USBDebugMIDI_Output(const char *prefix)
        : SerialDebugMIDI_Output(Serial, prefix) {}
};

/**
 * @brief   A class for MIDI interfaces sending and receiving 
 *          human-readable MIDI messages over a Stream.
 * 
 * @ingroup MIDIInterfaces
 */
class StreamDebugMIDI_Interface : public PrintDebugMIDI_Base,
                                  public MIDI_Interface {
  public:
    /**
     * @brief   Construct a debug MIDI interface on the given Stream.
     * 
     * @param   stream
     *          The Stream interface.
     * @param   prefix
     *          An optional string to print before each message.
     */
    StreamDebugMIDI_Interface(Stream &stream, const char *prefix = nullptr)
        : PrintDebugMIDI_Base(prefix), hexstream(stream) {}

    Stream &getStream() const { return hexstream.puller.stream; }

    /// Try reading and parsing a single incoming MIDI message.
    /// @return  Returns the type of the read message, or
    ///          `MIDIReadEvent::NO_MESSAGE` if no MIDI message was available.
    MIDIReadEvent read();

    /// Return the received channel voice message.
    ChannelMessage getChannelMessage() const {
        return parser.getChannelMessage();
    }
    /// Return the received system common message.
    SysCommonMessage getSysCommonMessage() const {
        return parser.getSysCommonMessage();
    }
    /// Return the received real-time message.
    RealTimeMessage getRealTimeMessage() const {
        return parser.getRealTimeMessage();
    }
    /// Return the received system exclusive message.
    SysExMessage getSysExMessage() const { return parser.getSysExMessage(); }

    void update() override;

  protected:
    // MIDI send implementations
    void sendChannelMessageImpl(ChannelMessage) override;
    void sendSysCommonImpl(SysCommonMessage) override;
    void sendSysExImpl(SysExMessage) override;
    void sendRealTimeImpl(RealTimeMessage) override;
    void sendNowImpl() override;

  private:
#if !DISABLE_PIPES
    void handleStall() override { MIDI_Interface::handleStall(this); }
#ifdef DEBUG_OUT
    const char *getName() const override { return "dbg"; }
#endif
#endif

  private:
    HexPuller<StreamPuller> hexstream;
    SerialMIDI_Parser parser;
};

/**
 * @brief   A wrapper class for debug MIDI interfaces sending and receiving 
 *          human-readable MIDI messages over a Serial port of class T.
 * 
 * @note    This is a template class because the class of the Serial object
 *          is completely different on different architectures, and they
 *          do not share a common super-class that has a `begin` method.
 * 
 * @ingroup MIDIInterfaces
 */
template <typename T>
class SerialDebugMIDI_Interface : public StreamDebugMIDI_Interface {
  public:
    /**
     * @brief   Construct a new Debug MIDI Interface on the given Serial 
     *          interface with the given baud rate.
     * 
     * @param   serial
     *          The Serial interface.
     * @param   baud
     *          The baud rate for the Serial interface.
     * @param   prefix
     *          An optional string to print before each message.
     */
    SerialDebugMIDI_Interface(T &serial,
                              unsigned long baud = AH::defaultBaudRate,
                              const char *prefix = nullptr)
        : StreamDebugMIDI_Interface(serial, prefix), baud(baud) {}
    /// @see @ref SerialDebugMIDI_Interface(T &,unsigned long,const char *)
    SerialDebugMIDI_Interface(T &serial, const char *prefix)
        : SerialDebugMIDI_Interface(serial, AH::defaultBaudRate, prefix) {}

    /**
     * @brief   Start the Serial interface at the predefined baud rate.
     */
    void begin() override { static_cast<T &>(getStream()).begin(baud); }

  private:
    const unsigned long baud;
};

/**
 * @brief   A class for debug MIDI interfaces sending and receiving 
 *          human-readable MIDI messages over a HardwareSerial port.
 * 
 * @ingroup MIDIInterfaces
 */
class HardwareSerialDebugMIDI_Interface
    : public SerialDebugMIDI_Interface<HardwareSerial> {
  public:
    /**
     * @brief   Construct a new Debug MIDI Interface on the given HardwareSerial
     *          interface with the given baud rate.
     * 
     * @param   serial
     *          The HardwareSerial interface.
     * @param   baud
     *          The baud rate for the serial interface.
     * @param   prefix
     *          An optional string to print before each message.
     */
    HardwareSerialDebugMIDI_Interface(HardwareSerial &serial,
                                      unsigned long baud = AH::defaultBaudRate,
                                      const char *prefix = nullptr)
        : SerialDebugMIDI_Interface(serial, baud, prefix) {}
    /// @see @ref HardwareSerialDebugMIDI_Interface(HardwareSerial &,unsigned long,const char *)
    HardwareSerialDebugMIDI_Interface(HardwareSerial &serial,
                                      const char *prefix)
        : SerialDebugMIDI_Interface(serial, prefix) {}
};

/**
 * @brief   A class for debug MIDI interfaces sending and receiving 
 *          human-readable MIDI messages over the USB CDC connection.
 * 
 *          Boards without a native USB connection (UNO, MEGA, Nano ...)
 *          use HardwareSerial0 for USB communication.
 * 
 * @ingroup MIDIInterfaces
 */
class USBDebugMIDI_Interface
    : public SerialDebugMIDI_Interface<decltype(Serial)> {
  public:
    /**
     * @brief   Construct a USBDebugMIDI_Interface with the given baud rate.
     * 
     * @param   baud
     *          The baud rate to start the USB Serial connection with.
     * @param   prefix
     *          An optional string to print before each message.
     */
    USBDebugMIDI_Interface(unsigned long baud = AH::defaultBaudRate,
                           const char *prefix = nullptr)
        : SerialDebugMIDI_Interface(Serial, baud, prefix) {}
    /// @see @ref USBDebugMIDI_Interface(unsigned long,const char *)
    USBDebugMIDI_Interface(const char *prefix)
        : SerialDebugMIDI_Interface(Serial, prefix) {}
};

END_CS_NAMESPACE
