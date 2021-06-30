#include <MIDI_Interfaces/MIDI_Callbacks.hpp>
#include <MIDI_Interfaces/SerialMIDI_Interface.hpp>
#include <TestStream.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

USING_CS_NAMESPACE;
using ::testing::Return;
using ::testing::Sequence;

using AH::ErrorException;

using u8vec = std::vector<uint8_t>;

TEST(StreamMIDI_Interface, send3B) {
    TestStream stream;
    StreamMIDI_Interface midi = stream;
    midi.sendChannelMessage(MIDIMessageType::NOTE_ON, CHANNEL_4, 0x55, 0x66);
    midi.sendNoteOn({0x55, CHANNEL_4}, 0x66);
    midi.sendNoteOff({0x55, CHANNEL_4}, 0x66);
    midi.sendControlChange({0x55, CHANNEL_4}, 0x66);
    midi.sendKeyPressure({0x55, CHANNEL_4}, 0x66);
    midi.sendPitchBend(CHANNEL_4, 0x3355);
    u8vec expected = {
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
    midi.sendChannelMessage(MIDIMessageType::PROGRAM_CHANGE, CHANNEL_4, 0x66);
    midi.sendProgramChange({CHANNEL_4}, 0x66);
    midi.sendProgramChange({0x66, CHANNEL_4});
    midi.sendChannelPressure(CHANNEL_4, 0x66);
    u8vec expected = {
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
    midi.send({sysex, CABLE_1});
    EXPECT_EQ(stream.sent, sysex);
}

TEST(StreamMIDI_Interface, SysExSend0B) {
    TestStream stream;
    StreamMIDI_Interface midi = stream;
    midi.send(SysExMessage{});
    EXPECT_TRUE(stream.sent.empty());
}

TEST(StreamMIDI_Interface, readRealTime) {
    TestStream stream;
    StreamMIDI_Interface midi = stream;
    stream.toRead.push(0xF8);
    RealTimeMessage expectedMsg = {MIDIMessageType::TIMING_CLOCK};
    EXPECT_EQ(midi.read(), MIDIReadEvent::REALTIME_MESSAGE);
    EXPECT_EQ(midi.getRealTimeMessage(), expectedMsg);
}

TEST(StreamMIDI_Interface, readNoteOn) {
    TestStream stream;
    StreamMIDI_Interface midi = stream;
    for (auto v : {0x93, 0x3C, 0x60})
        stream.toRead.push(v);
    EXPECT_EQ(midi.read(), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage expectedMsg = {0x93, 0x3C, 0x60};
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
    EXPECT_EQ(sysex.cable, CABLE_1);
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
    ChannelMessage expected = {0x94, 0x12, 0x34};
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
    SysExMessage expected = {sysex, 8};
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
    RealTimeMessage expected = {0xF8};
    EXPECT_CALL(callbacks, onRealTimeMessage(&midi, expected));
    midi.update();
}