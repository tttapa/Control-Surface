#include <MIDI_Interfaces/BLEMIDI/BLEMIDIPacketBuilder.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

USING_CS_NAMESPACE;

static uint16_t timestamp(uint8_t msb, uint8_t lsb) {
    return (uint16_t(msb) << 7) | lsb;
}

using bvec = std::vector<uint8_t>;

// Corresponds to spec "BLE Packet with One MIDI Message" figure
TEST(BLEMIDIPacketBuilder, oneNoteMessage) {
    BLEMIDIPacketBuilder b;
    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0x92,        // status
        0x12,        // d1
        0x34,        // d2
    };
    EXPECT_EQ(b.getPacket(), expected);
}

TEST(BLEMIDIPacketBuilder, oneProgramChangeMessage) {
    BLEMIDIPacketBuilder b;
    EXPECT_TRUE(b.add2B(0xC2, 0x12, timestamp(0x01, 0x02)));

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0xC2,        // status
        0x12,        // d1
    };
    EXPECT_EQ(b.getPacket(), expected);
}

// Corresponds to spec "BLE Packet with Two MIDI Messages" figure
TEST(BLEMIDIPacketBuilder, twoNoteMessages) {
    BLEMIDIPacketBuilder b;
    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));
    EXPECT_TRUE(b.add3B(0x99, 0x56, 0x78, timestamp(0x01, 0x03)));

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0x92,        // status
        0x12,        // d1
        0x34,        // d2
        0x80 | 0x03, //          timestamp lsb
        0x99,        // status
        0x56,        // d1
        0x78,        // d2
    };
    EXPECT_EQ(b.getPacket(), expected);
}

// Corresponds to spec "2 MIDI Messages with Running Status" figure
TEST(BLEMIDIPacketBuilder, twoNoteMessagesRunningStatus) {
    BLEMIDIPacketBuilder b;
    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));
    EXPECT_TRUE(b.add3B(0x92, 0x56, 0x78, timestamp(0x01, 0x02)));

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0x92,        // status
        0x12,        // d1
        0x34,        // d2
        0x56,        // d1
        0x78,        // d2
    };
    EXPECT_EQ(b.getPacket(), expected);
}

TEST(BLEMIDIPacketBuilder, twoNoteMessagesRunningStatusDifferentTimestamp) {
    BLEMIDIPacketBuilder b;
    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));
    EXPECT_TRUE(b.add3B(0x92, 0x56, 0x78, timestamp(0x01, 0x03)));

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0x92,        // status
        0x12,        // d1
        0x34,        // d2
        0x80 | 0x03, //          timestamp lsb
        0x56,        // d1
        0x78,        // d2
    };
    EXPECT_EQ(b.getPacket(), expected);
}

// Corresponds to spec "System Messages Do Not Cancel Running Status" figure
TEST(BLEMIDIPacketBuilder, realTimeBetweenRunningStatus) {
    BLEMIDIPacketBuilder b;
    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));
    EXPECT_TRUE(b.add3B(0x92, 0x56, 0x78, timestamp(0x01, 0x02)));
    EXPECT_TRUE(b.addRealTime(0xF8, timestamp(0x01, 0x02)));
    EXPECT_TRUE(b.add3B(0x92, 0x11, 0x22, timestamp(0x01, 0x02)));

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0x92,        // status
        0x12,        // d1
        0x34,        // d2
        0x56,        // d1
        0x78,        // d2
        0x80 | 0x02, //          timestamp lsb
        0xF8,        // real-time
        0x80 | 0x02, //          timestamp lsb
        0x11,        // d1
        0x22,        // d2
    };
    EXPECT_EQ(b.getPacket(), expected);
}

// Corresponds to spec "System Messages Do Not Cancel Running Status" figure
TEST(BLEMIDIPacketBuilder, realTimeBetweenRunningStatusDifferentTimestamp) {
    BLEMIDIPacketBuilder b;
    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));
    EXPECT_TRUE(b.add3B(0x92, 0x56, 0x78, timestamp(0x01, 0x02)));
    EXPECT_TRUE(b.addRealTime(0xF8, timestamp(0x01, 0x03)));
    EXPECT_TRUE(b.add3B(0x92, 0x11, 0x22, timestamp(0x01, 0x04)));

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0x92,        // status
        0x12,        // d1
        0x34,        // d2
        0x56,        // d1
        0x78,        // d2
        0x80 | 0x03, //          timestamp lsb
        0xF8,        // real-time
        0x80 | 0x04, //          timestamp lsb
        0x11,        // d1
        0x22,        // d2
    };
    EXPECT_EQ(b.getPacket(), expected);
}

