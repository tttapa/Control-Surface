#include <MIDI_Interfaces/DebugMIDI_Interface.hpp>
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

TEST(StreamDebugMIDI_Interface, send3B) {
    TestStream stream;
    StreamDebugMIDI_Interface midi = stream;
    midi.send(NOTE_ON, 4, 0x55, 0x66);
    midi.sendOnCable(NOTE_ON, 4, 0x55, 0x66, 9);
    midi.sendNoteOn({0x55, CHANNEL_4, 9}, 0x66);
    midi.sendNoteOff({0x55, CHANNEL_4, 9}, 0x66);
    midi.sendCC({0x55, CHANNEL_4, 9}, 0x66);
    midi.sendKP({0x55, CHANNEL_4, 9}, 0x66);
    midi.sendPB(MIDICNChannel{CHANNEL_4, 9}, 0x3355);
    std::string expected =
        "Note On         \tChannel: 4\tData 1: 0x55\tData 2: 0x66\tCable: 0\r\n"
        "Note On         \tChannel: 4\tData 1: 0x55\tData 2: 0x66\tCable: 9\r\n"
        "Note On         \tChannel: 4\tData 1: 0x55\tData 2: 0x66\tCable: 9\r\n"
        "Note Off        \tChannel: 4\tData 1: 0x55\tData 2: 0x66\tCable: 9\r\n"
        "Control Change  \tChannel: 4\tData 1: 0x55\tData 2: 0x66\tCable: 9\r\n"
        "Key Pressure    \tChannel: 4\tData 1: 0x55\tData 2: 0x66\tCable: 9\r\n"
        "Pitch Bend      \tChannel: 4\tData 1: 0x55\tData 2: 0x66\tCable: 9\r\n"
        "";
    std::string sentStr(stream.sent.begin(), stream.sent.end());
    EXPECT_EQ(sentStr, expected);
}

TEST(StreamDebugMIDI_Interface, send2B) {
    TestStream stream;
    StreamDebugMIDI_Interface midi = stream;
    midi.send(PROGRAM_CHANGE, 4, 0x66);
    midi.sendOnCable(PROGRAM_CHANGE, 4, 0x66, 9);
    midi.sendPC({CHANNEL_4, 9}, 0x66);
    midi.sendPC({0x66, CHANNEL_4, 9});
    midi.sendCP({CHANNEL_4, 9}, 0x66);
    std::string expected =
        "Program Change  \tChannel: 4\tData 1: 0x66\tCable: 0\r\n"
        "Program Change  \tChannel: 4\tData 1: 0x66\tCable: 9\r\n"
        "Program Change  \tChannel: 4\tData 1: 0x66\tCable: 9\r\n"
        "Program Change  \tChannel: 4\tData 1: 0x66\tCable: 9\r\n"
        "Channel Pressure\tChannel: 4\tData 1: 0x66\tCable: 9\r\n";
    std::string sentStr(stream.sent.begin(), stream.sent.end());
    EXPECT_EQ(sentStr, expected);
}

TEST(StreamDebugMIDI_Interface, SysExSend8B) {
    TestStream stream;
    StreamDebugMIDI_Interface midi = stream;
    Sequence seq;
    u8vec sysex = {0xF0, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0xF7};
    midi.send({sysex.data(), sysex.size(), 10});
    std::string expected =
        "SysEx           \tF0 11 22 33 44 55 66 F7 \tCable: 10\r\n";
    std::string sentStr(stream.sent.begin(), stream.sent.end());
    EXPECT_EQ(sentStr, expected);
}

TEST(StreamDebugMIDI_Interface, SysExSend0B) {
    TestStream stream;
    StreamDebugMIDI_Interface midi = stream;
    midi.send(SysExMessage{});
    EXPECT_TRUE(stream.sent.empty());
}

TEST(StreamDebugMIDI_Interface, SysExSend1B) {
    TestStream stream;
    StreamDebugMIDI_Interface midi = stream;
    uint8_t sysex[] = {0xF0};
    try {
        midi.send(sysex);
        FAIL();
    } catch (ErrorException &e) {
        EXPECT_EQ(e.getErrorCode(), 0x7F7F);
    }
}

TEST(StreamDebugMIDI_Interface, readRealTime) {
    TestStream stream;
    StreamDebugMIDI_Interface midi = stream;
    for (auto v : "F8 ")
        stream.toRead.push(v);
    EXPECT_EQ(midi.read(), TIMING_CLOCK_MESSAGE);
    EXPECT_EQ(midi.getCN(), 0);
}

TEST(StreamDebugMIDI_Interface, readNoteOn) {
    TestStream stream;
    StreamDebugMIDI_Interface midi = stream;
    for (auto v : "93 3C 60 ")
        stream.toRead.push(v);
    EXPECT_EQ(midi.read(), CHANNEL_MESSAGE);
    ChannelMessage expectedMsg = {0x93, 0x3C, 0x60, 0x00};
    EXPECT_EQ(midi.getChannelMessage(), expectedMsg);
}

TEST(StreamDebugMIDI_Interface, readNoteOnSingleChar) {
    TestStream stream;
    StreamDebugMIDI_Interface midi = stream;
    for (auto v : "93 C 60 ")
        stream.toRead.push(v);
    EXPECT_EQ(midi.read(), CHANNEL_MESSAGE);
    ChannelMessage expectedMsg = {0x93, 0x0C, 0x60, 0x00};
    EXPECT_EQ(midi.getChannelMessage(), expectedMsg);
}

using SysExVector = std::vector<uint8_t>;

TEST(StreamDebugMIDI_Interface, readSysEx) {
    TestStream stream;
    StreamDebugMIDI_Interface midi = stream;
    for (auto v : "F0 55 66 77 11 22 33 F7 ")
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