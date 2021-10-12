#pragma once

#include <AH/Arduino-Wrapper.h> // Print
#include <AH/STL/cstddef>       // size_t
#include <AH/STL/vector>
#include <AH/Settings/Warnings.hpp>
#include <Settings/NamespaceSettings.hpp>

#ifndef ARDUINO
#include <iostream>
#endif

AH_DIAGNOSTIC_WERROR()

#include <Def/Cable.hpp>
#include <Def/Channel.hpp>
#include <Def/MIDIAddress.hpp>

BEGIN_CS_NAMESPACE

// -------------------------------------------------------------------------- //

/// All possible MIDI status byte values (without channel).
enum class MIDIMessageType : uint8_t {
    NONE = 0x00,
    /* Channel Voice Messages */
    NOTE_OFF = 0x80,         // 3B
    NOTE_ON = 0x90,          // 3B
    KEY_PRESSURE = 0xA0,     // 3B
    CC = 0xB0,               // 3B
    CONTROL_CHANGE = CC,     // 3B
    PROGRAM_CHANGE = 0xC0,   // 2B
    CHANNEL_PRESSURE = 0xD0, // 2B
    PITCH_BEND = 0xE0,       // 3B

    SYSEX_START = 0xF0,

    /* System Common messages */
    MTC_QUARTER_FRAME = 0xF1,
    SONG_POSITION_POINTER = 0xF2,
    SONG_SELECT = 0xF3,
    UNDEFINED_SYSCOMMON_1 = 0xF4,
    UNDEFINED_SYSCOMMON_2 = 0xF5,
    TUNE_REQUEST = 0xF6,
    SYSEX_END = 0xF7,

    /* System Real-Time messages */
    TIMING_CLOCK = 0xF8,
    UNDEFINED_REALTIME_1 = 0xF9,
    START = 0xFA,
    CONTINUE = 0xFB,
    STOP = 0xFC,
    UNDEFINED_REALTIME_2 = 0xFD,
    ACTIVE_SENSING = 0xFE,
    SYSTEM_RESET = 0xFF,
};

/// MIDI USB Code Index Numbers.
///
/// @see    Table 4-1 in <https://usb.org/sites/default/files/midi10.pdf>.
enum class MIDICodeIndexNumber : uint8_t {
    MISC_FUNCTION_CODES = 0x0,
    CABLE_EVENTS = 0x1,
    SYSTEM_COMMON_2B = 0x2,
    SYSTEM_COMMON_3B = 0x3,
    SYSEX_START_CONT = 0x4,
    SYSTEM_COMMON_1B = 0x5,
    SYSEX_END_1B = 0x5,
    SYSEX_END_2B = 0x6,
    SYSEX_END_3B = 0x7,

    NOTE_OFF = 0x8,
    NOTE_ON = 0x9,
    KEY_PRESSURE = 0xA,
    CONTROL_CHANGE = 0xB,
    PROGRAM_CHANGE = 0xC,
    CHANNEL_PRESSURE = 0xD,
    PITCH_BEND = 0xE,

    SINGLE_BYTE = 0xF,
};

// -------------------------------------------------------------------------- //

struct MIDIMessage {
    /// Constructor.
    MIDIMessage(uint8_t header, uint8_t data1, uint8_t data2,
                Cable cable = CABLE_1)
        : header(header), data1(data1), data2(data2), cable(cable) {}

    /// Constructor.
    MIDIMessage(MIDIMessageType header, uint8_t data1, uint8_t data2,
                Cable cable = CABLE_1)
        : header(uint8_t(header)), data1(data1), data2(data2), cable(cable) {}

    uint8_t header; ///< MIDI status byte (message type and channel).
    uint8_t data1;  ///< First MIDI data byte
    uint8_t data2;  ///< First MIDI data byte

    Cable cable; ///< USB MIDI cable number;