TEST(BLEMIDIPacketBuilder, sysCommon1BetweenRunningStatusDifferentTimestamp) {
    BLEMIDIPacketBuilder b;
    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));
    EXPECT_TRUE(b.add3B(0x92, 0x56, 0x78, timestamp(0x01, 0x02)));
    EXPECT_TRUE(b.addSysCommon(0, 0xF6, 0x00, 0x00, timestamp(0x01, 0x03)));
    EXPECT_TRUE(b.add3B(0x92, 0x11, 0x22, timestamp(0x01, 0x04)));

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0x92,        // status
        0x12,        // d1
        0x34,        // d2
        0x56,        // d1
        0x78,        // d2
        0x80 | 0x03, //          timestamp lsb
        0xF6,        // syscom
        0x80 | 0x04, //          timestamp lsb
        0x11,        // d1
        0x22,        // d2
    };
    EXPECT_EQ(b.getPacket(), expected);
}

TEST(BLEMIDIPacketBuilder, sysCommon2BetweenRunningStatusDifferentTimestamp) {
    BLEMIDIPacketBuilder b;
    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));
    EXPECT_TRUE(b.add3B(0x92, 0x56, 0x78, timestamp(0x01, 0x02)));
    EXPECT_TRUE(b.addSysCommon(1, 0xF1, 0x31, 0x00, timestamp(0x01, 0x03)));
    EXPECT_TRUE(b.add3B(0x92, 0x11, 0x22, timestamp(0x01, 0x04)));

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0x92,        // status
        0x12,        // d1
        0x34,        // d2
        0x56,        // d1
        0x78,        // d2
        0x80 | 0x03, //          timestamp lsb
        0xF1,        // syscom
        0x31,        // d1
        0x80 | 0x04, //          timestamp lsb
        0x11,        // d1
        0x22,        // d2
    };
    EXPECT_EQ(b.getPacket(), expected);
}

TEST(BLEMIDIPacketBuilder, sysCommon3BetweenRunningStatusDifferentTimestamp) {
    BLEMIDIPacketBuilder b;
    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));
    EXPECT_TRUE(b.add3B(0x92, 0x56, 0x78, timestamp(0x01, 0x02)));
    EXPECT_TRUE(b.addSysCommon(2, 0xF2, 0x41, 0x74, timestamp(0x01, 0x03)));
    EXPECT_TRUE(b.add3B(0x92, 0x11, 0x22, timestamp(0x01, 0x04)));

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0x92,        // status
        0x12,        // d1
        0x34,        // d2
        0x56,        // d1
        0x78,        // d2
        0x80 | 0x03, //          timestamp lsb
        0xF2,        // syscom
        0x41,        // d1
        0x74,        // d2
        0x80 | 0x04, //          timestamp lsb
        0x11,        // d1
        0x22,        // d2
    };
    EXPECT_EQ(b.getPacket(), expected);
}

TEST(BLEMIDIPacketBuilder, noteMessageBufferFull) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(5 + 3);
    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));
    EXPECT_FALSE(b.add3B(0x99, 0x12, 0x34, timestamp(0x01, 0x02)));

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0x92,        0x12, 0x34,
    };
    EXPECT_EQ(b.getPacket(), expected);
}

TEST(BLEMIDIPacketBuilder, noteMessageBufferAlmostFull) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(5 + 4);
    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));
    EXPECT_TRUE(b.add3B(0x99, 0x12, 0x34, timestamp(0x01, 0x02)));

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0x92,        0x12, 0x34,
        0x80 | 0x02, //          timestamp lsb
        0x99,        0x12, 0x34,
    };
    EXPECT_EQ(b.getPacket(), expected);
}

TEST(BLEMIDIPacketBuilder, noteMessageBufferFullRunningStatus) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(5 + 1);
    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));
    EXPECT_FALSE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0x92,        0x12, 0x34,
    };
    EXPECT_EQ(b.getPacket(), expected);
}

TEST(BLEMIDIPacketBuilder, noteMessageBufferAlmostFullRunningStatus) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(5 + 2);
    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));
    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0x92,        0x12, 0x34, 0x12, 0x34,
    };
    EXPECT_EQ(b.getPacket(), expected);
}

