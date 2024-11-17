#include <MIDI_Interfaces/BLEMIDI/ESP32Backend.hpp>
#include <MIDI_Interfaces/BluetoothMIDI_Interface.hpp>
#include <MIDI_Interfaces/GenericBLEMIDI_Interface.hpp>
#include <MIDI_Interfaces/MIDI_Callbacks.hpp>

using namespace cs;
using testing::Mock;

struct MockBLEBackend;

struct MockBLEImpl {
    MockBLEBackend *instance;
    void init(ESP32BLEBackend<MockBLEImpl> &instance, BLESettings);
    void notify(BLEConnectionHandle, BLECharacteristicHandle, BLEDataView data);
};

struct MockBLEBackend : public ESP32BLEBackend<MockBLEImpl> {
    MOCK_METHOD(void, notifyMIDIBLE, (const std::vector<uint8_t> &));
    using Parent = ESP32BLEBackend<MockBLEImpl>;
    using IncomingMIDIMessage = typename Parent::IncomingMIDIMessage;
    using Parent::handleData;
    friend MockBLEImpl;
};

void MockBLEImpl::init(ESP32BLEBackend<MockBLEImpl> &instance, BLESettings) {
    this->instance = &dynamic_cast<MockBLEBackend &>(instance);
    this->instance->handleConnect(BLEConnectionHandle {0});
    this->instance->handleSubscribe(BLEConnectionHandle {0},
                                    BLECharacteristicHandle {0}, true);
}

void MockBLEImpl::notify(BLEConnectionHandle, BLECharacteristicHandle,
                         BLEDataView data) {
    std::vector<uint8_t> d {data.data, data.data + data.length};
    this->instance->notifyMIDIBLE(d);
}

