#include <MIDI_Interfaces/BLEMIDI/BLEMIDIPacketBuilder.hpp>

#include <gmock-wrapper.h>
#include <gtest-wrapper.h>

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
    EXPECT_EQ(b.asVector(), expected);
}

// Corresponds to spec "BLE Packet with Two MIDI Messages" figure
TEST(BLEMIDIPacketBuilder, twoNoteMessages) {
    BLEMIDIPacketBuilder b;
    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));
    EXPECT_TRUE(b.add3B(0x99, 0x56, 0x78, timestamp(0x01, 0x03)));

    bvec expected = {
        0x80 | 0x01,    // header + timestamp msb
        0x80 | 0x02,    //          timestamp lsb
        0x92,           // status
        0x12,           // d1
        0x34,           // d2
        0x80 | 0x03,    //          timestamp lsb
        0x99,           // status
        0x56,           // d1
        0x78,           // d2
    };
    EXPECT_EQ(b.asVector(), expected);
}

// Corresponds to spec "2 MIDI Messages with Running Status" figure
TEST(BLEMIDIPacketBuilder, twoNoteMessagesRunningStatus) {
    BLEMIDIPacketBuilder b;
    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));
    EXPECT_TRUE(b.add3B(0x92, 0x56, 0x78, timestamp(0x01, 0x02)));

    bvec expected = {
        0x80 | 0x01,    // header + timestamp msb
        0x80 | 0x02,    //          timestamp lsb
        0x92,           // status
        0x12,           // d1
        0x34,           // d2
        0x56,           // d1
        0x78,           // d2
    };
    EXPECT_EQ(b.asVector(), expected);
}

TEST(BLEMIDIPacketBuilder, twoNoteMessagesRunningStatusDifferentTimestamp) {
    BLEMIDIPacketBuilder b;
    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));
    EXPECT_TRUE(b.add3B(0x92, 0x56, 0x78, timestamp(0x01, 0x03)));

    bvec expected = {
        0x80 | 0x01,    // header + timestamp msb
        0x80 | 0x02,    //          timestamp lsb
        0x92,           // status
        0x12,           // d1
        0x34,           // d2
        0x80 | 0x03,    //          timestamp lsb
        0x56,           // d1
        0x78,           // d2
    };
    EXPECT_EQ(b.asVector(), expected);
}

// Corresponds to spec "System Messages Do Not Cancel Running Status" figure
TEST(BLEMIDIPacketBuilder, realTimeBetweenRunningStatus) {
    BLEMIDIPacketBuilder b;
    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));
    EXPECT_TRUE(b.add3B(0x92, 0x56, 0x78, timestamp(0x01, 0x02)));
    EXPECT_TRUE(b.addRealTime(0xF8, timestamp(0x01, 0x02)));
    EXPECT_TRUE(b.add3B(0x92, 0x11, 0x22, timestamp(0x01, 0x02)));

    bvec expected = {
        0x80 | 0x01,    // header + timestamp msb
        0x80 | 0x02,    //          timestamp lsb
        0x92,           // status
        0x12,           // d1
        0x34,           // d2
        0x56,           // d1
        0x78,           // d2
        0x80 | 0x02,    //          timestamp lsb
        0xF8,           // real-time
        0x80 | 0x02,    //          timestamp lsb
        0x11,           // d1
        0x22,           // d2
    };
    EXPECT_EQ(b.asVector(), expected);
}

// Corresponds to spec "System Messages Do Not Cancel Running Status" figure
TEST(BLEMIDIPacketBuilder, realTimeBetweenRunningStatusDifferentTimestamp) {
    BLEMIDIPacketBuilder b;
    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));
    EXPECT_TRUE(b.add3B(0x92, 0x56, 0x78, timestamp(0x01, 0x02)));
    EXPECT_TRUE(b.addRealTime(0xF8, timestamp(0x01, 0x03)));
    EXPECT_TRUE(b.add3B(0x92, 0x11, 0x22, timestamp(0x01, 0x04)));

    bvec expected = {
        0x80 | 0x01,    // header + timestamp msb
        0x80 | 0x02,    //          timestamp lsb
        0x92,           // status
        0x12,           // d1
        0x34,           // d2
        0x56,           // d1
        0x78,           // d2
        0x80 | 0x03,    //          timestamp lsb
        0xF8,           // real-time
        0x80 | 0x04,    //          timestamp lsb
        0x11,           // d1
        0x22,           // d2
    };
    EXPECT_EQ(b.asVector(), expected);
}

