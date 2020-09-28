/* ✔ */

#pragma once

#include <Def/MIDIAddress.hpp>
#include <MIDI_Parsers/MIDI_MessageTypes.hpp>

BEGIN_CS_NAMESPACE

/// Struct for easily matching MIDI messages.
struct ChannelMessageMatcher {
    /// Constructor.
    ChannelMessageMatcher(MIDIMessageType type, Channel channel, uint8_t data1,
                          uint8_t data2, Cable cable = CABLE_1)
        : type(type), channel(channel), data1(data1), data2(data2),
          cable(cable) {}

    /// Constructor.
    ChannelMessageMatcher(const ChannelMessage &midimsg)
        : type(midimsg.getMessageType()), channel(midimsg.getChannel()),
          data1(midimsg.data1), data2(midimsg.data2),
          cable(midimsg.getCable()) {}

    MIDIMessageType type; ///< MIDI Message Type
    Channel channel;      ///< MIDI Channel
    uint8_t data1;        ///< MIDI data byte 1
    uint8_t data2;        ///< MIDI data byte 2
    Cable cable;          ///< MIDI USB cable number

    /// Get the MIDI address of this message, using `data1` as the address.
    /// @note   Don't use this for Channel Pressure or Pitch Bend messages,
    ///         as `data1` will have a different meaning in those cases.
    MIDIAddress getAddress() const { return {data1, channel, cable}; }
    /// Get the MIDI channel and cable number.
    /// @note   Valid for all MIDI Channel messages, including Channel Pressure
    ///         and Pitch Bend.
    MIDIChannelCable getChannelCable() const { return {channel, cable}; }
};

inline bool operator==(ChannelMessageMatcher a, ChannelMessageMatcher b) {
    return a.type == b.type && a.channel == b.channel && a.data1 == b.data1 &&
           a.data2 == b.data2 && a.cable == b.cable;
}

END_CS_NAMESPACE