    /// Check for equality.
    bool operator==(MIDIMessage other) const {
        return this->header == other.header && this->data1 == other.data1 &&
               this->data2 == other.data2 && this->cable == other.cable;
    }
    /// Check for inequality.
    bool operator!=(MIDIMessage other) const { return !(*this == other); }

    /// Get the MIDI message type.
    MIDIMessageType getMessageType() const {
        if (hasValidChannelMessageHeader()) {
            return static_cast<MIDIMessageType>(header & 0xF0);
        } else {
            return static_cast<MIDIMessageType>(header);
        }
    }
    /// Set the MIDI message type.
    /// @note   Do not use this version for Channel Messages, it doesn't
    ///         correctly handle the channel.
    void setMessageType(MIDIMessageType type) {
        header = static_cast<uint8_t>(type);
    }

    /// Get the first data byte.
    uint8_t getData1() const { return data1; }
    /// Get the second data byte.
    uint8_t getData2() const { return data2; }
    /// Set the first data byte.
    void setData1(uint8_t data) { data1 = data; }
    /// Set the second data byte.
    void setData2(uint8_t data) { data2 = data; }

    /// Get the MIDI USB cable number of the message.
    Cable getCable() const { return cable; }
    /// Set the MIDI USB cable number of the message.
    void setCable(Cable cable) { this->cable = cable; }

    /// Check whether the header is a valid header for a channel message.
    bool hasValidChannelMessageHeader() const {
        return header >= (uint8_t(MIDIMessageType::NOTE_OFF) | 0x00) &&
               header <= (uint8_t(MIDIMessageType::PITCH_BEND) | 0x0F);
    }

    /// Check whether the header is a valid header for a System Common message.
    /// @note   SysExEnd is considered a System Common message by the MIDI
    ///         Standard, SysExStart is not.
    /// @note   Reserved System Common messages are also considered valid System
    ///         Common messages.
    bool hasValidSystemCommonHeader() const {
        return (header & 0xF8) == 0xF0 && header != 0xF0;
    }

    /// If Data 1 and Data 2 represent a single 14-bit number, you can use this
    /// method to retrieve that number.
    uint16_t getData14bit() const {
        return data1 | (uint16_t(data2) << uint16_t(7));
    }
    /// If Data 1 and Data 2 represent a single 14-bit number, you can use this
    /// method to set that number.
    void setData14bit(uint16_t data) {
        data1 = (data >> 0) & 0x7F;
        data2 = (data >> 7) & 0x7F;
    }

    /// Make sure that the status byte has the most significant bit set and
    /// the data bytes have the most significant bits cleared.
    void sanitize() {
        header |= 0x80;
        data1 &= 0x7F;
        data2 &= 0x7F;
    }
};

struct ChannelMessage : MIDIMessage {
    using MIDIMessage::MIDIMessage;

    /// Constructor.
    ChannelMessage(MIDIMessageType type, Channel channel, uint8_t data1,
                   uint8_t data2 = 0x00, Cable cable = CABLE_1)
        : MIDIMessage(uint8_t(type) | channel.getRaw(), data1, data2, cable) {}

    explicit ChannelMessage(const MIDIMessage &msg) : MIDIMessage(msg) {}

    /// Get the MIDI message type.
    MIDIMessageType getMessageType() const {
        return static_cast<MIDIMessageType>(header & 0xF0);
    }
    /// Set the MIDI message type.
    void setMessageType(MIDIMessageType type) {
        header &= 0x0F;
        header |= static_cast<uint8_t>(type) & 0xF0;
    }

    /// Get the MIDI channel of the message.
    Channel getChannel() const { return Channel(header & 0x0F); }
    /// Set the MIDI channel of the message.
    void setChannel(Channel channel) {
        header &= 0xF0;
        header |= channel.getRaw();
    }

    /// Get the MIDI channel and cable number.
    /// @note   Valid for all MIDI Channel messages, including Channel Pressure
    ///         and Pitch Bend.
    MIDIChannelCable getChannelCable() const { return {getChannel(), cable}; }
    /// Get the MIDI address of this message, using `data1` as the address.
    /// @note   Don't use this for Channel Pressure or Pitch Bend messages,
    ///         as `data1` will have a different meaning in those cases.
    MIDIAddress getAddress() const { return {data1, getChannelCable()}; }