TEST(BLEMIDIPacketBuilder, noteMessageBufferFull) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(5 + 3);
    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));
    EXPECT_FALSE(b.add3B(0x99, 0x12, 0x34, timestamp(0x01, 0x02)));

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0x92, 0x12, 0x34,
    };
    EXPECT_EQ(b.asVector(), expected);
}

TEST(BLEMIDIPacketBuilder, noteMessageBufferAlmostFull) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(5 + 4);
    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));
    EXPECT_TRUE(b.add3B(0x99, 0x12, 0x34, timestamp(0x01, 0x02)));

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0x92, 0x12, 0x34,
        0x80 | 0x02, //          timestamp lsb
        0x99, 0x12, 0x34,
    };
    EXPECT_EQ(b.asVector(), expected);
}

TEST(BLEMIDIPacketBuilder, noteMessageBufferFullRunningStatus) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(5 + 1);
    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));
    EXPECT_FALSE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0x92, 0x12, 0x34,
    };
    EXPECT_EQ(b.asVector(), expected);
}

TEST(BLEMIDIPacketBuilder, noteMessageBufferAlmostFullRunningStatus) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(5 + 2);
    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));
    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0x92, 0x12, 0x34,
        0x12, 0x34,
    };
    EXPECT_EQ(b.asVector(), expected);
}

TEST(BLEMIDIPacketBuilder, noteMessageBufferFullDifferentTimestamp) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(5 + 2);
    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));
    EXPECT_FALSE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x03)));

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0x92, 0x12, 0x34,
    };
    EXPECT_EQ(b.asVector(), expected);
}

TEST(BLEMIDIPacketBuilder, noteMessageBufferAlmostFullDifferentTimestamp) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(5 + 3);
    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x02)));
    EXPECT_TRUE(b.add3B(0x92, 0x12, 0x34, timestamp(0x01, 0x03)));

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0x92, 0x12, 0x34,
        0x80 | 0x03, //          timestamp lsb
        0x12, 0x34,
    };
    EXPECT_EQ(b.asVector(), expected);
}

// Corresponds to spec "System Exclusive Start & End in 1 Packet" figure
TEST(BLEMIDIPacketBuilder, sysExSinglePacket) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(10); // Exact fit
    bvec data = {0x10, 0x11, 0x12, 0x13, 0x14};
    const uint8_t *cont;
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

    size_t s = b.addSysEx(data.data(), data.size(), cont, ts);

    EXPECT_EQ(s, 0);
    EXPECT_EQ(cont, nullptr);
    EXPECT_EQ(b.asVector(), expected);
}

TEST(BLEMIDIPacketBuilder, sysExAlmostSinglePacket1) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(9); // One byte too short
    bvec data = {0x10, 0x11, 0x12, 0x13, 0x14};
    const uint8_t *cont;
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

    size_t s = b.addSysEx(data.data(), data.size(), cont, ts);

    EXPECT_EQ(s, 0);
    EXPECT_EQ(cont, data.data() + data.size());
    EXPECT_EQ(b.asVector(), expected);

    expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0xF7,        // SysEx end
    };

    b.reset();
    s = b.continueSysEx(cont, s, cont, ts);

    EXPECT_EQ(s, 0);
    EXPECT_EQ(cont, nullptr);
    EXPECT_EQ(b.asVector(), expected);
}

TEST(BLEMIDIPacketBuilder, sysExAlmostSinglePacket2) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(8); // Two bytes too short
    bvec data = {0x10, 0x11, 0x12, 0x13, 0x14};
    const uint8_t *cont;
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

    size_t s = b.addSysEx(data.data(), data.size(), cont, ts);

    EXPECT_EQ(s, 0);
    EXPECT_EQ(cont, data.data() + data.size());
    EXPECT_EQ(b.asVector(), expected);

    expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0xF7,        // SysEx end
    };

    b.reset();
    s = b.continueSysEx(cont, s, cont, ts);

    EXPECT_EQ(s, 0);
    EXPECT_EQ(cont, nullptr);
    EXPECT_EQ(b.asVector(), expected);
}

