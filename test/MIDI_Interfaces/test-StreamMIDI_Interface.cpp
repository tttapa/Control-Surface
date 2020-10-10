#include <MIDI_Interfaces/MIDI_Callbacks.hpp>
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
    midi.send(MIDIMessageType::NOTE_ON, CHANNEL_4, 0x55, 0x66);
    midi.sendOnCable(MIDIMessageType::NOTE_ON, CHANNEL_4, 0x55, 0x66, CABLE_1);
    midi.sendNoteOn({0x55, CHANNEL_4}, 0x66);
    midi.sendNoteOff({0x55, CHANNEL_4}, 0x66);
    midi.sendCC({0x55, CHANNEL_4}, 0x66);
    midi.sendKP({0x55, CHANNEL_4}, 0x66);
    midi.sendPB(CHANNEL_4, 0x3355);
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
    midi.send(MIDIMessageType::PROGRAM_CHANGE, CHANNEL_4, 0x66);
    midi.sendOnCable(MIDIMessageType::PROGRAM_CHANGE, CHANNEL_4, 0x66, CABLE_1);
    midi.sendPC({CHANNEL_4}, 0x66);
    midi.sendPC({0x66, CHANNEL_4});
    midi.sendCP(CHANNEL_4, 0x66);
    u8vec expected = {
        0xC3, 0x66, //
        0xC3, 0x66, //
        0xC3, 0x66, //
        0xC3, 0x66, //
        0xD3, 0x66, //
    };
    EXPECT_EQ(stream.sent, expected);
}

TEST(StreamMIDI_Interface, sendRealTime) {
    TestStream stream;
    StreamMIDI_Interface midi = stream;
    midi.sendRealTime(MIDIMessageType::TIMING_CLOCK);
    u8vec expected = {0xF8};
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
        midi.sendSysEx(sysex);
        FAIL();
    } catch (ErrorException &e) {
        EXPECT_EQ(e.getErrorCode(), 0x7F7F);
    }
}

TEST(StreamMIDI_Interface, readRealTime) {
    TestStream stream;
    StreamMIDI_Interface midi = stream;
    stream.toRead.push(0xF8);
    RealTimeMessage expectedMsg = {MIDIMessageType::TIMING_CLOCK, 0};
    EXPECT_EQ(midi.read(), MIDIReadEvent::REALTIME_MESSAGE);
    EXPECT_EQ(midi.getRealTimeMessage(), expectedMsg);
}

TEST(StreamMIDI_Interface, readNoteOn) {
    TestStream stream;
    StreamMIDI_Interface midi = stream;
    for (auto v : {0x93, 0x3C, 0x60})
        stream.toRead.push(v);
    EXPECT_EQ(midi.read(), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage expectedMsg = {0x93, 0x3C, 0x60, 0x00};
    EXPECT_EQ(midi.getChannelMessage(), expectedMsg);
}

using SysExVector = std::vector<uint8_t>;

TEST(StreamMIDI_Interface, readSysEx) {
    TestStream stream;
    StreamMIDI_Interface midi = stream;
    for (auto v : {0xF0, 0x55, 0x66, 0x77, 0x11, 0x22, 0x33, 0xF7})
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
    EXPECT_EQ(sysex.cable, 0);
}

TEST(StreamMIDI_Interface, readNoteUpdate) {
    class MockMIDI_Callbacks : public MIDI_Callbacks {
      public:
        MOCK_METHOD(void, onChannelMessage, (MIDI_Interface *, ChannelMessage),
                    ());
        MOCK_METHOD(void, onSysExMessage, (MIDI_Interface &, SysExMessage),
                    (override));
        MOCK_METHOD(void, onRealTimeMessage,
                    (MIDI_Interface &, RealTimeMessage), (override));
        void onChannelMessage(MIDI_Interface &midi, ChannelMessage m) override {
            onChannelMessage(&midi, m);
        }
    };
    MockMIDI_Callbacks callbacks;
    TestStream stream;
    StreamMIDI_Interface midi = stream;
    midi.setCallbacks(callbacks);
    midi.begin();
    for (auto v : {0x94, 0x12, 0x34})
        stream.toRead.push(v);
    ChannelMessage expected = {0x94, 0x12, 0x34, 0};
    EXPECT_CALL(callbacks, onChannelMessage(&midi, expected));
    midi.update();
}

TEST(StreamMIDI_Interface, readSysExUpdate) {
    class MockMIDI_Callbacks : public MIDI_Callbacks {
      public:
        MOCK_METHOD(void, onChannelMessage, (MIDI_Interface &, ChannelMessage),
                    (override));
        MOCK_METHOD(void, onSysExMessage, (MIDI_Interface *, SysExMessage), ());
        MOCK_METHOD(void, onRealTimeMessage,
                    (MIDI_Interface &, RealTimeMessage), (override));
        void onSysExMessage(MIDI_Interface &midi, SysExMessage m) override {
            onSysExMessage(&midi, m);
        }
    };
    MockMIDI_Callbacks callbacks;
    TestStream stream;
    StreamMIDI_Interface midi = stream;
    midi.setCallbacks(callbacks);
    midi.begin();
    uint8_t sysex[] = {0xF0, 0x55, 0x66, 0x77, 0x11, 0x22, 0x33, 0xF7};
    for (auto v : sysex)
        stream.toRead.push(v);
    SysExMessage expected = {sysex, 8, 0};
    EXPECT_CALL(callbacks, onSysExMessage(&midi, expected));
    midi.update();
}

TEST(StreamMIDI_Interface, readRealTimeUpdate) {
    class MockMIDI_Callbacks : public MIDI_Callbacks {
      public:
        MOCK_METHOD(void, onChannelMessage, (MIDI_Interface &, ChannelMessage),
                    (override));
        MOCK_METHOD(void, onSysExMessage, (MIDI_Interface &, SysExMessage),
                    (override));
        MOCK_METHOD(void, onRealTimeMessage,
                    (MIDI_Interface *, RealTimeMessage), ());
        void onRealTimeMessage(MIDI_Interface &midi,
                               RealTimeMessage m) override {
            onRealTimeMessage(&midi, m);
        }
    };
    MockMIDI_Callbacks callbacks;
    TestStream stream;
    StreamMIDI_Interface midi = stream;
    midi.setCallbacks(callbacks);
    midi.begin();
    stream.toRead.push(0xF8);
    RealTimeMessage expected = {0xF8, 0};
    EXPECT_CALL(callbacks, onRealTimeMessage(&midi, expected));
    midi.update();
}