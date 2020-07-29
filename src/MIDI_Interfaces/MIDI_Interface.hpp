#pragma once

#include "MIDI_Pipes.hpp"
#include <AH/Containers/Updatable.hpp>
#include <Def/Def.hpp>
#include <Def/MIDIAddress.hpp>
#include <MIDI_Parsers/MIDI_Parser.hpp>

BEGIN_CS_NAMESPACE

constexpr auto MIDI_BAUD = 31250;

class MIDI_Callbacks;

/**
 * @brief   Statically polymorphic template for classes that send MIDI messages.
 */
template <class Derived>
class MIDI_Sender {
  public:
    /// @name Sending MIDI
    /// @{

    /**
     * @brief   Send a 3-byte MIDI packet.
     *
     * @param   m
     *          MIDI message type. [0x80, 0xE0]
     * @param   c
     *          The MIDI channel. [1, 16]
     * @param   d1
     *          The first data byte. [0, 127]
     * @param   d2
     *          The second data byte. [0, 127]
     */
    void send(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2);

    /**
     * @brief   Send a 2-byte MIDI packet.
     *
     * @param   m
     *          MIDI message type. [0x80, 0xE0]
     * @param   c
     *          The MIDI channel. [1, 16]
     * @param   d1
     *          The first data byte. [0, 127]
     */
    void send(uint8_t m, uint8_t c, uint8_t d1);

    /**
     * @brief   Send a 3-byte MIDI packet with cable number.
     *
     * @param   m
     *          MIDI message type. [0x80, 0xE0]
     * @param   c
     *          The MIDI channel. [1, 16]
     * @param   d1
     *          The first data byte. [0, 127]
     * @param   d2
     *          The second data byte. [0, 127]
     * @param   cn
     *          The MIDI Cable Number. [0, 15]
     */
    void sendOnCable(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2, uint8_t cn);

    /**
     * @brief   Send a 2-byte MIDI packet with cable number.
     *
     * @param   m
     *          MIDI message type. [0x80, 0xE0]
     * @param   c
     *          The MIDI channel. [1, 16]
     * @param   d1
     *          The first data byte. [0, 127]
     * @param   cn
     *          The MIDI Cable Number. [0, 15]
     */
    void sendOnCable(uint8_t m, uint8_t c, uint8_t d1, uint8_t cn);

    /**
     * @brief   Send a single-byte MIDI packet with cable number.
     *
     * @param   r
     *          The MIDI byte to send.
     * @param   cn
     *          The MIDI Cable Number. [0, 15]
     */
    void sendOnCable(uint8_t r, uint8_t cn);

    /// Send a MIDI Note On event.
    void sendNoteOn(MIDIAddress address, uint8_t velocity);
    /// Send a MIDI Note Off event.
    void sendNoteOff(MIDIAddress address, uint8_t velocity);
    /// Send a MIDI Key Pressure event.
    void sendKP(MIDIAddress address, uint8_t pressure);
    /// Send a MIDI Control Change event.
    void sendCC(MIDIAddress address, uint8_t value);
    /// Send a MIDI Program Change event.
    void sendPC(MIDIAddress address);
    /// Send a MIDI Program Change event.
    void sendPC(MIDIChannelCN address, uint8_t value);
    /// Send a MIDI Channel Pressure event.
    void sendCP(MIDIChannelCN address, uint8_t pressure);
    /// Send a MIDI Pitch Bend event.
    void sendPB(MIDIChannelCN address, uint16_t value);
    /// Send a MIDI Channel Message
    void send(ChannelMessage message);
    /// Send a MIDI System Exclusive message.
    void send(SysExMessage message);
    /// Send a MIDI System Exclusive message.
    template <size_t N>
    void send(const uint8_t (&sysexdata)[N], uint8_t cn = 0) {
        send(SysExMessage{sysexdata, N, cn});
    }
    /// Send a MIDI Real-Time message.
    void send(RealTimeMessage message);
    /// Send a single-byte MIDI message.
    void send(uint8_t rt, uint8_t cn = 0);

    /// @}
};

/**
 * @brief   An abstract class for MIDI interfaces.
 */
