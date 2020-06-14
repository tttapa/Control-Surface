#pragma once

#include <AH/Settings/Warnings.hpp>
#include <Settings/NamespaceSettings.hpp>
#include <stddef.h> // size_t
#include <stdint.h> // uint8_t

#ifndef ARDUINO
#include <vector>
#endif

AH_DIAGNOSTIC_WERROR()

#include <Def/Channel.hpp>

BEGIN_CS_NAMESPACE

struct ChannelMessage {
    uint8_t header; ///< MIDI status byte (message type and channel).
    uint8_t data1;  ///< First MIDI data byte
    uint8_t data2;  ///< First MIDI data byte

    uint8_t CN; ///< USB MIDI cable number;

    /// Check for equality.
    bool operator==(ChannelMessage other) const {
        return this->header == other.header && this->data1 == other.data1 &&
               this->data2 == other.data2 && this->CN == other.CN;
    }
    /// Check for inequality.
    bool operator!=(ChannelMessage other) const { return !(*this == other); }

    /// Get the MIDI channel of the message.
    Channel getChannel() const { return Channel(header & 0x0F); }

    /// Set the MIDI channel of the message.
    void setChannel(Channel channel) {
        header &= 0xF0;
        header |= channel.getRaw();
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

    bool operator==(SysExMessage other) const {
        return this->length == other.length &&
               this->data == other.data && // TODO: compare contents or pointer?
               this->CN == other.CN;
    }
    bool operator!=(SysExMessage other) const { return !(*this == other); }
};

struct RealTimeMessage {
    uint8_t message;
    uint8_t CN;

    bool operator==(RealTimeMessage other) const {
        return this->message == other.message && this->CN == other.CN;
    }
    bool operator!=(RealTimeMessage other) const { return !(*this == other); }
};

END_CS_NAMESPACE

AH_DIAGNOSTIC_POP()