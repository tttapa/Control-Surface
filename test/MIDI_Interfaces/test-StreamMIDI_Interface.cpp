#include <MIDI_Interfaces/SerialMIDI_Interface.hpp>
#include <gmock-wrapper.h>
#include <gtest-wrapper.h>
#include <queue>

USING_CS_NAMESPACE;
using ::testing::Return;
using ::testing::Sequence;

using AH::ErrorException;

class TestStream : public Stream {
  public:
    size_t write(uint8_t data) override {
        sent.push_back(data);
        return 1;
    }
    int peek() override { return toRead.empty() ? -1 : toRead.front(); }
    int read() override {
        int retval = peek();
        if (!toRead.empty())
            toRead.pop();
        return retval;
    }
    int available() override { return toRead.size(); }

    std::vector<uint8_t> sent;
    std::queue<uint8_t> toRead;
};

using u8vec = std::vector<uint8_t>;

TEST(StreamMIDI_Interface, send3B) {
    TestStream stream;
    StreamMIDI_Interface midi = stream;
    midi.send(NOTE_ON, 4, 0x55, 0x66);
    midi.sendOnCable(NOTE_ON, 4, 0x55, 0x66, 9);
    midi.sendNoteOn({0x55, CHANNEL_4, 8}, 0x66);
    midi.sendNoteOff({0x55, CHANNEL_4, 8}, 0x66);
    midi.sendCC({0x55, CHANNEL_4, 8}, 0x66);
    midi.sendKP({0x55, CHANNEL_4, 8}, 0x66);
    midi.sendPB(MIDICNChannel{CHANNEL_4, 8}, 0x3355);
    u8vec expected = {
        0x93, 0x55, 0x66, //
        0x93, 0x55, 0x66, //
        0x93, 0x55, 0x66, //
        0x83, 0x55, 0x66, //
        0xB3, 0x55, 0x66, //
        0xA3, 0x55, 0x66, //
        0xE3, 0x55, 0x66, //
    };
    EXPECT_EQ(stream.sent, expected);
}

TEST(StreamMIDI_Interface, send2B) {
    TestStream stream;
    StreamMIDI_Interface midi = stream;
    midi.send(PROGRAM_CHANGE, 4, 0x66);
    midi.sendOnCable(PROGRAM_CHANGE, 4, 0x66, 9);
    midi.sendPC({CHANNEL_4, 8}, 0x66);
    midi.sendPC({0x66, CHANNEL_4, 8});
    midi.sendCP({CHANNEL_4, 8}, 0x66);
    u8vec expected = {
        0xC3, 0x66, //
        0xC3, 0x66, //
        0xC3, 0x66, //
        0xC3, 0x66, //
        0xD3, 0x66, //
    };
    EXPECT_EQ(stream.sent, expected);
}

TEST(StreamMIDI_Interface, SysExSend8B) {
    TestStream stream;
    StreamMIDI_Interface midi = stream;
    Sequence seq;
    u8vec sysex = {0xF0, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0xF7};
    midi.send({sysex.data(), sysex.size(), 10});
    EXPECT_EQ(stream.sent, sysex);
}

TEST(StreamMIDI_Interface, SysExSend0B) {
    TestStream stream;
    StreamMIDI_Interface midi = stream;
    midi.send(SysExMessage{});
    EXPECT_TRUE(stream.sent.empty());
}

TEST(StreamMIDI_Interface, SysExSend1B) {
    TestStream stream;
    StreamMIDI_Interface midi = stream;
    uint8_t sysex[] = {0xF0};
    try {
        midi.send(sysex);
        FAIL();
    } catch (ErrorException &e) {
        EXPECT_EQ(e.getErrorCode(), 0x7F7F);
    }
}

TEST(StreamMIDI_Interface, readRealTime) {
    TestStream stream;
    StreamMIDI_Interface midi = stream;
    stream.toRead.push(0xF8);
    EXPECT_EQ(midi.read(), TIMING_CLOCK_MESSAGE);
    EXPECT_EQ(midi.getCN(), 0);
}

TEST(StreamMIDI_Interface, readNoteOn) {
    TestStream stream;
    StreamMIDI_Interface midi = stream;
    for (auto v : {0x93, 0x3C, 0x60})
        stream.toRead.push(v);
    EXPECT_EQ(midi.read(), CHANNEL_MESSAGE);
    ChannelMessage expectedMsg = {0x93, 0x3C, 0x60, 0x00};
    EXPECT_EQ(midi.getChannelMessage(), expectedMsg);
}

using SysExVector = std::vector<uint8_t>;

TEST(StreamMIDI_Interface, readSysEx) {
    TestStream stream;
    StreamMIDI_Interface midi = stream;
    for (auto v : {0xF0, 0x55, 0x66, 0x77, 0x11, 0x22, 0x33, 0xF7, 0x00})
        stream.toRead.push(v);
    EXPECT_EQ(midi.read(), SYSEX_MESSAGE);
    SysExMessage sysex = midi.getSysExMessage();
    const SysExVector result = {
        sysex.data,
        sysex.data + sysex.length,
    };
    const SysExVector expected = {
        0xF0, 0x55, 0x66, 0x77, 0x11, 0x22, 0x33, 0xF7,
    };
    EXPECT_EQ(result, expected);
    EXPECT_EQ(sysex.CN, 0);
}

TEST(StreamMIDI_Interface, readSysExUpdate) {
    class MockMIDI_Callbacks : public MIDI_Callbacks {
      public:
        W_SUGGEST_OVERRIDE_OFF
        MOCK_METHOD1(onChannelMessage, void(Parsing_MIDI_Interface &));
        MOCK_METHOD1(onSysExMessage, void(Parsing_MIDI_Interface &));
        MOCK_METHOD2(onRealtimeMessage,
                     void(Parsing_MIDI_Interface &, uint8_t));
        W_SUGGEST_OVERRIDE_ON
    } callbacks;
    TestStream stream;
    StreamMIDI_Interface midi = stream;
    midi.setCallbacks(callbacks);
    midi.begin();
    for (auto v : {0xF0, 0x55, 0x66, 0x77, 0x11, 0x22, 0x33, 0xF7, 0x00})
        stream.toRead.push(v);
    EXPECT_CALL(callbacks, onSysExMessage(testing::_));
    midi.update();
    SysExMessage sysex = midi.getSysExMessage();
    const SysExVector result = {
        sysex.data,
        sysex.data + sysex.length,
    };
    const SysExVector expected = {
        0xF0, 0x55, 0x66, 0x77, 0x11, 0x22, 0x33, 0xF7,
    };
    EXPECT_EQ(result, expected);
    EXPECT_EQ(sysex.CN, 0);
}