struct BluetoothMIDI_Interface : GenericBLEMIDI_Interface<MockBLEBackend> {
    void parse(const uint8_t *data, size_t length) {
        BLEDataView view {data, static_cast<uint16_t>(length)};
        auto data_gen = [view {view}]() mutable {
            return std::exchange(view, {});
        };
        backend.handleData(BLEConnectionHandle {0},
                           BLEDataGenerator {compat::in_place, data_gen},
                           BLEDataLifetime::ConsumeImmediately);
    }
};

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
    midi.update();

    std::vector<uint8_t> expectedSysExMessages = {};
    EXPECT_EQ(cb.sysExMessages, expectedSysExMessages);
    EXPECT_EQ(cb.sysExCounter, 0);

    std::vector<ChannelMessage> expectedChannelMessages = {
        {0x90, 0x3C, 0x7F},
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
    midi.update();

    std::vector<uint8_t> expectedSysExMessages = {};
    EXPECT_EQ(cb.sysExMessages, expectedSysExMessages);
    EXPECT_EQ(cb.sysExCounter, 0);

    std::vector<ChannelMessage> expectedChannelMessages = {
        {0x90, 0x3C, 0x7F},
        {0x80, 0x3D, 0x7E},
        {0xB1, 0x10, 0x40},
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
    midi.update();

    std::vector<uint8_t> expectedSysExMessages = {};
    EXPECT_EQ(cb.sysExMessages, expectedSysExMessages);
    EXPECT_EQ(cb.sysExCounter, 0);

    std::vector<ChannelMessage> expectedChannelMessages = {
        {0x90, 0x3C, 0x7F},
        {0x90, 0x3D, 0x7E},
        {0xB1, 0x10, 0x40},
    };
    EXPECT_EQ(cb.channelMessages, expectedChannelMessages);
}

TEST(BluetoothMIDIInterface,
     receiveMultipleChannelMessageRunningStatusTimestamp) {
    MockMIDI_Callbacks cb;

    BluetoothMIDI_Interface midi;
    midi.begin();
    midi.setCallbacks(&cb);

    uint8_t data[] = {
        0x84, 0x83, 0x90, // note on
        0x3C, 0x7F,       //
        0x3D, 0x7E,       //
        0x85, 0xB1,       // control change
        0x10, 0x40,       //
        0x11, 0x41,       //
        0x87,             // low timestamp byte
        0x12, 0x42,       //
        0x13, 0x43,       //
        0x89, 0xD9,       // channel pressure
        0x14,             //
        0x15,             //
    };
    midi.parse(data, sizeof(data));
    midi.update();

    std::vector<uint8_t> expectedSysExMessages = {};
    EXPECT_EQ(cb.sysExMessages, expectedSysExMessages);
    EXPECT_EQ(cb.sysExCounter, 0);

    std::vector<ChannelMessage> expectedChannelMessages = {
        {0x90, 0x3C, 0x7F}, {0x90, 0x3D, 0x7E}, {0xB1, 0x10, 0x40},
        {0xB1, 0x11, 0x41}, {0xB1, 0x12, 0x42}, {0xB1, 0x13, 0x43},
        {0xD9, 0x14, 0x00}, {0xD9, 0x15, 0x00},
    };
    EXPECT_EQ(cb.channelMessages, expectedChannelMessages);
}

// This test is basically the same as the previous one, but it also checks the
// timestamp values.
TEST(BluetoothMIDIInterface,
     receiveMultipleChannelMessageRunningStatusTimestampChecked) {
    BluetoothMIDI_Interface midi;
    midi.begin();

    uint8_t data[] = {
        0x84, 0x83, 0x90, // note on
        0x3C, 0x7F,       //
        0x3D, 0x7E,       //
        0x85, 0xB1,       // control change
        0x10, 0x40,       //
        0x11, 0x41,       //
        0x87,             // low timestamp byte
        0x12, 0x42,       //
        0x13, 0x43,       //
        0x89, 0xD9,       // channel pressure
        0x14,             //
        0x15,             //
        0x82,             // low timestamp byte (overflow)
        0x16,             //
    };
    midi.parse(data, sizeof(data));
    EXPECT_EQ(midi.read(), MIDIReadEvent::CHANNEL_MESSAGE);
    EXPECT_EQ(midi.getChannelMessage(), (ChannelMessage {0x90, 0x3C, 0x7F}));
    EXPECT_EQ(midi.getTimestamp(), 0x0203);
    EXPECT_EQ(midi.read(), MIDIReadEvent::CHANNEL_MESSAGE);
    EXPECT_EQ(midi.getChannelMessage(), (ChannelMessage {0x90, 0x3D, 0x7E}));
    EXPECT_EQ(midi.getTimestamp(), 0x0203);
    EXPECT_EQ(midi.read(), MIDIReadEvent::CHANNEL_MESSAGE);
    EXPECT_EQ(midi.getChannelMessage(), (ChannelMessage {0xB1, 0x10, 0x40}));
    EXPECT_EQ(midi.getTimestamp(), 0x0205);
    EXPECT_EQ(midi.read(), MIDIReadEvent::CHANNEL_MESSAGE);
    EXPECT_EQ(midi.getChannelMessage(), (ChannelMessage {0xB1, 0x11, 0x41}));
    EXPECT_EQ(midi.getTimestamp(), 0x0205);
    EXPECT_EQ(midi.read(), MIDIReadEvent::CHANNEL_MESSAGE);
    EXPECT_EQ(midi.getChannelMessage(), (ChannelMessage {0xB1, 0x12, 0x42}));
    EXPECT_EQ(midi.getTimestamp(), 0x0207);
    EXPECT_EQ(midi.read(), MIDIReadEvent::CHANNEL_MESSAGE);
    EXPECT_EQ(midi.getChannelMessage(), (ChannelMessage {0xB1, 0x13, 0x43}));
    EXPECT_EQ(midi.getTimestamp(), 0x0207);
    EXPECT_EQ(midi.read(), MIDIReadEvent::CHANNEL_MESSAGE);
    EXPECT_EQ(midi.getChannelMessage(), (ChannelMessage {0xD9, 0x14, 0x00}));
    EXPECT_EQ(midi.getTimestamp(), 0x0209);
    EXPECT_EQ(midi.read(), MIDIReadEvent::CHANNEL_MESSAGE);
    EXPECT_EQ(midi.getChannelMessage(), (ChannelMessage {0xD9, 0x15, 0x00}));
    EXPECT_EQ(midi.getTimestamp(), 0x0209);
    EXPECT_EQ(midi.read(), MIDIReadEvent::CHANNEL_MESSAGE);
    EXPECT_EQ(midi.getChannelMessage(), (ChannelMessage {0xD9, 0x16, 0x00}));
    EXPECT_EQ(midi.getTimestamp(), 0x0282);
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
    midi.update();

    std::vector<uint8_t> expectedSysExMessages = {};
    EXPECT_EQ(cb.sysExMessages, expectedSysExMessages);
    EXPECT_EQ(cb.sysExCounter, 0);

    std::vector<ChannelMessage> expectedChannelMessages = {
        {0x90, 0x3C, 0x7F},
        {0x90, 0x3D, 0x7E},
        {0xB1, 0x10, 0x40},
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
    midi.update();

    std::vector<uint8_t> expectedSysExMessages = {};
    EXPECT_EQ(cb.sysExMessages, expectedSysExMessages);
    EXPECT_EQ(cb.sysExCounter, 0);

    std::vector<ChannelMessage> expectedChannelMessages = {
        {0xD0, 0x3C, 0x00},
        {0xC0, 0x3D, 0x00},
        {0xB1, 0x10, 0x40},
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
    midi.update();

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
    midi.update();

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
    midi.update();

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
    midi.update();

    std::vector<uint8_t> expectedSysExMessages = {0xF0, 0x01, 0x02,
                                                  0x03, 0x04, 0xF7};
    EXPECT_EQ(cb.sysExMessages, expectedSysExMessages);
    EXPECT_EQ(cb.sysExCounter, 1);

    std::vector<ChannelMessage> expectedChannelMessages = {};
    EXPECT_EQ(cb.channelMessages, expectedChannelMessages);
}

static uint16_t timestamp(uint8_t msb, uint8_t lsb) {
    return (uint16_t(msb) << 7) | lsb;
}

TEST(BluetoothMIDIInterface, receiveSysCommonRunningStatusChannelMessage) {
    BluetoothMIDI_Interface midi;
    midi.begin();

    uint8_t data[] = {
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

    midi.parse(data, sizeof(data));
    EXPECT_EQ(midi.read(), MIDIReadEvent::CHANNEL_MESSAGE);
    EXPECT_EQ(midi.getChannelMessage(), ChannelMessage(0x92, 0x12, 0x34));
    EXPECT_EQ(midi.getTimestamp(), timestamp(0x01, 0x02));
    EXPECT_EQ(midi.read(), MIDIReadEvent::CHANNEL_MESSAGE);
    EXPECT_EQ(midi.getChannelMessage(), ChannelMessage(0x92, 0x56, 0x78));
    EXPECT_EQ(midi.getTimestamp(), timestamp(0x01, 0x02));
    EXPECT_EQ(midi.read(), MIDIReadEvent::SYSCOMMON_MESSAGE);
    EXPECT_EQ(midi.getSysCommonMessage(), SysCommonMessage(0xF2, 0x41, 0x74));
    EXPECT_EQ(midi.getTimestamp(), timestamp(0x01, 0x03));
    EXPECT_EQ(midi.read(), MIDIReadEvent::CHANNEL_MESSAGE);
    EXPECT_EQ(midi.getChannelMessage(), ChannelMessage(0x92, 0x11, 0x22));
    EXPECT_EQ(midi.getTimestamp(), timestamp(0x01, 0x04));
}

TEST(BluetoothMIDIInterface, emptyPacket) {
    MockMIDI_Callbacks cb;

    BluetoothMIDI_Interface midi;
    midi.begin();
    midi.setCallbacks(&cb);

    uint8_t data[] = {0x81}; // Just a header and nothing else
    midi.parse(data, sizeof(data));
    midi.update();

    EXPECT_TRUE(cb.sysExMessages.empty());
    EXPECT_EQ(cb.sysExCounter, 0);

    std::vector<ChannelMessage> expectedChannelMessages = {};
    EXPECT_EQ(cb.channelMessages, expectedChannelMessages);
}

TEST(BluetoothMIDIInterface, invalidPacket) {
    MockMIDI_Callbacks cb;

    BluetoothMIDI_Interface midi;
    midi.begin();
    midi.setCallbacks(&cb);

    uint8_t data[] = {0x12, 0x13, 0x14}; // First byte is not a header
    midi.parse(data, sizeof(data));
    midi.update();

    EXPECT_TRUE(cb.sysExMessages.empty());
    EXPECT_EQ(cb.sysExCounter, 0);

    std::vector<ChannelMessage> expectedChannelMessages = {};
    EXPECT_EQ(cb.channelMessages, expectedChannelMessages);
}

using namespace ::testing;

TEST(BluetoothMIDIInterface, sendOneNoteMessage) {
    BluetoothMIDI_Interface midi;
    midi.begin();

    std::vector<uint8_t> expected = {0x81, 0x82, 0x92, 0x12, 0x34};
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(1) // For time stamp
        .WillRepeatedly(Return(timestamp(0x01, 0x02)));
    EXPECT_CALL(midi.backend, notifyMIDIBLE(expected));

    midi.sendNoteOn({0x12, Channel_3}, 0x34);
    midi.sendNow();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(BluetoothMIDIInterface, sendTwoNoteMessages) {
    BluetoothMIDI_Interface midi;
    midi.begin();

    std::vector<uint8_t> expected = {
        0x81, 0x82, 0x92, 0x12, 0x34, 0x82, 0x99, 0x56, 0x78,
    };
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2) // For time stamp
        .WillRepeatedly(Return(timestamp(0x01, 0x02)));
    EXPECT_CALL(midi.backend, notifyMIDIBLE(expected));

    midi.sendNoteOn({0x12, Channel_3}, 0x34);
    midi.sendNoteOn({0x56, Channel_10}, 0x78);
    midi.sendNow();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(BluetoothMIDIInterface, sendTwoNoteMessagesRunningStatus) {
    BluetoothMIDI_Interface midi;
    midi.begin();

    std::vector<uint8_t> expected = {
        0x81, 0x82, 0x92, 0x12, 0x34, 0x56, 0x78,
    };
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2) // For time stamp
        .WillRepeatedly(Return(timestamp(0x01, 0x02)));
    EXPECT_CALL(midi.backend, notifyMIDIBLE(expected));

    midi.sendNoteOn({0x12, Channel_3}, 0x34);
    midi.sendNoteOn({0x56, Channel_3}, 0x78);
    midi.sendNow();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(BluetoothMIDIInterface, sendNoteMessageBufferFull) {
    BluetoothMIDI_Interface midi;
    midi.begin();
    midi.backend.forceMinMTU(7 + 3);

    std::vector<uint8_t> expected1 = {0x81, 0x82, 0x85, 0x56, 0x78};
    std::vector<uint8_t> expected2 = {0x81, 0x83, 0x86, 0x66, 0x79};
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2) // For time stamp
        .WillOnce(Return(timestamp(0x01, 0x02)))
        .WillOnce(Return(timestamp(0x01, 0x03)));
    testing::Sequence s;
    EXPECT_CALL(midi.backend, notifyMIDIBLE(expected1)).InSequence(s);
    EXPECT_CALL(midi.backend, notifyMIDIBLE(expected2)).InSequence(s);

    midi.sendNoteOff({0x56, Channel_6}, 0x78);
    midi.sendNoteOff({0x66, Channel_7}, 0x79);
    midi.sendNow();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(BluetoothMIDIInterface, sendOneProgramChangeMessage) {
    BluetoothMIDI_Interface midi;
    midi.begin();

    std::vector<uint8_t> expected = {0x81, 0x82, 0xC5, 0x78};
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(1) // For time stamp
        .WillRepeatedly(Return(timestamp(0x01, 0x02)));
    EXPECT_CALL(midi.backend, notifyMIDIBLE(expected));

    midi.sendProgramChange(Channel_6, 0x78);
    midi.sendNow();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(BluetoothMIDIInterface, sendProgramChangeMessageBufferFull) {
    BluetoothMIDI_Interface midi;
    midi.begin();
    midi.backend.forceMinMTU(6 + 3);

    std::vector<uint8_t> expected1 = {0x81, 0x82, 0xC5, 0x78};
    std::vector<uint8_t> expected2 = {0x81, 0x83, 0xC6, 0x79};
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2) // For time stamp
        .WillOnce(Return(timestamp(0x01, 0x02)))
        .WillOnce(Return(timestamp(0x01, 0x03)));
    testing::Sequence s;
    EXPECT_CALL(midi.backend, notifyMIDIBLE(expected1)).InSequence(s);
    EXPECT_CALL(midi.backend, notifyMIDIBLE(expected2)).InSequence(s);

    midi.sendProgramChange(Channel_6, 0x78);
    midi.sendProgramChange(Channel_7, 0x79);
    midi.sendNow();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(BluetoothMIDIInterface, sendRealTimeMessage) {
    BluetoothMIDI_Interface midi;
    midi.begin();

    std::vector<uint8_t> expected = {0x81, 0x82, 0xF8};
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(1) // For time stamp
        .WillOnce(Return(timestamp(0x01, 0x02)));
    EXPECT_CALL(midi.backend, notifyMIDIBLE(expected));

    midi.sendRealTime(0xF8);
    midi.sendNow();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(BluetoothMIDIInterface, sendRealTimeMessageBufferFull) {
    BluetoothMIDI_Interface midi;
    midi.begin();
    midi.backend.forceMinMTU(5 + 3);

    std::vector<uint8_t> expected1 = {0x81, 0x82, 0xF8, 0x83, 0xF9};
    std::vector<uint8_t> expected2 = {0x81, 0x84, 0xFA};
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(3) // For time stamp
        .WillOnce(Return(timestamp(0x01, 0x02)))
        .WillOnce(Return(timestamp(0x01, 0x03)))
        .WillOnce(Return(timestamp(0x01, 0x04)));
    testing::Sequence s;
    EXPECT_CALL(midi.backend, notifyMIDIBLE(expected1)).InSequence(s);
    EXPECT_CALL(midi.backend, notifyMIDIBLE(expected2)).InSequence(s);

    midi.sendRealTime(0xF8);
    midi.sendRealTime(0xF9);
    midi.sendRealTime(0xFA);
    midi.sendNow();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(BluetoothMIDIInterface, sendSysCommon) {
    BluetoothMIDI_Interface midi;
    midi.begin();

    std::vector<uint8_t> expected = {0x81, 0x82, 0xF2, 0x12, 0x34};
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(1) // For time stamp
        .WillOnce(Return(timestamp(0x01, 0x02)));
    EXPECT_CALL(midi.backend, notifyMIDIBLE(expected));

    midi.sendSysCommon(MIDIMessageType::SongPositionPointer, 0x12, 0x34);
    midi.sendNow();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(BluetoothMIDIInterface, sendSysCommonMessageBufferFull) {
    BluetoothMIDI_Interface midi;
    midi.begin();
    midi.backend.forceMinMTU(5 + 3);

    std::vector<uint8_t> expected1 = {0x81, 0x82, 0xF2, 0x12, 0x34};
    std::vector<uint8_t> expected2 = {0x81, 0x83, 0xF1, 0x56};
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2) // For time stamp
        .WillOnce(Return(timestamp(0x01, 0x02)))
        .WillOnce(Return(timestamp(0x01, 0x03)));
    testing::Sequence s;
    EXPECT_CALL(midi.backend, notifyMIDIBLE(expected1)).InSequence(s);
    EXPECT_CALL(midi.backend, notifyMIDIBLE(expected2)).InSequence(s);

    midi.sendSysCommon(MIDIMessageType::SongPositionPointer, 0x12, 0x34);
    midi.sendSysCommon(MIDIMessageType::MTCQuarterFrame, 0x56);
    midi.sendNow();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(BluetoothMIDIInterface, sendLongSysEx) {
    std::chrono::milliseconds timeout {100};
    BluetoothMIDI_Interface midi;
    midi.begin();
    midi.backend.forceMinMTU(5 + 3);
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
    EXPECT_CALL(midi.backend, notifyMIDIBLE(expected[0]));
    EXPECT_CALL(midi.backend, notifyMIDIBLE(expected[1]));

    midi.send(SysExMessage(sysex));
    // First two packets should be sent immediately
    Mock::VerifyAndClear(&midi);

    // Third packet is sent after the timeout
    std::this_thread::sleep_for(timeout * 0.9);
    EXPECT_CALL(midi.backend, notifyMIDIBLE(expected[2]));
    std::this_thread::sleep_for(timeout * 0.2);
    Mock::VerifyAndClear(&midi);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(BluetoothMIDIInterface, sendSysExBufferFullPacket1) {
    std::chrono::milliseconds timeout {100};
    BluetoothMIDI_Interface midi;
    midi.begin();
    midi.backend.forceMinMTU(5 + 3);
    midi.setTimeout(timeout);

    std::vector<uint8_t> sysex = {0xF0, 0xF7};

    std::vector<uint8_t> expected[] = {
        {
            0x80 | 0x01, // header + timestamp msb
            0x80 | 0x02, //          timestamp lsb
            0x91,        // status
            0x14,        // data 1
            0x15,        // data 2
        },
        {
            0x80 | 0x01, // header + timestamp msb
            0x80 | 0x03, //          timestamp lsb
            0xF0,        // SysEx start
            0x80 | 0x03, //          timestamp lsb
            0xF7,        // SysEx end
        },
    };

    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2) // For time stamp
        .WillOnce(Return(timestamp(0x01, 0x02)))
        .WillOnce(Return(timestamp(0x01, 0x03)));

    InSequence seq;
    EXPECT_CALL(midi.backend, notifyMIDIBLE(expected[0]));
    EXPECT_CALL(midi.backend, notifyMIDIBLE(expected[1]));

    midi.sendNoteOn({0x14, Channel_2}, 0x15);
    midi.send(SysExMessage(sysex));
    midi.sendNow();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(BluetoothMIDIInterface, sendLongSysExFlush) {
    std::chrono::milliseconds timeout {100};
    BluetoothMIDI_Interface midi;
    midi.begin();
    midi.backend.forceMinMTU(5 + 3);
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
    EXPECT_CALL(midi.backend, notifyMIDIBLE(expected[0]));
    EXPECT_CALL(midi.backend, notifyMIDIBLE(expected[1]));

    midi.send(SysExMessage(sysex));
    // First two packets should be sent immediately
    Mock::VerifyAndClear(&midi);

    // Third packet is sent after flush
    EXPECT_CALL(midi.backend, notifyMIDIBLE(expected[2]));
    midi.sendNow();
    Mock::VerifyAndClear(&midi);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}
