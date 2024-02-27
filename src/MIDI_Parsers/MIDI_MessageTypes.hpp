#pragma once

#include <AH/Arduino-Wrapper.h>           // Print
#include <AH/PrintStream/PrintStream.hpp> // operator<<
#include <AH/STL/cstddef>                 // size_t
#include <AH/STL/vector>
#include <Settings/NamespaceSettings.hpp>

#ifndef ARDUINO
#include <iostream>
#endif

#include <Def/Cable.hpp>
#include <Def/Channel.hpp>
#include <Def/MIDIAddress.hpp>

BEGIN_CS_NAMESPACE

// -------------------------------------------------------------------------- //

/// All possible MIDI status byte values (without channel).
enum class MIDIMessageType : uint8_t {
    None = 0x00, ///< Special value that does not correspond to an actual
                 ///< message type.
    /* Channel Voice Messages */
    NoteOff = 0x80,         ///< Note Off Channel Voice message (3B).
    NoteOn = 0x90,          ///< Note On Channel Voice message (3B).
    KeyPressure = 0xA0,     ///< Key Pressure Channel Voice message (3B).
    ControlChange = 0xB0,   ///< Control Change Channel Voice message (3B).
    ProgramChange = 0xC0,   ///< Program Change Channel Voice message (2B).
    ChannelPressure = 0xD0, ///< Channel Pressure Channel Voice message (2B).
    PitchBend = 0xE0,       ///< Pitch Bend Channel Voice message (3B).

    SysExStart = 0xF0, ///< Start of System Exclusive.

    /* System Common messages */
    MTCQuarterFrame = 0xF1,     ///< MIDI Time Code Quarter Frame System Common
                                ///< message (2B).
    SongPositionPointer = 0xF2, ///< Song Position Pointer System Common
                                ///< message (3B).
    SongSelect = 0xF3,          ///< Song Select System Common message (2B).
    UndefinedSysCommon1 = 0xF4, ///< Undefined System Common message 0xF4 (1B).
    UndefinedSysCommon2 = 0xF5, ///< Undefined System Common message 0xF5 (1B).
    TuneRequest = 0xF6,         ///< Tune Request System Common message (1B).
    SysExEnd = 0xF7,            ///< End of System Exclusive.

    /* System Real-Time messages */
    TimingClock = 0xF8,        ///< Timing Clock System Real-Time message.
    UndefinedRealTime1 = 0xF9, ///< Undefined System Real-Time message 0xF9.
    Start = 0xFA,              ///< Start System Real-Time message.
    Continue = 0xFB,           ///< Continue System Real-Time message.
    Stop = 0xFC,               ///< Stop System Real-Time message.
    UndefinedRealTime2 = 0xFD, ///< Undefined System Real-Time message 0xFD.
    ActiveSensing = 0xFE,      ///< Active Sensing System Real-Time message.
    SystemReset = 0xFF,        ///< Reset System Real-Time message.

// clang-format off
#ifndef DOXYGEN
    NONE CS_DEPREC("Use None instead") = None,
    NOTE_OFF CS_DEPREC("Use NoteOff instead") = NoteOff,
    NOTE_ON CS_DEPREC("Use NoteOn instead") = NoteOn,
    KEY_PRESSURE CS_DEPREC("Use KeyPressure instead") = KeyPressure,
    CC CS_DEPREC("Use ControlChange instead") = ControlChange,
    CONTROL_CHANGE CS_DEPREC("Use ControlChange instead") = ControlChange,
    PROGRAM_CHANGE CS_DEPREC("Use ProgramChange instead") = ProgramChange,
    CHANNEL_PRESSURE CS_DEPREC("Use ChannelPressure instead") = ChannelPressure,
    PITCH_BEND CS_DEPREC("Use PitchBend instead") = PitchBend,
    SYSEX_START CS_DEPREC("Use SysExStart instead") = SysExStart,
    MTC_QUARTER_FRAME CS_DEPREC("Use MTCQuarterFrame instead") = MTCQuarterFrame,
    SONG_POSITION_POINTER CS_DEPREC("Use SongPositionPointer instead") = SongPositionPointer,
    SONG_SELECT CS_DEPREC("Use SongSelect instead") = SongSelect,
    UNDEFINED_SYSCOMMON_1 CS_DEPREC("Use UndefinedSysCommon1 instead") = UndefinedSysCommon1,
    UNDEFINED_SYSCOMMON_2 CS_DEPREC("Use UndefinedSysCommon2 instead") = UndefinedSysCommon2,
    TUNE_REQUEST CS_DEPREC("Use TuneRequest instead") = TuneRequest,
    SYSEX_END CS_DEPREC("Use SysExEnd instead") = SysExEnd,
    TIMING_CLOCK CS_DEPREC("Use TimingClock instead") = TimingClock,
    UNDEFINED_REALTIME_1 CS_DEPREC("Use UndefinedRealTime1 instead") = UndefinedRealTime1,
    START CS_DEPREC("Use Start instead") = Start,
    CONTINUE CS_DEPREC("Use Continue instead") = Continue,
    STOP CS_DEPREC("Use Stop instead") = Stop,
    UNDEFINED_REALTIME_2 CS_DEPREC("Use UndefinedRealTime2 instead") = UndefinedRealTime2,
    ACTIVE_SENSING CS_DEPREC("Use ActiveSensing instead") = ActiveSensing,
    SYSTEM_RESET CS_DEPREC("Use SystemReset instead") = SystemReset,
#endif // DOXYGEN
    // clang-format on
};

