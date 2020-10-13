#include <MIDI_Interfaces/BluetoothMIDI_Interface.hpp>
#include <MIDI_Interfaces/MIDI_Callbacks.hpp>

using namespace CS;
using testing::Mock;

class MockMIDI_Callbacks : public MIDI_Callbacks {
  public:
    void onChannelMessage(MIDI_Interface &, ChannelMessage msg) override {
        channelMessages.push_back(msg);
    }
    void onSysExMessage(MIDI_Interface &, SysExMessage msg) override {
        sysExMessages.insert(sysExMessages.end(), msg.data,
                             msg.data + msg.length);
        sysExCounter++;
    }
    void onRealTimeMessage(MIDI_Interface &, RealTimeMessage msg) override {
        realtimeMessages.push_back(msg);
    }

    std::vector<ChannelMessage> channelMessages;
    std::vector<uint8_t> sysExMessages;
    std::vector<RealTimeMessage> realtimeMessages;
    size_t sysExCounter = 0;
};

TEST(BluetoothMIDIInterface, initializeBegin) {
    BluetoothMIDI_Interface midi;
    midi.begin();
}

TEST(BluetoothMIDIInterface, notInitialized) {
    BluetoothMIDI_Interface midi;
    // Destructor shouldn't fail if thread wasn't started
}

TEST(BluetoothMIDIInterface, receiveChannelMessage) {
    MockMIDI_Callbacks cb;

    BluetoothMIDI_Interface midi;
    midi.begin();
    midi.setCallbacks(&cb);

    uint8_t data[] = {0x80, 0x80, 0x90, 0x3C, 0x7F};
    midi.parse(data, sizeof(data));

    std::vector<uint8_t> expectedSysExMessages = {};
    EXPECT_EQ(cb.sysExMessages, expectedSysExMessages);
    EXPECT_EQ(cb.sysExCounter, 0);

    std::vector<ChannelMessage> expectedChannelMessages = {
        {0x90, 0x3C, 0x7F, 0x00},
    };
    EXPECT_EQ(cb.channelMessages, expectedChannelMessages);
}

TEST(BluetoothMIDIInterface, receiveMultipleChannelMessage) {
    MockMIDI_Callbacks cb;

    BluetoothMIDI_Interface midi;
    midi.begin();
    midi.setCallbacks(&cb);

    uint8_t data[] = {0x80, 0x80, 0x90, 0x3C, 0x7F, 0x80, 0x80,
                      0x3D, 0x7E, 0x80, 0xB1, 0x10, 0x40};
    midi.parse(data, sizeof(data));

    std::vector<uint8_t> expectedSysExMessages = {};
    EXPECT_EQ(cb.sysExMessages, expectedSysExMessages);
    EXPECT_EQ(cb.sysExCounter, 0);

    std::vector<ChannelMessage> expectedChannelMessages = {
        {0x90, 0x3C, 0x7F, 0x00},
        {0x80, 0x3D, 0x7E, 0x00},
        {0xB1, 0x10, 0x40, 0x00},
    };
    EXPECT_EQ(cb.channelMessages, expectedChannelMessages);
}

TEST(BluetoothMIDIInterface, receiveMultipleChannelMessageRunningStatus) {
    MockMIDI_Callbacks cb;

    BluetoothMIDI_Interface midi;
    midi.begin();
    midi.setCallbacks(&cb);

    uint8_t data[] = {0x80, 0x80, 0x90, 0x3C, 0x7F, 0x3D,
                      0x7E, 0x80, 0xB1, 0x10, 0x40};
    midi.parse(data, sizeof(data));

    std::vector<uint8_t> expectedSysExMessages = {};
    EXPECT_EQ(cb.sysExMessages, expectedSysExMessages);
    EXPECT_EQ(cb.sysExCounter, 0);

    std::vector<ChannelMessage> expectedChannelMessages = {
        {0x90, 0x3C, 0x7F, 0x00},
        {0x90, 0x3D, 0x7E, 0x00},
        {0xB1, 0x10, 0x40, 0x00},
    };
    EXPECT_EQ(cb.channelMessages, expectedChannelMessages);
}

