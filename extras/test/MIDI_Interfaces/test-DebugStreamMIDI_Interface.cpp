#include <MIDI_Interfaces/DebugMIDI_Interface.hpp>
#include <TestStream.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

USING_CS_NAMESPACE;
using ::testing::Return;
using ::testing::Sequence;

using AH::ErrorException;

using u8vec = std::vector<uint8_t>;

using MMT = MIDIMessageType;

TEST(StreamDebugMIDI_Interface, send3B) {
    TestStream stream;
    StreamDebugMIDI_Interface midi = stream;
    midi.sendChannelMessage(MMT::NoteOn, Channel_4, 0x55, 0x66, Cable_5);
    midi.sendNoteOn({0x55, Channel_4, Cable_9}, 0x66);
    midi.sendNoteOff({0x55, Channel_4, Cable_9}, 0x66);
    midi.sendControlChange({0x55, Channel_4, Cable_9}, 0x66);
    midi.sendKeyPressure({0x55, Channel_4, Cable_9}, 0x66);
    midi.sendPitchBend({Channel_4, Cable_9}, 0x3355);
    std::string expected = "Note On          Channel: 4\tData 1: 0x55\tData "
                           "2: 0x66\tCable: 5\r\n"
                           "Note On          Channel: 4\tData 1: 0x55\tData "
                           "2: 0x66\tCable: 9\r\n"
                           "Note Off         Channel: 4\tData 1: 0x55\tData "
                           "2: 0x66\tCable: 9\r\n"
                           "Control Change   Channel: 4\tData 1: 0x55\tData "
                           "2: 0x66\tCable: 9\r\n"
                           "Key Pressure     Channel: 4\tData 1: 0x55\tData "
                           "2: 0x66\tCable: 9\r\n"
                           "Pitch Bend       Channel: 4\tData 1: 0x55\tData "
                           "2: 0x66 (13141)\tCable: 9\r\n"
                           "";
    std::string sentStr(stream.sent.begin(), stream.sent.end());
    EXPECT_EQ(sentStr, expected);
}

TEST(StreamDebugMIDI_Interface, send2B) {
    TestStream stream;
    StreamDebugMIDI_Interface midi = stream;
    midi.sendChannelMessage(MMT::ProgramChange, Channel_4, 0x66, Cable_5);
    midi.sendProgramChange({Channel_4, Cable_9}, 0x66);
    midi.sendProgramChange({0x66, Channel_4, Cable_9});
    midi.sendChannelPressure({Channel_4, Cable_9}, 0x66);
    std::string expected =
        "Program Change   Channel: 4\tData 1: 0x66\tCable: 5\r\n"
        "Program Change   Channel: 4\tData 1: 0x66\tCable: 9\r\n"
        "Program Change   Channel: 4\tData 1: 0x66\tCable: 9\r\n"
        "Channel Pressure Channel: 4\tData 1: 0x66\tCable: 9\r\n";
    std::string sentStr(stream.sent.begin(), stream.sent.end());
    EXPECT_EQ(sentStr, expected);
}

TEST(StreamDebugMIDI_Interface, SysExSend8B) {
    TestStream stream;
    StreamDebugMIDI_Interface midi = stream;
    Sequence seq;
    u8vec sysex = {0xF0, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0xF7};
    midi.send({sysex, Cable_10});
    std::string expected =
        "System Exclusive [8]\tF0 11 22 33 44 55 66 F7\tCable: 10\r\n";
    std::string sentStr(stream.sent.begin(), stream.sent.end());
    EXPECT_EQ(sentStr, expected);
}

TEST(StreamDebugMIDI_Interface, SysCommonMTCQF) {
    TestStream stream;
    StreamDebugMIDI_Interface midi = stream;
    Sequence seq;
    SysCommonMessage msg = {MMT::MTCQuarterFrame, 0x47, Cable_10};
    midi.send(msg);
    std::string expected = "System Common    MTCQuarterFrame\tData 1: "
                           "0x47\tCable: 10\r\n";
    std::string sentStr(stream.sent.begin(), stream.sent.end());
    EXPECT_EQ(sentStr, expected);
}

TEST(StreamDebugMIDI_Interface, SysCommonSPP) {
    TestStream stream;
    StreamDebugMIDI_Interface midi = stream;
    Sequence seq;
    SysCommonMessage msg = {MMT::SongPositionPointer, 0x12, 0x34, Cable_10};
    midi.send(msg);
    std::string expected = "System Common    SongPositionPointer\tData 1: "
                           "0x12\tData 2: 0x34 (6674)\tCable: 10\r\n";
    std::string sentStr(stream.sent.begin(), stream.sent.end());
    EXPECT_EQ(sentStr, expected);
}