TEST(BLEMIDIPacketBuilder, noteMessageBufferFullDifferentTimestamp) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(5 + 2);
    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));
    EXPECT_FALSE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x03)));

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0x92,        0x12, 0x34,
    };
    EXPECT_EQ(b.getPacket(), expected);
}

TEST(BLEMIDIPacketBuilder, noteMessageBufferAlmostFullDifferentTimestamp) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(5 + 3);
    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));
    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x03)));

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0x92,        0x12, 0x34,
        0x80 | 0x03, //          timestamp lsb
        0x12,        0x34,
    };
    EXPECT_EQ(b.getPacket(), expected);
}

TEST(BLEMIDIPacketBuilder, realTimeMessageBufferAlmostFull) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(7); // Two bytes left after note

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0x92,        // status
        0x12,        // d1
        0x34,        // d2
        0x80 | 0x03, //          timestamp lsb
        0xF8,        // real-time
    };

    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));
    EXPECT_TRUE(b.addRealTime(0xF8, timestamp(0x01, 0x03)));

    EXPECT_EQ(b.getPacket(), expected);
}

TEST(BLEMIDIPacketBuilder, realTimeMessageBufferFull1) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(6); // One byte left after note

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0x92,        // status
        0x12,        // d1
        0x34,        // d2
    };

    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));
    EXPECT_FALSE(b.addRealTime(0xF8, timestamp(0x01, 0x03)));

    EXPECT_EQ(b.getPacket(), expected);
}

TEST(BLEMIDIPacketBuilder, realTimeMessageBufferFull2) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(5); // No space left after note

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0x92,        // status
        0x12,        // d1
        0x34,        // d2
    };

    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));
    EXPECT_FALSE(b.addRealTime(0xF8, timestamp(0x01, 0x03)));

    EXPECT_EQ(b.getPacket(), expected);
}

TEST(BLEMIDIPacketBuilder, sysCommon1MessageBufferAlmostFull) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(7); // Two bytes left after note

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0x92,        // status
        0x12,        // d1
        0x34,        // d2
        0x80 | 0x03, //          timestamp lsb
        0xF6,        // syscom
    };

    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));
    EXPECT_TRUE(b.addSysCommon(0, 0xF6, 0x00, 0x00, timestamp(0x01, 0x03)));

    EXPECT_EQ(b.getPacket(), expected);
}

TEST(BLEMIDIPacketBuilder, sysCommon1MessageBufferFull1) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(6); // One byte left after note

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0x92,        // status
        0x12,        // d1
        0x34,        // d2
    };

    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));
    EXPECT_FALSE(b.addSysCommon(0, 0xF6, 0x00, 0x00, timestamp(0x01, 0x03)));

    EXPECT_EQ(b.getPacket(), expected);
}

TEST(BLEMIDIPacketBuilder, sysCommon1MessageBufferFull2) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(5); // No space left after note

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0x92,        // status
        0x12,        // d1
        0x34,        // d2
    };

    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));
    EXPECT_FALSE(b.addSysCommon(0, 0xF6, 0x00, 0x00, timestamp(0x01, 0x03)));

    EXPECT_EQ(b.getPacket(), expected);
}

// Corresponds to spec "System Exclusive Start & End in 1 Packet" figure
TEST(BLEMIDIPacketBuilder, sysExSinglePacket) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(10); // Exact fit
    bvec data = {0xF0, 0x10, 0x11, 0x12, 0x13, 0x14, 0xF7};
    const uint8_t *dataptr = data.data();
    size_t length = data.size();
    uint16_t ts = timestamp(0x01, 0x02);

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0xF0,        // SysEx start
        0x10,        // data
        0x11,        // data
        0x12,        // data
        0x13,        // data
        0x14,        // data
        0x80 | 0x02, //          timestamp lsb
        0xF7,        // SysEx end
    };

    EXPECT_TRUE(b.addSysEx(dataptr, length, ts));

    EXPECT_EQ(length, 0);
    EXPECT_EQ(dataptr, nullptr);
    EXPECT_EQ(b.getPacket(), expected);
}

TEST(BLEMIDIPacketBuilder, sysExFirstPacketFull1) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(6); // One byte left after note
    bvec data = {0xF0, 0x10, 0x11, 0x12, 0x13, 0x14, 0xF7};
    const uint8_t *dataptr = data.data();
    size_t length = data.size();
    uint16_t ts = timestamp(0x01, 0x02);

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0x92,        // status
        0x12,        // d1
        0x34,        // d2
    };

    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));
    EXPECT_FALSE(b.addSysEx(dataptr, length, ts));

    EXPECT_EQ(length, data.size());
    EXPECT_EQ(dataptr, data.data());
    EXPECT_EQ(b.getPacket(), expected);
}

