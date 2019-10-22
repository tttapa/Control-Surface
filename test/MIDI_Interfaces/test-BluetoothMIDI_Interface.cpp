#include <MIDI_Interfaces/BluetoothMIDI_Interface.hpp>

using namespace CS;

TEST(BluetoothMIDIInterface, initializeBegin) {
    BluetoothMIDI_Interface midi;
    BLEMIDI &ble = midi.getBLEMIDI();
    EXPECT_CALL(ble, begin(&midi, &midi));
    // EXPECT_CALL(ble, setServerCallbacks(&midi));
    // EXPECT_CALL(ble, setCharacteristicsCallbacks(&midi));
    midi.begin();
}

class MockMIDI_Callbacks : public MIDI_Callbacks {
  public:
    void onChannelMessage(Parsing_MIDI_Interface &midi) override {
        channelMessages.push_back(midi.getChannelMessage());
    }
    void onSysExMessage(Parsing_MIDI_Interface &midi) override {
        SysExMessage msg = midi.getSysExMessage();
        sysExMessages.insert(sysExMessages.end(), msg.data,
                             msg.data + msg.length);
        sysExCounter++;
    }
    void onRealtimeMessage(Parsing_MIDI_Interface &midi, uint8_t rtm) override {
        (void)midi;
        realtimeMessages.push_back(rtm);
    }

    std::vector<ChannelMessage> channelMessages;
    std::vector<uint8_t> sysExMessages;
    std::vector<uint8_t> realtimeMessages;
    size_t sysExCounter = 0;
};

TEST(BluetoothMIDIInterface, receiveChannelMessage) {
    MockMIDI_Callbacks cb;

    BluetoothMIDI_Interface midi;
    BLEMIDI &ble = midi.getBLEMIDI();
    EXPECT_CALL(ble, begin(&midi, &midi));
    // EXPECT_CALL(ble, setServerCallbacks(&midi));
    // EXPECT_CALL(ble, setCharacteristicsCallbacks(&midi));
    midi.begin();
    midi.setCallbacks(&cb);

    uint8_t data[] = {0x80, 0x80, 0x90, 0x3C, 0x7F};
    midi.parse(data, sizeof(data));

    std::vector<uint8_t> expectedSysExMessages = {};
    EXPECT_EQ(cb.sysExMessages, expectedSysExMessages);
    EXPECT_EQ(cb.sysExCounter, 0);

    std::vector<ChannelMessage> expectedChannelMessages = {{0x90, 0x3C, 0x7F}};
    EXPECT_EQ(cb.channelMessages, expectedChannelMessages);
}

TEST(BluetoothMIDIInterface, receiveMultipleChannelMessage) {
    MockMIDI_Callbacks cb;

    BluetoothMIDI_Interface midi;
    BLEMIDI &ble = midi.getBLEMIDI();
    EXPECT_CALL(ble, begin(&midi, &midi));
    // EXPECT_CALL(ble, setServerCallbacks(&midi));
    // EXPECT_CALL(ble, setCharacteristicsCallbacks(&midi));
    midi.begin();
    midi.setCallbacks(&cb);

    uint8_t data[] = {0x80, 0x80, 0x90, 0x3C, 0x7F, 0x80, 0x80,
                      0x3D, 0x7E, 0x80, 0xB1, 0x10, 0x40};
    midi.parse(data, sizeof(data));

    std::vector<uint8_t> expectedSysExMessages = {};
    EXPECT_EQ(cb.sysExMessages, expectedSysExMessages);
    EXPECT_EQ(cb.sysExCounter, 0);

    std::vector<ChannelMessage> expectedChannelMessages = {
        {0x90, 0x3C, 0x7F}, {0x80, 0x3D, 0x7E}, {0xB1, 0x10, 0x40}};
    EXPECT_EQ(cb.channelMessages, expectedChannelMessages);
}

TEST(BluetoothMIDIInterface, receiveMultipleChannelMessageRunningStatus) {
    MockMIDI_Callbacks cb;

    BluetoothMIDI_Interface midi;
    BLEMIDI &ble = midi.getBLEMIDI();
    EXPECT_CALL(ble, begin(&midi, &midi));
    // EXPECT_CALL(ble, setServerCallbacks(&midi));
    // EXPECT_CALL(ble, setCharacteristicsCallbacks(&midi));
    midi.begin();
    midi.setCallbacks(&cb);

    uint8_t data[] = {0x80, 0x80, 0x90, 0x3C, 0x7F, 0x3D,
                      0x7E, 0x80, 0xB1, 0x10, 0x40};
    midi.parse(data, sizeof(data));

    std::vector<uint8_t> expectedSysExMessages = {};
    EXPECT_EQ(cb.sysExMessages, expectedSysExMessages);
    EXPECT_EQ(cb.sysExCounter, 0);

    std::vector<ChannelMessage> expectedChannelMessages = {
        {0x90, 0x3C, 0x7F}, {0x90, 0x3D, 0x7E}, {0xB1, 0x10, 0x40}};
    EXPECT_EQ(cb.channelMessages, expectedChannelMessages);
}