/// MIDI USB Code Index Numbers.
///
/// @see    Table 4-1 in <https://usb.org/sites/default/files/midi10.pdf>.
enum class MIDICodeIndexNumber : uint8_t {
    MiscFunctionCodes = 0x0,
    CableEvents = 0x1,
    SystemCommon2B = 0x2,
    SystemCommon3B = 0x3,
    SysExStartCont = 0x4,
    SystemCommon1B = 0x5,
    SysExEnd1B = 0x5,
    SysExEnd2B = 0x6,
    SysExEnd3B = 0x7,

    NoteOff = 0x8,
    NoteOn = 0x9,
    KeyPressure = 0xA,
    ControlChange = 0xB,
    ProgramChange = 0xC,
    ChannelPressure = 0xD,
    PitchBend = 0xE,

    SingleByte = 0xF,
};

// -------------------------------------------------------------------------- //

struct MIDIMessage {
    /// Constructor.
    MIDIMessage(uint8_t header, uint8_t data1, uint8_t data2,
                Cable cable = Cable_1)
        : header(header), data1(data1), data2(data2), cable(cable) {}

    /// Constructor.
    MIDIMessage(MIDIMessageType header, uint8_t data1, uint8_t data2,
                Cable cable = Cable_1)
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
        return header >= (uint8_t(MIDIMessageType::NoteOff) | 0x00) &&
               header <= (uint8_t(MIDIMessageType::PitchBend) | 0x0F);
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
                   uint8_t data2 = 0x00, Cable cable = Cable_1)
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
        return type <= MIDIMessageType::ControlChange ||
               type == MIDIMessageType::PitchBend;
    }

    constexpr static auto NoteOff = MIDIMessageType::NoteOff;
    constexpr static auto NoteOn = MIDIMessageType::NoteOn;
    constexpr static auto KeyPressure = MIDIMessageType::KeyPressure;
    constexpr static auto ControlChange = MIDIMessageType::ControlChange;
    constexpr static auto ProgramChange = MIDIMessageType::ProgramChange;
    constexpr static auto ChannelPressure = MIDIMessageType::ChannelPressure;
    constexpr static auto PitchBend = MIDIMessageType::PitchBend;
};

struct SysCommonMessage : MIDIMessage {
    using MIDIMessage::MIDIMessage;

    /// Constructor.
    SysCommonMessage(MIDIMessageType type, uint8_t data1 = 0x00,
                     uint8_t data2 = 0x00, Cable cable = Cable_1)
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
        if (getMessageType() == MIDIMessageType::SongPositionPointer)
            return 2;
        else if (getMessageType() <= MIDIMessageType::SongSelect)
            return 1;
        else
            return 0;
    }

    constexpr static auto MTCQuarterFrame = MIDIMessageType::MTCQuarterFrame;
    constexpr static auto SongPositionPointer =
        MIDIMessageType::SongPositionPointer;
    constexpr static auto SongSelect = MIDIMessageType::SongSelect;
    constexpr static auto UndefinedSysCommon1 =
        MIDIMessageType::UndefinedSysCommon1;
    constexpr static auto UndefinedSysCommon2 =
        MIDIMessageType::UndefinedSysCommon2;
    constexpr static auto TuneRequest = MIDIMessageType::TuneRequest;
};

struct SysExMessage {
    /// Constructor.
    SysExMessage() : data(nullptr), length(0), cable(Cable_1) {}

    /// Constructor.
    SysExMessage(const uint8_t *data, uint16_t length, Cable cable = Cable_1)
        : data(data), length(length), cable(cable.getRaw()) {}

    /// Constructor.
    SysExMessage(const std::vector<uint8_t> &vec, Cable cable = Cable_1)
        : SysExMessage(vec.data(), vec.size(), cable) {}

    /// Constructor.
    template <uint16_t N>
    SysExMessage(const uint8_t (&array)[N], Cable cable = Cable_1)
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
        return length >= 1 && data[0] == uint8_t(MIDIMessageType::SysExStart);
    }

    bool isLastChunk() const {
        return length >= 1 &&
               data[length - 1] == uint8_t(MIDIMessageType::SysExEnd);
    }

    bool isCompleteMessage() const { return isFirstChunk() && isLastChunk(); }

    constexpr static auto SysExStart = MIDIMessageType::SysExStart;
    constexpr static auto SysExEnd = MIDIMessageType::SysExEnd;
};

struct RealTimeMessage {
    /// Constructor.
    RealTimeMessage(uint8_t message, Cable cable = Cable_1)
        : message(message), cable(cable.getRaw()) {}

    /// Constructor.
    RealTimeMessage(MIDIMessageType message, Cable cable = Cable_1)
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

    constexpr static auto TimingClock = MIDIMessageType::TimingClock;
    constexpr static auto UndefinedRealTime1 =
        MIDIMessageType::UndefinedRealTime1;
    constexpr static auto Start = MIDIMessageType::Start;
    constexpr static auto Continue = MIDIMessageType::Continue;
    constexpr static auto Stop = MIDIMessageType::Stop;
    constexpr static auto UndefinedRealTime2 =
        MIDIMessageType::UndefinedRealTime2;
    constexpr static auto ActiveSensing = MIDIMessageType::ActiveSensing;
    constexpr static auto RESET = MIDIMessageType::SystemReset;
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