TEST(StreamDebugMIDI_Interface, SysCommonSS) {
    TestStream stream;
    StreamDebugMIDI_Interface midi = stream;
    Sequence seq;
    SysCommonMessage msg = {MMT::SongSelect, 0x51, Cable_10};
    midi.send(msg);
    std::string expected = "System Common    SongSelect\tData 1: "
                           "0x51\tCable: 10\r\n";
    std::string sentStr(stream.sent.begin(), stream.sent.end());
    EXPECT_EQ(sentStr, expected);
}

TEST(StreamDebugMIDI_Interface, SysCommonTR) {
    TestStream stream;
    StreamDebugMIDI_Interface midi = stream;
    Sequence seq;
    SysCommonMessage msg = {MMT::TuneRequest, Cable_10};
    midi.send(msg);
    std::string expected = "System Common    TuneRequest\tCable: 10\r\n";
    std::string sentStr(stream.sent.begin(), stream.sent.end());
    EXPECT_EQ(sentStr, expected);
}

TEST(StreamDebugMIDI_Interface, RealTimeSend) {
    TestStream stream;
    StreamDebugMIDI_Interface midi = stream;
    Sequence seq;
    midi.send({0xF8, Cable_10});
    std::string expected = "Real-Time        TimingClock\tCable: 10\r\n";
    std::string sentStr(stream.sent.begin(), stream.sent.end());
    EXPECT_EQ(sentStr, expected);
}

TEST(StreamDebugMIDI_Interface, RealTimeSendPrefix) {
    TestStream stream;
    StreamDebugMIDI_Interface midi = stream;
    midi.setPrefix("<prefix>");
    Sequence seq;
    midi.send({0xF8, Cable_10});
    std::string expected =
        "<prefix> Real-Time        TimingClock\tCable: 10\r\n";
    std::string sentStr(stream.sent.begin(), stream.sent.end());
    EXPECT_EQ(sentStr, expected);
}

TEST(StreamDebugMIDI_Interface, SysExSend0B) {
    TestStream stream;
    StreamDebugMIDI_Interface midi = stream;
    midi.send(SysExMessage {});
    EXPECT_TRUE(stream.sent.empty());
}

TEST(StreamDebugMIDI_Interface, readRealTime) {
    TestStream stream;
    StreamDebugMIDI_Interface midi = stream;
    for (auto v : "F8 ")
        stream.toRead.push(v);
    RealTimeMessage expectedMsg = {MMT::TimingClock};
    EXPECT_EQ(midi.read(), MIDIReadEvent::REALTIME_MESSAGE);
    EXPECT_EQ(midi.getRealTimeMessage(), expectedMsg);
}

TEST(StreamDebugMIDI_Interface, readNoteOn) {
    TestStream stream;
    StreamDebugMIDI_Interface midi = stream;
    for (auto v : "93 3C 60 ")
        stream.toRead.push(v);
    EXPECT_EQ(midi.read(), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage expectedMsg = {0x93, 0x3C, 0x60};
    EXPECT_EQ(midi.getChannelMessage(), expectedMsg);
}

TEST(StreamDebugMIDI_Interface, readNoteOnSingleDigit) {
    TestStream stream;
    StreamDebugMIDI_Interface midi = stream;
    for (auto v : "93 3 60 ")
        stream.toRead.push(v);
    EXPECT_EQ(midi.read(), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage expectedMsg = {0x93, 0x3, 0x60};
    EXPECT_EQ(midi.getChannelMessage(), expectedMsg);
}

TEST(StreamDebugMIDI_Interface, readNoteOnNoSpaces) {
    TestStream stream;
    StreamDebugMIDI_Interface midi = stream;
    for (auto v : "933C60")
        stream.toRead.push(v);
    EXPECT_EQ(midi.read(), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage expectedMsg = {0x93, 0x3C, 0x60};
    EXPECT_EQ(midi.getChannelMessage(), expectedMsg);
}

TEST(StreamDebugMIDI_Interface, readNoteOnSingleChar) {
    TestStream stream;
    StreamDebugMIDI_Interface midi = stream;
    for (auto v : "93 C 60 ")
        stream.toRead.push(v);
    EXPECT_EQ(midi.read(), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage expectedMsg = {0x93, 0x0C, 0x60};
    EXPECT_EQ(midi.getChannelMessage(), expectedMsg);
}

using SysExVector = std::vector<uint8_t>;

TEST(StreamDebugMIDI_Interface, readSysEx) {
    TestStream stream;
    StreamDebugMIDI_Interface midi = stream;
    for (auto v : "F0 55 66 77 11 22 33 F7 ")
        stream.toRead.push(v);
    EXPECT_EQ(midi.read(), MIDIReadEvent::SYSEX_MESSAGE);
    SysExMessage sysex = midi.getSysExMessage();
    const SysExVector result = {
        sysex.data,
        sysex.data + sysex.length,
    };
    const SysExVector expected = {
        0xF0, 0x55, 0x66, 0x77, 0x11, 0x22, 0x33, 0xF7,
    };
    EXPECT_EQ(result, expected);
    EXPECT_EQ(sysex.cable, Cable_1);
}