class MIDI_Interface : public TrueMIDI_SinkSource,
                       public MIDI_Sender<MIDI_Interface>,
                       public AH::Updatable<MIDI_Interface> {
  protected:
    /**
     * @brief   Constructor.
     */
    MIDI_Interface() = default;

    MIDI_Interface(MIDI_Interface &&) = default;

  public:
    /**
     * @brief   Destructor.
     */
    virtual ~MIDI_Interface();

    /**
     * @brief   Initialize the MIDI Interface.
     */
    void begin() override {}

    /**
     * @brief   Read the MIDI interface and call the callback if a message is
     *          received.
     */
    void update() override = 0;

    /// @name   Default MIDI Interfaces
    /// @{
    /**
     * @brief   Return the default MIDI interface.
     */
    static MIDI_Interface *getDefault();

    /**
     * @brief   Set this MIDI interface as the default interface.
     */
    void setAsDefault();
    /// @}

    /// @name   MIDI Input Callbacks
    /// @{
    /**
     * @brief   Set the callbacks that will be called when a MIDI message is 
     *          received.
     * 
     * @param   cb
     *          A pointer to an object that implements the MIDI_Callbacks class.
     */
    virtual void setCallbacks(MIDI_Callbacks *cb) = 0;

    /**
     * @brief   Set the callbacks that will be called when a MIDI message is 
     *          received.
     * 
     * @param   cb
     *          A reference to an object that implements the MIDI_Callbacks 
     *          class.
     */
    void setCallbacks(MIDI_Callbacks &cb) { setCallbacks(&cb); }
    /// @}

  protected:
    friend class MIDI_Sender<MIDI_Interface>;
    /**
     * @brief   Low-level function for sending a 3-byte MIDI message.
     */
    virtual void sendImpl(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2,
                          uint8_t cn) = 0;
    /**
     * @brief   Low-level function for sending a 2-byte MIDI message.
     */
    virtual void sendImpl(uint8_t m, uint8_t c, uint8_t d1, uint8_t cn) = 0;

    /**
     * @brief   Low-level function for sending a system exclusive MIDI message.
     */
    virtual void sendImpl(const uint8_t *data, size_t length, uint8_t cn) = 0;

    /** 
     * @brief   Low-level function for sending a single-byte MIDI message.
     */
    virtual void sendImpl(uint8_t rt, uint8_t cn) = 0;

  protected:
    /// Accept an incoming MIDI Channel message.
    void sinkMIDIfromPipe(ChannelMessage) override;
    /// Accept an incoming MIDI System Exclusive message.
    void sinkMIDIfromPipe(SysExMessage) override;
    /// Accept an incoming MIDI Real-Time message.
    void sinkMIDIfromPipe(RealTimeMessage) override;

  private:
    static MIDI_Interface *DefaultMIDI_Interface;
};

/**
 * @brief   An abstract class for MIDI interfaces.
 */
class Parsing_MIDI_Interface : public MIDI_Interface {
  protected:
    /**
     * @brief   Construct a MIDI interface with the given parser.
     *
     *          Also set this interface as the default MIDI interface.
     *
     * @param   parser
     *          The MIDI parser to use for the interface.
     */
    Parsing_MIDI_Interface(MIDI_Parser &parser);

  public:
    /// @name   Parsing MIDI Input
    /// @{
    MIDI_Parser &getParser() { return parser; }

    /**
     * @brief   Return the received channel message.
     */
    ChannelMessage getChannelMessage();

    /**
     * @brief   Return the received system exclusive message.
     */
    SysExMessage getSysExMessage() const;

    /**
     * @brief   Return the cable number of the received message.
     */
    uint8_t getCN() const;
    /// @}

    void update() override;

    void setCallbacks(MIDI_Callbacks *cb) override { this->callbacks = cb; }
    using MIDI_Interface::setCallbacks;

  protected:
    bool dispatchMIDIEvent(MIDI_read_t event);

  private:
    /**
     * @brief   Try reading and parsing a single incoming MIDI message.
     * @return  Returns the type of the read message, or @ref NO_MESSAGE if no
     *          MIDI message was available.
     */
    virtual MIDI_read_t read() = 0;

    bool onRealtimeMessage(uint8_t message);
    bool onChannelMessage();
    bool onSysExMessage();

  protected:
    MIDI_Parser &parser;
    MIDI_Callbacks *callbacks = nullptr;
    MIDI_read_t event = NO_MESSAGE;
};

// LCOV_EXCL_START
/**
 * @brief   A class for callbacks from MIDI input.
 */
class MIDI_Callbacks {
  public:
    /// Callback for incoming MIDI Channel Messages (notes, control change, 
    /// pitch bend, etc.)
    virtual void onChannelMessage(Parsing_MIDI_Interface &midi) { (void)midi; }
    /// Callback for incoming MIDI System Exclusive Messages.
    virtual void onSysExMessage(Parsing_MIDI_Interface &midi) { (void)midi; }
    /// Callback for incoming MIDI Real-Time Messages.
    virtual void onRealtimeMessage(Parsing_MIDI_Interface &midi,
                                   uint8_t message) {
        (void)midi;
        (void)message;
    }

    /// Destructor.
    virtual ~MIDI_Callbacks() = default;
};
// LCOV_EXCL_STOP

END_CS_NAMESPACE

#include "MIDI_Interface.ipp"