TEST(BluetoothMIDIInterface,
     receiveMultipleChannelMessageRunningStatusRealTime) {
    MockMIDI_Callbacks cb;

    BluetoothMIDI_Interface midi;
    midi.begin();
    midi.setCallbacks(&cb);

    uint8_t data[] = {0x80, 0x80, 0x90, 0x3C,
                      0x7F, 0x80, 0xF8, 0x80, // Real Time
                      0x3D, 0x7E,             // Continuation of note on
                      0x80, 0xB1, 0x10, 0x40};
    midi.parse(data, sizeof(data));

    std::vector<uint8_t> expectedSysExMessages = {};
    EXPECT_EQ(cb.sysExMessages, expectedSysExMessages);
    EXPECT_EQ(cb.sysExCounter, 0);

    std::vector<ChannelMessage> expectedChannelMessages = {
        {0x90, 0x3C, 0x7F, 0x00},
        {0x90, 0x3D, 0x7E, 0x00},
        {0xB1, 0x10, 0x40, 0x00},
    };
    EXPECT_EQ(cb.channelMessages, expectedChannelMessages);
}

TEST(BluetoothMIDIInterface, receiveMultipleTwoByteChannelMessage) {
    MockMIDI_Callbacks cb;

    BluetoothMIDI_Interface midi;
    midi.begin();
    midi.setCallbacks(&cb);

    uint8_t data[] = {0x80, 0x80, 0xD0, 0x3C, 0x80, 0xC0,
                      0x3D, 0x80, 0xB1, 0x10, 0x40};
    midi.parse(data, sizeof(data));

    std::vector<uint8_t> expectedSysExMessages = {};
    EXPECT_EQ(cb.sysExMessages, expectedSysExMessages);
    EXPECT_EQ(cb.sysExCounter, 0);

    std::vector<ChannelMessage> expectedChannelMessages = {
        {0xD0, 0x3C, 0x00, 0x00},
        {0xC0, 0x3D, 0x00, 0x00},
        {0xB1, 0x10, 0x40, 0x00},
    };
    EXPECT_EQ(cb.channelMessages, expectedChannelMessages);
}

TEST(BluetoothMIDIInterface, receiveSysEx) {
    MockMIDI_Callbacks cb;

    BluetoothMIDI_Interface midi;
    midi.begin();
    midi.setCallbacks(&cb);

    uint8_t data[] = {0x80, 0x80, 0xF0, 0x01, 0x02, 0x03, 0x04, 0x80, 0xF7};
    midi.parse(data, sizeof(data));

    std::vector<uint8_t> expectedSysExMessages = {0xF0, 0x01, 0x02,
                                                  0x03, 0x04, 0xF7};
    EXPECT_EQ(cb.sysExMessages, expectedSysExMessages);
    EXPECT_EQ(cb.sysExCounter, 1);

    std::vector<ChannelMessage> expectedChannelMessages = {};
    EXPECT_EQ(cb.channelMessages, expectedChannelMessages);
}

TEST(BluetoothMIDIInterface, receiveSysEx2) {
    MockMIDI_Callbacks cb;

    BluetoothMIDI_Interface midi;
    midi.begin();
    midi.setCallbacks(&cb);

    uint8_t data[] = {0x95, 0xED, 0xF0, 0x1, 0x2, 0x3, 0x4, 0xED, 0xF7};
    midi.parse(data, sizeof(data));

    std::vector<uint8_t> expectedSysExMessages = {0xF0, 0x01, 0x02,
                                                  0x03, 0x04, 0xF7};
    EXPECT_EQ(cb.sysExMessages, expectedSysExMessages);
    EXPECT_EQ(cb.sysExCounter, 1);

    std::vector<ChannelMessage> expectedChannelMessages = {};
    EXPECT_EQ(cb.channelMessages, expectedChannelMessages);
}

TEST(BluetoothMIDIInterface, receiveSysExSplitAcrossPackets) {
    MockMIDI_Callbacks cb;

    BluetoothMIDI_Interface midi;
    midi.begin();
    midi.setCallbacks(&cb);

    uint8_t data1[] = {0x81, 0x82, 0xF0, 0x01, 0x02};
    uint8_t data2[] = {0x83, 0x03, 0x04, 0x84, 0xF7};
    midi.parse(data1, sizeof(data1));
    midi.parse(data2, sizeof(data2));

    std::vector<uint8_t> expectedSysExMessages = {0xF0, 0x01, 0x02,
                                                  0x03, 0x04, 0xF7};
    EXPECT_EQ(cb.sysExMessages, expectedSysExMessages);
    EXPECT_EQ(cb.sysExCounter, 1);

    std::vector<ChannelMessage> expectedChannelMessages = {};
    EXPECT_EQ(cb.channelMessages, expectedChannelMessages);
}