TEST(BLEMIDIPacketBuilder, sysExFirstPacketFull2) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(5); // No bytes left after note
    bvec data = {0xF0, 0x10, 0x11, 0x12, 0x13, 0x14, 0xF7};
    const uint8_t *dataptr = data.data();
    size_t length = data.size();
    uint16_t ts = timestamp(0x01, 0x02);

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0x92,        // status
        0x12,        // d1
        0x34,        // d2
    };

    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));
    EXPECT_FALSE(b.addSysEx(dataptr, length, ts));

    EXPECT_EQ(length, data.size());
    EXPECT_EQ(dataptr, data.data());
    EXPECT_EQ(b.getPacket(), expected);
}

TEST(BLEMIDIPacketBuilder, sysExAlmostSinglePacket1) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(9); // One byte too short
    bvec data = {0xF0, 0x10, 0x11, 0x12, 0x13, 0x14, 0xF7};
    const uint8_t *dataptr = data.data();
    size_t length = data.size();
    uint16_t ts = timestamp(0x01, 0x02);

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0xF0,        // SysEx start
        0x10,        // data
        0x11,        // data
        0x12,        // data
        0x13,        // data
        0x14,        // data
    };

    EXPECT_TRUE(b.addSysEx(dataptr, length, ts));

    EXPECT_EQ(length, 1);
    EXPECT_EQ(dataptr, data.data() + data.size() - length);
    EXPECT_EQ(b.getPacket(), expected);

    expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0xF7,        // SysEx end
    };

    b.reset();
    b.continueSysEx(dataptr, length, ts);

    EXPECT_EQ(length, 0);
    EXPECT_EQ(dataptr, nullptr);
    EXPECT_EQ(b.getPacket(), expected);
}

TEST(BLEMIDIPacketBuilder, sysExAlmostSinglePacket2) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(8); // Two bytes too short
    bvec data = {0xF0, 0x10, 0x11, 0x12, 0x13, 0x14, 0xF7};
    const uint8_t *dataptr = data.data();
    size_t length = data.size();
    uint16_t ts = timestamp(0x01, 0x02);

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0xF0,        // SysEx start
        0x10,        // data
        0x11,        // data
        0x12,        // data
        0x13,        // data
        0x14,        // data
    };

    EXPECT_TRUE(b.addSysEx(dataptr, length, ts));

    EXPECT_EQ(length, 1);
    EXPECT_EQ(dataptr, data.data() + data.size() - length);
    EXPECT_EQ(b.getPacket(), expected);

    expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0xF7,        // SysEx end
    };

    b.reset();
    b.continueSysEx(dataptr, length, ts);

    EXPECT_EQ(length, 0);
    EXPECT_EQ(dataptr, nullptr);
    EXPECT_EQ(b.getPacket(), expected);
}

TEST(BLEMIDIPacketBuilder, sysExTwoPackets) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(7); // Three bytes too short
    bvec data = {0xF0, 0x10, 0x11, 0x12, 0x13, 0x14, 0xF7};
    const uint8_t *dataptr = data.data();
    size_t length = data.size();
    uint16_t ts = timestamp(0x01, 0x02);

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0xF0,        // SysEx start
        0x10,        // data
        0x11,        // data
        0x12,        // data
        0x13,        // data
    };

    EXPECT_TRUE(b.addSysEx(dataptr, length, ts));

    EXPECT_EQ(length, 2);
    EXPECT_EQ(dataptr, data.data() + data.size() - length);
    EXPECT_EQ(b.getPacket(), expected);

    expected = {
        0x80 | 0x01, // header + timestamp msb
        0x14,        // data
        0x80 | 0x02, //          timestamp lsb
        0xF7,        // SysEx end
    };

    b.reset();
    b.continueSysEx(dataptr, length, ts);

    EXPECT_EQ(length, 0);
    EXPECT_EQ(dataptr, nullptr);
    EXPECT_EQ(b.getPacket(), expected);
}

