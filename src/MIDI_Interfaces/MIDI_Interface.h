#pragma once

#include <MIDI_Parsers/MIDI_Parser.h>

#define MIDI_BAUD 31250

/**
 * @brief   An abstract class for MIDI interfaces.
 */
class MIDI_Interface {
  public:
    /**
     * @brief   Construct a MIDI interface with the given parser.
     *
     *          Also set this interface as the default MIDI interface.
     *
     * @param   parser
     *          The MIDI parser to use for the interface.
     */
    MIDI_Interface(MIDI_Parser &parser);

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
     * @brief   Try to read a MIDI message and return what type of message
     *          has been read.
     *
     * @return  NO_MESSAGE
     *          No MIDI message is ready to be read.
     * @return  CHANNEL_MESSAGE
     *          A MIDI channel message has been received
     *          and is ready to be read using `getChannelMessage`.
     * @return  SYSEX_MESSAGE
     *          A MIDI system exclusive message has been received
     *          and is ready to be read using `getSysExBuffer` and
     *          `getSysExLength`.
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

  protected:
    /**
     * @brief   Low-level function for sending a 3-byte MIDI message.
     */
    virtual void sendImpl(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2) = 0;
    /**
     * @brief   Low-level function for sending a 2-byte MIDI message.
     */
    virtual void sendImpl(uint8_t m, uint8_t c, uint8_t d1) = 0;

    MIDI_Parser &parser;

    static MIDI_Interface *DefaultMIDI_Interface;

  public:
    /**
     * @brief   Return the received channel message.
     */
    MIDI_message getChannelMessage();

    /**
     * @brief   Return the length of the received system exclusive message.
     */
    size_t getSysExLength();

    /**
     * @brief   Return a pointer to the received system exclusive message.
     */
    const uint8_t *getSysExBuffer();
};