    /// Check whether this message has one or two data bytes.
    ///
    /// - 2 data bytes: Note On/Off, Aftertouch, Control Change or Pitch Bend
    /// - 1 data byte: Program Change or Channel Pressure
    ///
    /// Returns false if the header is a SysEx, Real-Time or System Common byte.
    bool hasTwoDataBytes() const {
        auto type = getMessageType();
        return type <= MIDIMessageType::CONTROL_CHANGE ||
               type == MIDIMessageType::PITCH_BEND;
    }

    constexpr static auto NOTE_OFF = MIDIMessageType::NOTE_OFF;
    constexpr static auto NOTE_ON = MIDIMessageType::NOTE_ON;
    constexpr static auto KEY_PRESSURE = MIDIMessageType::KEY_PRESSURE;
    constexpr static auto CC = MIDIMessageType::CC;
    constexpr static auto CONTROL_CHANGE = MIDIMessageType::CONTROL_CHANGE;
    constexpr static auto PROGRAM_CHANGE = MIDIMessageType::PROGRAM_CHANGE;
    constexpr static auto CHANNEL_PRESSURE = MIDIMessageType::CHANNEL_PRESSURE;
    constexpr static auto PITCH_BEND = MIDIMessageType::PITCH_BEND;
};

struct SysCommonMessage : MIDIMessage {
    using MIDIMessage::MIDIMessage;

    /// Constructor.
    SysCommonMessage(MIDIMessageType type, uint8_t data1 = 0x00,
                     uint8_t data2 = 0x00, Cable cable = CABLE_1)
        : MIDIMessage(type, data1, data2, cable) {}
    /// Constructor.
    SysCommonMessage(MIDIMessageType type, uint8_t data1, Cable cable)
        : SysCommonMessage(type, data1, 0x00, cable) {}
    /// Constructor.
    SysCommonMessage(MIDIMessageType type, Cable cable)
        : SysCommonMessage(type, 0x00, 0x00, cable) {}

    explicit SysCommonMessage(const MIDIMessage &msg) : MIDIMessage(msg) {}

    /// Get the MIDI message type.
    MIDIMessageType getMessageType() const {
        return static_cast<MIDIMessageType>(header);
    }

    /// Get the number of data bytes of this type of System Common message.
    uint8_t getNumberOfDataBytes() const {
        if (getMessageType() == MIDIMessageType::SONG_POSITION_POINTER)
            return 2;
        else if (getMessageType() <= MIDIMessageType::SONG_SELECT)
            return 1;
        else
            return 0;
    }

    constexpr static auto MTC_QUARTER_FRAME =
        MIDIMessageType::MTC_QUARTER_FRAME;
    constexpr static auto SONG_POSITION_POINTER =
        MIDIMessageType::SONG_POSITION_POINTER;
    constexpr static auto SONG_SELECT = MIDIMessageType::SONG_SELECT;
    constexpr static auto UNDEFINED_SYSCOMMON_1 =
        MIDIMessageType::UNDEFINED_SYSCOMMON_1;
    constexpr static auto UNDEFINED_SYSCOMMON_2 =
        MIDIMessageType::UNDEFINED_SYSCOMMON_2;
    constexpr static auto TUNE_REQUEST = MIDIMessageType::TUNE_REQUEST;
};

struct SysExMessage {
    /// Constructor.
    SysExMessage() : data(nullptr), length(0), cable(CABLE_1) {}

    /// Constructor.
    SysExMessage(const uint8_t *data, uint16_t length, Cable cable = CABLE_1)
        : data(data), length(length), cable(cable.getRaw()) {}

    /// Constructor.
    SysExMessage(const std::vector<uint8_t> &vec, Cable cable = CABLE_1)
        : SysExMessage(vec.data(), vec.size(), cable) {}