TEST(BluetoothMIDIInterface,
     receiveMultipleChannelMessageRunningStatusRealTime) {
    MockMIDI_Callbacks cb;

    BluetoothMIDI_Interface midi;
    BLEMIDI &ble = midi.getBLEMIDI();
    EXPECT_CALL(ble, begin(&midi, &midi));
    // EXPECT_CALL(ble, setServerCallbacks(&midi));
    // EXPECT_CALL(ble, setCharacteristicsCallbacks(&midi));
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
        {0x90, 0x3C, 0x7F}, {0x90, 0x3D, 0x7E}, {0xB1, 0x10, 0x40}};
    EXPECT_EQ(cb.channelMessages, expectedChannelMessages);
}

TEST(BluetoothMIDIInterface, receiveMultipleTwoByteChannelMessage) {
    MockMIDI_Callbacks cb;

    BluetoothMIDI_Interface midi;
    BLEMIDI &ble = midi.getBLEMIDI();
    EXPECT_CALL(ble, begin(&midi, &midi));
    // EXPECT_CALL(ble, setServerCallbacks(&midi));
    // EXPECT_CALL(ble, setCharacteristicsCallbacks(&midi));
    midi.begin();
    midi.setCallbacks(&cb);

    uint8_t data[] = {0x80, 0x80, 0xD0, 0x3C, 0x80, 0xC0,
                      0x3D, 0x80, 0xB1, 0x10, 0x40};
    midi.parse(data, sizeof(data));

    std::vector<uint8_t> expectedSysExMessages = {};
    EXPECT_EQ(cb.sysExMessages, expectedSysExMessages);
    EXPECT_EQ(cb.sysExCounter, 0);

    std::vector<ChannelMessage> expectedChannelMessages = {
        {0xD0, 0x3C, 0x00}, {0xC0, 0x3D, 0x00}, {0xB1, 0x10, 0x40}};
    EXPECT_EQ(cb.channelMessages, expectedChannelMessages);
}

TEST(BluetoothMIDIInterface, receiveSysEx) {
    MockMIDI_Callbacks cb;

    BluetoothMIDI_Interface midi;
    BLEMIDI &ble = midi.getBLEMIDI();
    EXPECT_CALL(ble, begin(&midi, &midi));
    // EXPECT_CALL(ble, setServerCallbacks(&midi));
    // EXPECT_CALL(ble, setCharacteristicsCallbacks(&midi));
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
    BLEMIDI &ble = midi.getBLEMIDI();
    EXPECT_CALL(ble, begin(&midi, &midi));
    // EXPECT_CALL(ble, setServerCallbacks(&midi));
    // EXPECT_CALL(ble, setCharacteristicsCallbacks(&midi));
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
    BLEMIDI &ble = midi.getBLEMIDI();
    EXPECT_CALL(ble, begin(&midi, &midi));
    // EXPECT_CALL(ble, setServerCallbacks(&midi));
    // EXPECT_CALL(ble, setCharacteristicsCallbacks(&midi));
    midi.begin();
    midi.setCallbacks(&cb);

    uint8_t data1[] = {0x80, 0x80, 0xF0, 0x01, 0x02};
    uint8_t data2[] = {0x80, 0x03, 0x04, 0x80, 0xF7};
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
    BLEMIDI &ble = midi.getBLEMIDI();
    EXPECT_CALL(ble, begin(&midi, &midi));
    // EXPECT_CALL(ble, setServerCallbacks(&midi));
    // EXPECT_CALL(ble, setCharacteristicsCallbacks(&midi));
    midi.begin();
    midi.setCallbacks(&cb);

    uint8_t data[] = {0x80, 0x80, 0xF0, 0x01, 0x02,
                      0x80, 0xF8, 0x80, // this is a system real time message
                      0x03, 0x04, 0x80, 0xF7};
    midi.parse(data, sizeof(data));

    std::vector<uint8_t> expectedSysExMessages = {0xF0, 0x01, 0x02,
                                                  0x03, 0x04, 0xF7};
    EXPECT_EQ(cb.sysExMessages, expectedSysExMessages);
    EXPECT_EQ(cb.sysExCounter, 1);

    std::vector<ChannelMessage> expectedChannelMessages = {};
    EXPECT_EQ(cb.channelMessages, expectedChannelMessages);
}