TEST(BLEMIDIPacketBuilder, sysExTwoPackets) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(7); // Three bytes too short
    bvec data = {0x10, 0x11, 0x12, 0x13, 0x14};
    const uint8_t *cont;
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

    size_t s = b.addSysEx(data.data(), data.size(), cont, ts);

    EXPECT_EQ(s, 1);
    EXPECT_EQ(cont, data.data() + data.size() - s);
    EXPECT_EQ(b.asVector(), expected);

    expected = {
        0x80 | 0x01, // header + timestamp msb
        0x14,        // data
        0x80 | 0x02, //          timestamp lsb
        0xF7,        // SysEx end
    };

    b.reset();
    s = b.continueSysEx(cont, s, cont, ts);

    EXPECT_EQ(s, 0);
    EXPECT_EQ(cont, nullptr);
    EXPECT_EQ(b.asVector(), expected);
}

TEST(BLEMIDIPacketBuilder, sysExAlmostTwoPackets1) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(5);
    bvec data = {0x10, 0x11, 0x12, 0x13, 0x14};
    const uint8_t *cont;
    uint16_t ts = timestamp(0x01, 0x02);

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0xF0,        // SysEx start
        0x10,        // data
        0x11,        // data
    };

    size_t s = b.addSysEx(data.data(), data.size(), cont, ts);

    EXPECT_EQ(s, 3);
    EXPECT_EQ(cont, data.data() + data.size() - s);
    EXPECT_EQ(b.asVector(), expected);

    expected = {
        0x80 | 0x01, // header + timestamp msb
        0x12,        // data
        0x13,        // data
        0x14,        // data
    };

    b.reset();
    s = b.continueSysEx(cont, s, cont, ts);

    EXPECT_EQ(s, 0);
    EXPECT_EQ(cont, data.data() + data.size() - s);
    EXPECT_EQ(b.asVector(), expected);

    expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0xF7,        // SysEx end
    };

    b.reset();
    s = b.continueSysEx(cont, s, cont, ts);

    EXPECT_EQ(s, 0);
    EXPECT_EQ(cont, nullptr);
    EXPECT_EQ(b.asVector(), expected);
}

TEST(BLEMIDIPacketBuilder, sysExAlmostTwoPackets2) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(5);
    bvec data = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15};
    const uint8_t *cont;
    uint16_t ts = timestamp(0x01, 0x02);

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0xF0,        // SysEx start
        0x10,        // data
        0x11,        // data
    };

    size_t s = b.addSysEx(data.data(), data.size(), cont, ts);

    EXPECT_EQ(s, 4);
    EXPECT_EQ(cont, data.data() + data.size() - s);
    EXPECT_EQ(b.asVector(), expected);

    expected = {
        0x80 | 0x01, // header + timestamp msb
        0x12,        // data
        0x13,        // data
        0x14,        // data
        0x15,        // data
    };

    b.reset();
    s = b.continueSysEx(cont, s, cont, ts);

    EXPECT_EQ(s, 0);
    EXPECT_EQ(cont, data.data() + data.size() - s);
    EXPECT_EQ(b.asVector(), expected);

    expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0xF7,        // SysEx end
    };

    b.reset();
    s = b.continueSysEx(cont, s, cont, ts);

    EXPECT_EQ(s, 0);
    EXPECT_EQ(cont, nullptr);
    EXPECT_EQ(b.asVector(), expected);
}

TEST(BLEMIDIPacketBuilder, sysExThreePackets) {
    BLEMIDIPacketBuilder b;
    b.setCapacity(5);
    bvec data = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16};
    const uint8_t *cont;
    uint16_t ts = timestamp(0x01, 0x02);

    bvec expected = {
        0x80 | 0x01, // header + timestamp msb
        0x80 | 0x02, //          timestamp lsb
        0xF0,        // SysEx start
        0x10,        // data
        0x11,        // data
    };

    size_t s = b.addSysEx(data.data(), data.size(), cont, ts);

    EXPECT_EQ(s, 5);
    EXPECT_EQ(cont, data.data() + data.size() - s);
    EXPECT_EQ(b.asVector(), expected);

    expected = {
        0x80 | 0x01, // header + timestamp msb
        0x12,        // data
        0x13,        // data
        0x14,        // data
        0x15,        // data
    };

    b.reset();
    s = b.continueSysEx(cont, s, cont, ts);

    EXPECT_EQ(s, 1);
    EXPECT_EQ(cont, data.data() + data.size() - s);
    EXPECT_EQ(b.asVector(), expected);

    expected = {
        0x80 | 0x01, // header + timestamp msb
        0x16,        // data
        0x80 | 0x02, //          timestamp lsb
        0xF7,        // SysEx end
    };

    b.reset();
    s = b.continueSysEx(cont, s, cont, ts);

    EXPECT_EQ(s, 0);
    EXPECT_EQ(cont, nullptr);
    EXPECT_EQ(b.asVector(), expected);
}