    /// Constructor.
    template <uint16_t N>
    SysExMessage(const uint8_t (&array)[N], Cable cable = CABLE_1)
        : SysExMessage(array, N, cable) {}

    const uint8_t *data;
    uint16_t length;

    Cable cable;

    bool operator==(SysExMessage other) const {
        return this->length == other.length && this->cable == other.cable &&
               (this->length == 0 ||
                memcmp(this->data, other.data, length) == 0);
    }
    bool operator!=(SysExMessage other) const { return !(*this == other); }

    /// Get the MIDI USB cable number of the message.
    Cable getCable() const { return cable; }
    /// Set the MIDI USB cable number of the message.
    void setCable(Cable cable) { this->cable = cable; }

    bool isFirstChunk() const {
        return length >= 1 && data[0] == uint8_t(MIDIMessageType::SYSEX_START);
    }

    bool isLastChunk() const {
        return length >= 1 &&
               data[length - 1] == uint8_t(MIDIMessageType::SYSEX_END);
    }

    bool isCompleteMessage() const { return isFirstChunk() && isLastChunk(); }

    constexpr static auto SYSEX_START = MIDIMessageType::SYSEX_START;
    constexpr static auto SYSEX_END = MIDIMessageType::SYSEX_END;
};

struct RealTimeMessage {
    /// Constructor.
    RealTimeMessage(uint8_t message, Cable cable = CABLE_1)
        : message(message), cable(cable.getRaw()) {}

    /// Constructor.
    RealTimeMessage(MIDIMessageType message, Cable cable = CABLE_1)
        : message(uint8_t(message)), cable(cable.getRaw()) {}

    uint8_t message;
    Cable cable;

    bool operator==(RealTimeMessage other) const {
        return this->message == other.message && this->cable == other.cable;
    }
    bool operator!=(RealTimeMessage other) const { return !(*this == other); }

    /// Set the MIDI message type.
    void setMessageType(MIDIMessageType type) {
        message = static_cast<uint8_t>(type);
    }
    /// Get the MIDI message type.
    MIDIMessageType getMessageType() const {
        return static_cast<MIDIMessageType>(message);
    }

    /// Get the MIDI USB cable number of the message.
    Cable getCable() const { return cable; }
    /// Set the MIDI USB cable number of the message.
    void setCable(Cable cable) { this->cable = cable; }

    /// Check whether the header is a valid header for a Real-Time message.
    bool isValid() const { return message >= 0xF8; }

    constexpr static auto TIMING_CLOCK = MIDIMessageType::TIMING_CLOCK;
    constexpr static auto UNDEFINED_REALTIME_1 =
        MIDIMessageType::UNDEFINED_REALTIME_1;
    constexpr static auto START = MIDIMessageType::START;
    constexpr static auto CONTINUE = MIDIMessageType::CONTINUE;
    constexpr static auto STOP = MIDIMessageType::STOP;
    constexpr static auto UNDEFINED_REALTIME_2 =
        MIDIMessageType::UNDEFINED_REALTIME_2;
    constexpr static auto ACTIVE_SENSING = MIDIMessageType::ACTIVE_SENSING;
    constexpr static auto RESET = MIDIMessageType::SYSTEM_RESET;
};

#ifndef ARDUINO
inline std::ostream &operator<<(std::ostream &os, SysExMessage m) {
    os << "SysExMessage [" << m.length << "] " << AH::HexDump(m.data, m.length)
       << " (cable " << m.cable.getOneBased() << ")";
    return os;
}
#endif

inline Print &operator<<(Print &os, SysExMessage m) {
    os << "SysExMessage [" << m.length << "] " << AH::HexDump(m.data, m.length)
       << " (cable " << m.cable.getOneBased() << ")";
    return os;
}

FlashString_t enum_to_string(MIDIMessageType);
inline Print &operator<<(Print &os, MIDIMessageType m) {
    return os << enum_to_string(m);
}

END_CS_NAMESPACE

AH_DIAGNOSTIC_POP()