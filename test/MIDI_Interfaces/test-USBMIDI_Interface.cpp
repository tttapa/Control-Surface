#include <MIDI_Interfaces/USBMIDI_Interface.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

USING_CS_NAMESPACE;
using ::testing::Return;
using ::testing::Sequence;
using ::testing::StrictMock;

using AH::ErrorException;

TEST(USBMIDI_Interface, send3B) {
    StrictMock<USBMIDI_Interface> midi;
    EXPECT_CALL(midi.backend, write(0x89, 0x93, 0x55, 0x66));
    midi.sendNoteOn({0x55, CHANNEL_4, CABLE_9}, 0x66);
}

TEST(USBMIDI_Interface, send2B) {
    StrictMock<USBMIDI_Interface> midi;
    EXPECT_CALL(midi.backend, write(0x8C, 0xC3, 0x66, 0x00));
    midi.sendProgramChange({CHANNEL_4, CABLE_9}, 0x66);
}

TEST(USBMIDI_Interface, RealTime) {
    StrictMock<USBMIDI_Interface> midi;
    Sequence seq;
    EXPECT_CALL(midi.backend, write(0x8F, 0xF8, 0x00, 0x00)).InSequence(seq);
    midi.sendRealTime(MIDIMessageType::TIMING_CLOCK, CABLE_9);
}

TEST(USBMIDI_Interface, SysExSend3B) {
    StrictMock<USBMIDI_Interface> midi;
    Sequence seq;
    EXPECT_CALL(midi.backend, write(0x87, 0xF0, 0x55, 0xF7)).InSequence(seq);
    uint8_t sysex[] = {0xF0, 0x55, 0xF7};
    midi.sendSysEx(sysex, CABLE_9);
}

TEST(USBMIDI_Interface, SysExSend4B) {
    StrictMock<USBMIDI_Interface> midi;
    Sequence seq;
    EXPECT_CALL(midi.backend, write(0x94, 0xF0, 0x55, 0x66)).InSequence(seq);
    EXPECT_CALL(midi.backend, write(0x95, 0xF7, 0x00, 0x00)).InSequence(seq);
    uint8_t sysex[] = {0xF0, 0x55, 0x66, 0xF7};
    midi.sendSysEx(sysex, CABLE_10);
}

TEST(USBMIDI_Interface, SysExSend5B) {
    StrictMock<USBMIDI_Interface> midi;
    Sequence seq;
    EXPECT_CALL(midi.backend, write(0x94, 0xF0, 0x55, 0x66)).InSequence(seq);
    EXPECT_CALL(midi.backend, write(0x96, 0x77, 0xF7, 0x00)).InSequence(seq);
    uint8_t sysex[] = {0xF0, 0x55, 0x66, 0x77, 0xF7};
    midi.sendSysEx(sysex, CABLE_10);
}

TEST(USBMIDI_Interface, SysExSend6B) {
    StrictMock<USBMIDI_Interface> midi;
    Sequence seq;
    EXPECT_CALL(midi.backend, write(0x94, 0xF0, 0x55, 0x66)).InSequence(seq);
    EXPECT_CALL(midi.backend, write(0x97, 0x77, 0x11, 0xF7)).InSequence(seq);
    uint8_t sysex[] = {0xF0, 0x55, 0x66, 0x77, 0x11, 0xF7};
    midi.sendSysEx(sysex, CABLE_10);
}

TEST(USBMIDI_Interface, SysExSend7B) {
    StrictMock<USBMIDI_Interface> midi;
    Sequence seq;
    EXPECT_CALL(midi.backend, write(0x94, 0xF0, 0x55, 0x66)).InSequence(seq);
    EXPECT_CALL(midi.backend, write(0x94, 0x77, 0x11, 0x22)).InSequence(seq);
    EXPECT_CALL(midi.backend, write(0x95, 0xF7, 0x00, 0x00)).InSequence(seq);
    uint8_t sysex[] = {0xF0, 0x55, 0x66, 0x77, 0x11, 0x22, 0xF7};
    midi.sendSysEx(sysex, CABLE_10);
}

TEST(USBMIDI_Interface, SysExSend8B) {
    StrictMock<USBMIDI_Interface> midi;
    Sequence seq;
    EXPECT_CALL(midi.backend, write(0x94, 0xF0, 0x55, 0x66)).InSequence(seq);
    EXPECT_CALL(midi.backend, write(0x94, 0x77, 0x11, 0x22)).InSequence(seq);
    EXPECT_CALL(midi.backend, write(0x96, 0x33, 0xF7, 0x00)).InSequence(seq);
    uint8_t sysex[] = {0xF0, 0x55, 0x66, 0x77, 0x11, 0x22, 0x33, 0xF7};
    midi.sendSysEx(sysex, CABLE_10);
}

TEST(USBMIDI_Interface, SysExSend9B) {
    StrictMock<USBMIDI_Interface> midi;
    Sequence seq;
    EXPECT_CALL(midi.backend, write(0x94, 0xF0, 0x55, 0x66)).InSequence(seq);
    EXPECT_CALL(midi.backend, write(0x94, 0x77, 0x11, 0x22)).InSequence(seq);
    EXPECT_CALL(midi.backend, write(0x97, 0x33, 0x44, 0xF7)).InSequence(seq);
    uint8_t sysex[] = {0xF0, 0x55, 0x66, 0x77, 0x11, 0x22, 0x33, 0x44, 0xF7};
    midi.sendSysEx(sysex, CABLE_10);
}

TEST(USBMIDI_Interface, SysExSend0B) {
    StrictMock<USBMIDI_Interface> midi;
    midi.sendSysEx(nullptr, 0, CABLE_10);
}