TEST(BLEMIDIPacketBuilder, sysExAlmostTwoPackets1) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(5);
    bvec data = {0xF0, 0x10, 0x11, 0x12, 0x13, 0x14, 0xF7};
    const uint8_t *dataptr = data.data();
    size_t length = data.size();
    uint16_t ts = timestamp(0x01, 0x02);

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0xF0,        // SysEx start
        0x10,        // data
        0x11,        // data
    };

    EXPECT_TRUE(b.addSysEx(dataptr, length, ts));

    EXPECT_EQ(length, 4);
    EXPECT_EQ(dataptr, data.data() + data.size() - length);
    EXPECT_EQ(b.getPacket(), expected);

    expected = {
        0x80 | 0x01, // header + timestamp msb
        0x12,        // data
        0x13,        // data
        0x14,        // data
    };

    b.reset();
    b.continueSysEx(dataptr, length, ts);

    EXPECT_EQ(length, 1);
    EXPECT_EQ(dataptr, data.data() + data.size() - length);
    EXPECT_EQ(b.getPacket(), expected);

    expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0xF7,        // SysEx end
    };

    b.reset();
    b.continueSysEx(dataptr, length, ts);

    EXPECT_EQ(length, 0);
    EXPECT_EQ(dataptr, nullptr);
    EXPECT_EQ(b.getPacket(), expected);
}

TEST(BLEMIDIPacketBuilder, sysExAlmostTwoPackets2) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(5);
    bvec data = {0xF0, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0xF7};
    const uint8_t *dataptr = data.data();
    size_t length = data.size();
    uint16_t ts = timestamp(0x01, 0x02);

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0xF0,        // SysEx start
        0x10,        // data
        0x11,        // data
    };

    EXPECT_TRUE(b.addSysEx(dataptr, length, ts));

    EXPECT_EQ(length, 5);
    EXPECT_EQ(dataptr, data.data() + data.size() - length);
    EXPECT_EQ(b.getPacket(), expected);

    expected = {
        0x80 | 0x01, // header + timestamp msb
        0x12,        // data
        0x13,        // data
        0x14,        // data
        0x15,        // data
    };

    b.reset();
    b.continueSysEx(dataptr, length, ts);

    EXPECT_EQ(length, 1);
    EXPECT_EQ(dataptr, data.data() + data.size() - length);
    EXPECT_EQ(b.getPacket(), expected);

    expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0xF7,        // SysEx end
    };

    b.reset();
    b.continueSysEx(dataptr, length, ts);

    EXPECT_EQ(length, 0);
    EXPECT_EQ(dataptr, nullptr);
    EXPECT_EQ(b.getPacket(), expected);
}

TEST(BLEMIDIPacketBuilder, sysExThreePackets) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(5);
    bvec data = {0xF0, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0xF7};
    const uint8_t *dataptr = data.data();
    size_t length = data.size();
    uint16_t ts = timestamp(0x01, 0x02);

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0xF0,        // SysEx start
        0x10,        // data
        0x11,        // data
    };

    EXPECT_TRUE(b.addSysEx(dataptr, length, ts));

    EXPECT_EQ(length, 6);
    EXPECT_EQ(dataptr, data.data() + data.size() - length);
    EXPECT_EQ(b.getPacket(), expected);

    expected = {
        0x80 | 0x01, // header + timestamp msb
        0x12,        // data
        0x13,        // data
        0x14,        // data
        0x15,        // data
    };

    b.reset();
    b.continueSysEx(dataptr, length, ts);

    EXPECT_EQ(length, 2);
    EXPECT_EQ(dataptr, data.data() + data.size() - length);
    EXPECT_EQ(b.getPacket(), expected);

    expected = {
        0x80 | 0x01, // header + timestamp msb
        0x16,        // data
        0x80 | 0x02, //          timestamp lsb
        0xF7,        // SysEx end
    };

    b.reset();
    b.continueSysEx(dataptr, length, ts);

    EXPECT_EQ(length, 0);
    EXPECT_EQ(dataptr, nullptr);
    EXPECT_EQ(b.getPacket(), expected);
}

TEST(BLEMIDIPacketBuilder, sysExAddChunk) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(6);
    bvec data = {0x10, 0x11, 0x12, 0x13, 0x14};
    const uint8_t *dataptr = data.data();
    size_t length = data.size();
    uint16_t ts = timestamp(0x01, 0x02);

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x10,        // data
        0x11,        // data
        0x12,        // data
        0x13,        // data
        0x14,        // data
    };

    b.addSysEx(dataptr, length, ts);

    EXPECT_EQ(length, 0);
    EXPECT_EQ(dataptr, nullptr);
    EXPECT_EQ(b.getPacket(), expected);
}

