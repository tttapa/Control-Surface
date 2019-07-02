#pragma once

#include <Def/Def.hpp>
#include <Def/MIDICNChannelAddress.hpp>
#include <MIDI_Parsers/MIDI_Parser.hpp>

#define MIDI_BAUD 31250

class MIDI_Interface;

/**
 * @brief   A class for callbacks from MIDI input.
 */
class MIDI_Callbacks {
    friend class MIDI_Interface;
    virtual void onChannelMessage(UNUSED_PARAM MIDI_Interface &midi) {}
    virtual void onSysExMessage(UNUSED_PARAM MIDI_Interface &midi) {}
    virtual void onRealtimeMessage(UNUSED_PARAM MIDI_Interface &midi,
                                   UNUSED_PARAM uint8_t message) {}

  public:
    virtual ~MIDI_Callbacks() = default;
};

/**
 * @brief   An abstract class for MIDI interfaces.
 */
class MIDI_Interface {
  protected:
    /**
     * @brief   Construct a MIDI interface with the given parser.
     *
     *          Also set this interface as the default MIDI interface.
     *
     * @param   parser
     *          The MIDI parser to use for the interface.
     */
    MIDI_Interface(MIDI_Parser &parser);

  public:
    void setCallbacks(MIDI_Callbacks *cb) { this->callbacks = cb; }

    /**
     * @brief   Destructor.
     */
    virtual ~MIDI_Interface();

    /**
     * @brief   Initialize the MIDI Interface.
     */
    virtual void begin() {}

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
    void sendCN(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2, uint8_t cn);

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
    void sendCN(uint8_t m, uint8_t c, uint8_t d1, uint8_t cn);

    void sendNoteOn(MIDICNChannelAddress address, uint8_t velocity);
    void sendNoteOff(MIDICNChannelAddress address, uint8_t velocity);
    void sendCC(MIDICNChannelAddress address, uint8_t value);
    void sendPB(MIDICNChannelAddress address, uint16_t value);
    void sendPB(MIDICNChannel address, uint16_t value);
    void sendPC(MIDICNChannel address, uint8_t value);

    void update();

    /**
     * @todo    Documentation
     */
    virtual MIDI_read_t read() = 0;

    /**
     * @brief   Return the default MIDI interface.
     */
    static MIDI_Interface *getDefault();

    /**
     * @brief   Set this MIDI interface as the default interface.
     */
    void setAsDefault();

  private:
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

  protected:
    void onChannelMessage();
    void onSysExMessage();
    void onRealtimeMessage(uint8_t message);

    bool dispatchMIDIEvent(MIDI_read_t event);

  private:
    static MIDI_Interface *DefaultMIDI_Interface;

    MIDI_Parser &parser;
    MIDI_Callbacks *callbacks = nullptr;

  public:
    /**
     * @brief   Return the received channel message.
     */
    MIDI_message getChannelMessage();

    /**
     * @brief   Return the received system exclusive message.
     */
    SysExMessage getSysExMessage() const;

    /**
     * @brief   Return the cable number of the received message.
     */
    uint8_t getCN() const;
};