TEST(USBMIDI_Interface, SysExSend1B) {
    StrictMock<USBMIDI_Interface> midi;
    uint8_t sysex[] = {0xF0};
    midi.sendSysEx(sysex, CABLE_10);
}

TEST(USBMIDI_Interface, SysExSend2B) {
    StrictMock<USBMIDI_Interface> midi;
    Sequence seq;
    EXPECT_CALL(midi.backend, write(0x96, 0xF0, 0xF7, 0x00)).InSequence(seq);
    uint8_t sysex[] = {0xF0, 0xF7};
    midi.sendSysEx(sysex, CABLE_10);
}

TEST(USBMIDI_Interface, SysExSendChunks) {
    StrictMock<USBMIDI_Interface> midi;
    Sequence seq;
    EXPECT_CALL(midi.backend, write(0x94, 0xF0, 0x55, 0x66)).InSequence(seq);
    EXPECT_CALL(midi.backend, write(0x94, 0x77, 0x11, 0x22)).InSequence(seq);
    EXPECT_CALL(midi.backend, write(0x94, 0x23, 0x24, 0x25)).InSequence(seq);
    EXPECT_CALL(midi.backend, write(0x94, 0x26, 0x27, 0x28)).InSequence(seq);
    EXPECT_CALL(midi.backend, write(0x94, 0x29, 0x2A, 0x2B)).InSequence(seq);
    EXPECT_CALL(midi.backend, write(0x94, 0x2C, 0x2D, 0x2E)).InSequence(seq);
    EXPECT_CALL(midi.backend, write(0x97, 0x33, 0x44, 0xF7)).InSequence(seq);
    std::vector<uint8_t> chunks[] = {
        {0xF0, 0x55, 0x66},
        {0x77, 0x11},
        {0x22},
        {0x23},
        {0x24},
        {0x25, 0x26},
        {0x27, 0x28},
        {0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E},
        {0x33},
        {0x44, 0xF7},
    };
    for (const auto &chunk : chunks)
        midi.send(SysExMessage(chunk, CABLE_10));
}

// -------------------------------------------------------------------------- //

TEST(USBMIDI_Interface, readRealTime) {
    StrictMock<USBMIDI_Interface> midi;
    EXPECT_CALL(midi.backend, read())
        .WillOnce(Return(USBMIDI_Interface::MIDIUSBPacket_t{0x3F, 0xF8, 0, 0}));
    RealTimeMessage expectedMsg = {MIDIMessageType::TIMING_CLOCK, CABLE_4};
    EXPECT_EQ(midi.read(), MIDIReadEvent::REALTIME_MESSAGE);
    EXPECT_EQ(midi.getRealTimeMessage(), expectedMsg);
}

TEST(USBMIDI_Interface, readNoteOn) {
    StrictMock<USBMIDI_Interface> midi;
    EXPECT_CALL(midi.backend, read())
        .WillOnce(
            Return(USBMIDI_Interface::MIDIUSBPacket_t{0x59, 0x93, 0x3C, 0x60}));
    EXPECT_EQ(midi.read(), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage expectedMsg = {0x93, 0x3C, 0x60, CABLE_6};
    EXPECT_EQ(midi.getChannelMessage(), expectedMsg);
}

TEST(USBMIDI_Interface, readSysCommon2) {
    StrictMock<USBMIDI_Interface> midi;
    EXPECT_CALL(midi.backend, read())
        .WillOnce(
            Return(USBMIDI_Interface::MIDIUSBPacket_t{0x53, 0xF2, 0x12, 0x34}));
    EXPECT_EQ(midi.read(), MIDIReadEvent::SYSCOMMON_MESSAGE);
    SysCommonMessage expectedMsg = {0xF2, 0x12, 0x34, CABLE_6};
    EXPECT_EQ(midi.getSysCommonMessage(), expectedMsg);
}

TEST(USBMIDI_Interface, readSysCommon1) {
    StrictMock<USBMIDI_Interface> midi;
    EXPECT_CALL(midi.backend, read())
        .WillOnce(
            Return(USBMIDI_Interface::MIDIUSBPacket_t{0x52, 0xF3, 0x56, 0x00}));
    EXPECT_EQ(midi.read(), MIDIReadEvent::SYSCOMMON_MESSAGE);
    SysCommonMessage expectedMsg = {0xF3, 0x56, 0x00, CABLE_6};
    EXPECT_EQ(midi.getSysCommonMessage(), expectedMsg);
}

TEST(USBMIDI_Interface, readSysCommon0) {
    StrictMock<USBMIDI_Interface> midi;
    EXPECT_CALL(midi.backend, read())
        .WillOnce(
            Return(USBMIDI_Interface::MIDIUSBPacket_t{0x55, 0xF6, 0x00, 0x00}));
    EXPECT_EQ(midi.read(), MIDIReadEvent::SYSCOMMON_MESSAGE);
    SysCommonMessage expectedMsg = {0xF6, 0x00, 0x00, CABLE_6};
    EXPECT_EQ(midi.getSysCommonMessage(), expectedMsg);
}

using SysExVector = std::vector<uint8_t>;

TEST(USBMIDI_Interface, readSysEx) {
    StrictMock<USBMIDI_Interface> midi;
    using Packet_t = USBMIDI_Interface::MIDIUSBPacket_t;
    EXPECT_CALL(midi.backend, read())
        .WillOnce(Return(Packet_t{{0x54, 0xF0, 0x55, 0x66}}))
        .WillOnce(Return(Packet_t{{0x54, 0x77, 0x11, 0x22}}))
        .WillOnce(Return(Packet_t{{0x56, 0x33, 0xF7, 0x00}}));
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
    EXPECT_EQ(sysex.cable, CABLE_6);
}