TEST(BLEMIDIPacketBuilder, sysExAddChunk0B) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(50);
    bvec data = {};
    const uint8_t *dataptr = data.data();
    size_t length = data.size();
    uint16_t ts = timestamp(0x01, 0x02);

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
    };

    b.addSysEx(dataptr, length, ts);

    EXPECT_EQ(length, 0);
    EXPECT_EQ(dataptr, nullptr);
    EXPECT_EQ(b.getPacket(), expected);
}

TEST(BLEMIDIPacketBuilder, sysExAddChunk1B) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(50);
    bvec data = {0xF0};
    const uint8_t *dataptr = data.data();
    size_t length = data.size();
    uint16_t ts = timestamp(0x01, 0x02);

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0xF0,        // SysEx start
    };

    b.addSysEx(dataptr, length, ts);

    EXPECT_EQ(length, 0);
    EXPECT_EQ(dataptr, nullptr);
    EXPECT_EQ(b.getPacket(), expected);

    data = {0xF7};
    dataptr = data.data();
    length = data.size();
    ts = timestamp(0x01, 0x03);

    expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0xF0,        // SysEx start
        0x80 | 0x03, //          timestamp lsb
        0xF7,        // SysEx end
    };

    b.addSysEx(dataptr, length, ts);

    EXPECT_EQ(length, 0);
    EXPECT_EQ(dataptr, nullptr);
    EXPECT_EQ(b.getPacket(), expected);
}

TEST(BLEMIDIPacketBuilder, sysExAddChunkTwoPackets) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(6);
    bvec data = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16};
    const uint8_t *dataptr = data.data();
    size_t length = data.size();
    uint16_t ts = timestamp(0x01, 0x02);

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x10,        // data
        0x11,        // data
        0x12,        // data
        0x13,        // data
        0x14,        // data
    };

    b.addSysEx(dataptr, length, ts);

    EXPECT_EQ(length, 2);
    EXPECT_EQ(dataptr, data.data() + data.size() - length);
    EXPECT_EQ(b.getPacket(), expected);

    expected = {
        0x80 | 0x01, // header + timestamp msb
        0x15,        // data
        0x16,        // data
    };

    b.reset();
    b.continueSysEx(dataptr, length, ts);

    EXPECT_EQ(length, 0);
    EXPECT_EQ(dataptr, nullptr);
    EXPECT_EQ(b.getPacket(), expected);
}

TEST(BLEMIDIPacketBuilder, sysExAddChunkTwoPackets1ByteLeft) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(6);
    bvec data = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15};
    const uint8_t *dataptr = data.data();
    size_t length = data.size();
    uint16_t ts = timestamp(0x01, 0x02);

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x10,        // data
        0x11,        // data
        0x12,        // data
        0x13,        // data
        0x14,        // data
    };

    b.addSysEx(dataptr, length, ts);

    EXPECT_EQ(length, 1);
    EXPECT_EQ(dataptr, data.data() + data.size() - length);
    EXPECT_EQ(b.getPacket(), expected);

    expected = {
        0x80 | 0x01, // header + timestamp msb
        0x15,        // data
    };

    b.reset();
    b.continueSysEx(dataptr, length, ts);

    EXPECT_EQ(length, 0);
    EXPECT_EQ(dataptr, nullptr);
    EXPECT_EQ(b.getPacket(), expected);
}

TEST(BLEMIDIPacketBuilder, sysExAddChunkEndTwoPackets) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(6);
    bvec data = {0x10, 0x11, 0x12, 0x13, 0xF7};
    const uint8_t *dataptr = data.data();
    size_t length = data.size();
    uint16_t ts = timestamp(0x01, 0x02);

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x10,        // data
        0x11,        // data
        0x12,        // data
        0x13,        // data
                     // End doesn't fit
    };

    b.addSysEx(dataptr, length, ts);

    EXPECT_EQ(length, 1);
    EXPECT_EQ(dataptr, data.data() + data.size() - length);
    EXPECT_EQ(b.getPacket(), expected);

    expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0xF7,        // SysExEnd
    };

    b.reset();
    b.continueSysEx(dataptr, length, ts);

    EXPECT_EQ(length, 0);
    EXPECT_EQ(dataptr, nullptr);
    EXPECT_EQ(b.getPacket(), expected);
}