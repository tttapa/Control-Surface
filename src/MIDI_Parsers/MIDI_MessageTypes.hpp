#pragma once

#include <AH/Settings/Warnings.hpp>
#include <Settings/NamespaceSettings.hpp>
#include <stddef.h> // size_t
#include <stdint.h> // uint8_t

#ifndef ARDUINO
#include <vector>
#endif

AH_DIAGNOSTIC_WERROR()

BEGIN_CS_NAMESPACE

struct ChannelMessage {
    ChannelMessage() : header(0), data1(0), data2(0), CN(0) {}
    ChannelMessage(uint8_t header, uint8_t data1, uint8_t data2, uint8_t CN = 0)
        : header(header), data1(data1), data2(data2), CN(CN) {}

    uint8_t header;
    uint8_t data1;
    uint8_t data2;
    uint8_t CN;

    bool operator==(ChannelMessage other) const {
        return this->header == other.header && this->data1 == other.data1 &&
               this->data2 == other.data2 && this->CN == other.CN;
    }
    bool operator!=(ChannelMessage other) const { return !(*this == other); }
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