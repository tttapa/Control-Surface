#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <Def/Def.hpp>
#include <Settings/SettingsWrapper.hpp>

#ifndef ARDUINO
#include <vector>
#endif

BEGIN_CS_NAMESPACE

const uint8_t NOTE_OFF = 0x80;
const uint8_t NOTE_ON = 0x90;
const uint8_t KEY_PRESSURE = 0xA0;
const uint8_t CC = 0xB0, CONTROL_CHANGE = CC;
const uint8_t PROGRAM_CHANGE = 0xC0;
const uint8_t CHANNEL_PRESSURE = 0xD0;
const uint8_t PITCH_BEND = 0xE0;

const uint8_t SysExStart = 0xF0;
const uint8_t SysExEnd = 0xF7;

const uint8_t TuneRequest = 0xF6;

enum MIDI_read_t : uint8_t {
    NO_MESSAGE = 0,
    CHANNEL_MESSAGE = 1,
    SYSEX_MESSAGE = 2,

    /* System Real-Time messages */
    TIMING_CLOCK_MESSAGE = 0xF8,
    UNDEFINED_REALTIME_MESSAGE_1 = 0xF9,
    START_MESSAGE = 0xFA,
    CONTINUE_MESSAGE = 0xFB,
    STOP_MESSAGE = 0xFC,
    UNDEFINED_REALTIME_MESSAGE_2 = 0xFD,
    ACTIVE_SENSING_MESSAGE = 0xFE,
    RESET_MESSAGE = 0xFF
};

// -------------------------------------------------------------------------- //

struct ChannelMessage {
    uint8_t header;
    uint8_t data1;
    uint8_t data2;

    uint8_t CN = 0;

    bool operator==(const ChannelMessage &other) const {
        return this->header == other.header && this->data1 == other.data1 &&
               this->data2 == other.data2 && this->CN == other.CN;
    }

    bool operator!=(const ChannelMessage &other) const {
        return !(*this == other);
    }
};

struct SysExMessage {
    SysExMessage() : data(nullptr), length(0), CN(0) {}
    SysExMessage(const uint8_t *data, size_t length, uint8_t CN = 0)
        : data(data), length(length), CN(CN) {}
#ifndef ARDUINO
    SysExMessage(const std::vector<uint8_t> &vec, uint8_t CN = 0)
        : data(vec.data()), length(vec.size()), CN(CN) {}
#endif
    const uint8_t *data;
    uint8_t length;
    uint8_t CN;
};

struct RealTimeMessage {
    uint8_t message;
    uint8_t CN;
};

// -------------------------------------------------------------------------- //

class MIDI_Parser {
  public:
    /** Get the latest MIDI channel message */
    ChannelMessage getChannelMessage();
#if !IGNORE_SYSEX
    /** Get the latest SysEx message. */
    virtual SysExMessage getSysEx() const = 0;
#else
    SysExMessage getSysEx() const { return {nullptr, 0, 0}; }
#endif
    /** Get the pointer to the SysEx data. */
    const uint8_t *getSysExBuffer() const { return getSysEx().data; }
    /** Get the length of the SysEx message. */
    size_t getSysExLength() const { return getSysEx().length; }
    /** Get the cable number of the latests MIDI message */
    virtual uint8_t getCN() const { return 0; };

  protected:
    ChannelMessage midimsg = {};

  public:
    /** Check if the given byte is a MIDI header byte. */
    static bool isStatus(uint8_t data);
    /** Check if the given byte is a MIDI data byte. */
    static bool isData(uint8_t data);
};

END_CS_NAMESPACE