TEST(BluetoothMIDIInterface, receiveSysExAndRealTime) {
    MockMIDI_Callbacks cb;

    BluetoothMIDI_Interface midi;
    midi.begin();
    midi.setCallbacks(&cb);

    uint8_t data[] = {0x81, 0x82, 0xF0, 0x01, 0x02,
                      0x83, 0xF8, 0x84, // this is a system real time message
                      0x03, 0x04, 0x85, 0xF7};
    midi.parse(data, sizeof(data));

    std::vector<uint8_t> expectedSysExMessages = {0xF0, 0x01, 0x02,
                                                  0x03, 0x04, 0xF7};
    EXPECT_EQ(cb.sysExMessages, expectedSysExMessages);
    EXPECT_EQ(cb.sysExCounter, 1);

    std::vector<ChannelMessage> expectedChannelMessages = {};
    EXPECT_EQ(cb.channelMessages, expectedChannelMessages);
}

using namespace ::testing;

static uint16_t timestamp(uint8_t msb, uint8_t lsb) {
    return (uint16_t(msb) << 7) | lsb;
}

TEST(BluetoothMIDIInterface, sendOneNoteMessage) {
    BluetoothMIDI_Interface midi;
    BLEMIDI &ble = midi.getBLEMIDI();
    midi.begin();

    std::vector<uint8_t> expected = {0x81, 0x82, 0x92, 0x12, 0x34};
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(1) // For time stamp
        .WillRepeatedly(Return(timestamp(0x01, 0x02)));
    EXPECT_CALL(ble, notifyValue(expected));

    midi.sendNoteOn({0x12, CHANNEL_3}, 0x34);
    midi.flush();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(BluetoothMIDIInterface, sendTwoNoteMessages) {
    BluetoothMIDI_Interface midi;
    BLEMIDI &ble = midi.getBLEMIDI();
    midi.begin();

    std::vector<uint8_t> expected = {
        0x81, 0x82, 0x92, 0x12, 0x34, 0x82, 0x99, 0x56, 0x78,
    };
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2) // For time stamp
        .WillRepeatedly(Return(timestamp(0x01, 0x02)));
    EXPECT_CALL(ble, notifyValue(expected));

    midi.sendNoteOn({0x12, CHANNEL_3}, 0x34);
    midi.sendNoteOn({0x56, CHANNEL_10}, 0x78);
    midi.flush();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(BluetoothMIDIInterface, sendTwoNoteMessagesRunningStatus) {
    BluetoothMIDI_Interface midi;
    BLEMIDI &ble = midi.getBLEMIDI();
    midi.begin();

    std::vector<uint8_t> expected = {
        0x81, 0x82, 0x92, 0x12, 0x34, 0x56, 0x78,
    };
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2) // For time stamp
        .WillRepeatedly(Return(timestamp(0x01, 0x02)));
    EXPECT_CALL(ble, notifyValue(expected));

    midi.sendNoteOn({0x12, CHANNEL_3}, 0x34);
    midi.sendNoteOn({0x56, CHANNEL_3}, 0x78);
    midi.flush();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(BluetoothMIDIInterface, sendLongSysEx) {
    std::chrono::milliseconds timeout {100};
    BluetoothMIDI_Interface midi;
    BLEMIDI &ble = midi.getBLEMIDI();
    midi.begin();
    midi.forceMinMTU(5 + 3);
    midi.setTimeout(timeout);

    std::vector<uint8_t> sysex = {
        0xF0, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0xF7,
    };

    std::vector<uint8_t> expected[] = {
        {
            0x80 | 0x01, // header + timestamp msb
            0x80 | 0x02, //          timestamp lsb
            0xF0,        // SysEx start
            0x10,        // data
            0x11,        // data
        },
        {
            0x80 | 0x01, // header + timestamp msb
            0x12,        // data
            0x13,        // data
            0x14,        // data
            0x15,        // data
        },
        {
            0x80 | 0x01, // header + timestamp msb
            0x16,        // data
            0x80 | 0x02, //          timestamp lsb
            0xF7,        // SysEx end
        },
    };

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(1) // For time stamp
        .WillRepeatedly(Return(timestamp(0x01, 0x02)));
    
    InSequence seq;
    EXPECT_CALL(ble, notifyValue(expected[0]));
    EXPECT_CALL(ble, notifyValue(expected[1]));

    midi.send(SysExMessage(sysex));
    // First two packets should be sent immediately
    Mock::VerifyAndClear(&ble);
    
    // Third packet is sent after the timeout
    std::this_thread::sleep_for(timeout * 0.9);
    EXPECT_CALL(ble, notifyValue(expected[2]));
    std::this_thread::sleep_for(timeout * 0.2);
    Mock::VerifyAndClear(&ble);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(BluetoothMIDIInterface, sendLongSysExFlush) {
    std::chrono::milliseconds timeout {100};
    BluetoothMIDI_Interface midi;
    BLEMIDI &ble = midi.getBLEMIDI();
    midi.begin();
    midi.forceMinMTU(5 + 3);
    midi.setTimeout(timeout);

    std::vector<uint8_t> sysex = {
        0xF0, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0xF7,
    };

    std::vector<uint8_t> expected[] = {
        {
            0x80 | 0x01, // header + timestamp msb
            0x80 | 0x02, //          timestamp lsb
            0xF0,        // SysEx start
            0x10,        // data
            0x11,        // data
        },
        {
            0x80 | 0x01, // header + timestamp msb
            0x12,        // data
            0x13,        // data
            0x14,        // data
            0x15,        // data
        },
        {
            0x80 | 0x01, // header + timestamp msb
            0x16,        // data
            0x80 | 0x02, //          timestamp lsb
            0xF7,        // SysEx end
        },
    };

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(1) // For time stamp
        .WillRepeatedly(Return(timestamp(0x01, 0x02)));
    
    InSequence seq;
    EXPECT_CALL(ble, notifyValue(expected[0]));
    EXPECT_CALL(ble, notifyValue(expected[1]));

    midi.send(SysExMessage(sysex));
    // First two packets should be sent immediately
    Mock::VerifyAndClear(&ble);
    
    // Third packet is sent after flush
    EXPECT_CALL(ble, notifyValue(expected[2]));
    midi.flush();
    Mock::VerifyAndClear(&ble);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(BluetoothMIDIInterface, sendLongSysExFlushDestructor) {
    std::chrono::milliseconds timeout {100};
    auto midi = std::make_unique<BluetoothMIDI_Interface>();
    BLEMIDI &ble = midi->getBLEMIDI();
    midi->begin();
    midi->forceMinMTU(5 + 3);
    midi->setTimeout(timeout);

    std::vector<uint8_t> sysex = {
        0xF0, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0xF7,
    };

    std::vector<uint8_t> expected[] = {
        {
            0x80 | 0x01, // header + timestamp msb
            0x80 | 0x02, //          timestamp lsb
            0xF0,        // SysEx start
            0x10,        // data
            0x11,        // data
        },
        {
            0x80 | 0x01, // header + timestamp msb
            0x12,        // data
            0x13,        // data
            0x14,        // data
            0x15,        // data
        },
        {
            0x80 | 0x01, // header + timestamp msb
            0x16,        // data
            0x80 | 0x02, //          timestamp lsb
            0xF7,        // SysEx end
        },
    };

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(1) // For time stamp
        .WillRepeatedly(Return(timestamp(0x01, 0x02)));
    
    InSequence seq;
    EXPECT_CALL(ble, notifyValue(expected[0]));
    EXPECT_CALL(ble, notifyValue(expected[1]));

    midi->send(SysExMessage(sysex));
    // First two packets should be sent immediately
    Mock::VerifyAndClear(&ble);
    
    // Third packet is sent when the MIDI interface is destroyed
    EXPECT_CALL(ble, notifyValue(expected[2]));
    midi.reset();
    Mock::VerifyAndClear(&ble);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}
