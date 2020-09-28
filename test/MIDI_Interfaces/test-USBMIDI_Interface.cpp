#include <MIDI_Interfaces/USBMIDI_Interface.hpp>
#include <gmock-wrapper.h>
#include <gtest-wrapper.h>

USING_CS_NAMESPACE;
using ::testing::Return;
using ::testing::Sequence;
using ::testing::StrictMock;

using AH::ErrorException;

TEST(USBMIDI_Interface, send3B) {
    StrictMock<USBMIDI_Interface> midi;
    EXPECT_CALL(midi, writeUSBPacket(8, 0x9, 0x93, 0x55, 0x66));
    midi.sendNoteOn({0x55, CHANNEL_4, CABLE_9}, 0x66);
}

TEST(USBMIDI_Interface, send2B) {
    StrictMock<USBMIDI_Interface> midi;
    EXPECT_CALL(midi, writeUSBPacket(8, 0xC, 0xC3, 0x66, 0x00));
    midi.sendPC({CHANNEL_4, CABLE_9}, 0x66);
}

TEST(USBMIDI_Interface, RealTime) {
    StrictMock<USBMIDI_Interface> midi;
    Sequence seq;
    EXPECT_CALL(midi, writeUSBPacket(8, 0xF, 0xF8, 0x00, 0x00)).InSequence(seq);
    midi.sendOnCable(MIDIMessageType::TIMING_CLOCK, CABLE_9);
}

TEST(USBMIDI_Interface, SysExSend3B) {
    StrictMock<USBMIDI_Interface> midi;
    Sequence seq;
    EXPECT_CALL(midi, writeUSBPacket(8, 0x7, 0xF0, 0x55, 0xF7)).InSequence(seq);
    uint8_t sysex[] = {0xF0, 0x55, 0xF7};
    midi.send(sysex, CABLE_9);
}

TEST(USBMIDI_Interface, SysExSend4B) {
    StrictMock<USBMIDI_Interface> midi;
    Sequence seq;
    EXPECT_CALL(midi, writeUSBPacket(9, 0x4, 0xF0, 0x55, 0x66)).InSequence(seq);
    EXPECT_CALL(midi, writeUSBPacket(9, 0x5, 0xF7, 0x00, 0x00)).InSequence(seq);
    uint8_t sysex[] = {0xF0, 0x55, 0x66, 0xF7};
    midi.send(sysex, CABLE_10);
}

TEST(USBMIDI_Interface, SysExSend5B) {
    StrictMock<USBMIDI_Interface> midi;
    Sequence seq;
    EXPECT_CALL(midi, writeUSBPacket(9, 0x4, 0xF0, 0x55, 0x66)).InSequence(seq);
    EXPECT_CALL(midi, writeUSBPacket(9, 0x6, 0x77, 0xF7, 0x00)).InSequence(seq);
    uint8_t sysex[] = {0xF0, 0x55, 0x66, 0x77, 0xF7};
    midi.send(sysex, CABLE_10);
}

TEST(USBMIDI_Interface, SysExSend6B) {
    StrictMock<USBMIDI_Interface> midi;
    Sequence seq;
    EXPECT_CALL(midi, writeUSBPacket(9, 0x4, 0xF0, 0x55, 0x66)).InSequence(seq);
    EXPECT_CALL(midi, writeUSBPacket(9, 0x7, 0x77, 0x11, 0xF7)).InSequence(seq);
    uint8_t sysex[] = {0xF0, 0x55, 0x66, 0x77, 0x11, 0xF7};
    midi.send(sysex, CABLE_10);
}

TEST(USBMIDI_Interface, SysExSend7B) {
    StrictMock<USBMIDI_Interface> midi;
    Sequence seq;
    EXPECT_CALL(midi, writeUSBPacket(9, 0x4, 0xF0, 0x55, 0x66)).InSequence(seq);
    EXPECT_CALL(midi, writeUSBPacket(9, 0x4, 0x77, 0x11, 0x22)).InSequence(seq);
    EXPECT_CALL(midi, writeUSBPacket(9, 0x5, 0xF7, 0x00, 0x00)).InSequence(seq);
    uint8_t sysex[] = {0xF0, 0x55, 0x66, 0x77, 0x11, 0x22, 0xF7};
    midi.send(sysex, CABLE_10);
}

TEST(USBMIDI_Interface, SysExSend8B) {
    StrictMock<USBMIDI_Interface> midi;
    Sequence seq;
    EXPECT_CALL(midi, writeUSBPacket(9, 0x4, 0xF0, 0x55, 0x66)).InSequence(seq);
    EXPECT_CALL(midi, writeUSBPacket(9, 0x4, 0x77, 0x11, 0x22)).InSequence(seq);
    EXPECT_CALL(midi, writeUSBPacket(9, 0x6, 0x33, 0xF7, 0x00)).InSequence(seq);
    uint8_t sysex[] = {0xF0, 0x55, 0x66, 0x77, 0x11, 0x22, 0x33, 0xF7};
    midi.send(sysex, CABLE_10);
}

TEST(USBMIDI_Interface, SysExSend9B) {
    StrictMock<USBMIDI_Interface> midi;
    Sequence seq;
    EXPECT_CALL(midi, writeUSBPacket(9, 0x4, 0xF0, 0x55, 0x66)).InSequence(seq);
    EXPECT_CALL(midi, writeUSBPacket(9, 0x4, 0x77, 0x11, 0x22)).InSequence(seq);
    EXPECT_CALL(midi, writeUSBPacket(9, 0x7, 0x33, 0x44, 0xF7)).InSequence(seq);
    uint8_t sysex[] = {0xF0, 0x55, 0x66, 0x77, 0x11, 0x22, 0x33, 0x44, 0xF7};
    midi.send(sysex, CABLE_10);
}

TEST(USBMIDI_Interface, SysExSend0B) {
    StrictMock<USBMIDI_Interface> midi;
    midi.send(SysExMessage{});
}

TEST(USBMIDI_Interface, SysExSend2B) {
    StrictMock<USBMIDI_Interface> midi;
    Sequence seq;
    EXPECT_CALL(midi, writeUSBPacket(9, 0x6, 0xF0, 0xF7, 0x00)).InSequence(seq);
    uint8_t sysex[] = {0xF0, 0xF7};
    midi.send(sysex, CABLE_10);
}

TEST(USBMIDI_Interface, SysExSend1B) {
    StrictMock<USBMIDI_Interface> midi;
    uint8_t sysex[] = {0xF0};
    try {
        midi.send(sysex);
        FAIL();
    } catch (ErrorException &e) {
        EXPECT_EQ(e.getErrorCode(), 0x7F7F);
    }
}

TEST(USBMIDI_Interface, readRealTime) {
    StrictMock<USBMIDI_Interface> midi;
    EXPECT_CALL(midi, readUSBPacket())
        .WillOnce(Return(USBMIDI_Interface::MIDIUSBPacket_t{0x3F, 0xF8, 0, 0}));
    RealTimeMessage expectedMsg = {MIDIMessageType::TIMING_CLOCK, 0x3};
    EXPECT_EQ(midi.read(), MIDIReadEvent::REALTIME_MESSAGE);
    EXPECT_EQ(midi.getRealTimeMessage(), expectedMsg);
}

TEST(USBMIDI_Interface, readNoteOn) {
    StrictMock<USBMIDI_Interface> midi;
    EXPECT_CALL(midi, readUSBPacket())
        .WillOnce(
            Return(USBMIDI_Interface::MIDIUSBPacket_t{0x59, 0x93, 0x3C, 0x60}));
    EXPECT_EQ(midi.read(), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage expectedMsg = {0x93, 0x3C, 0x60, 0x05};
    EXPECT_EQ(midi.getChannelMessage(), expectedMsg);
}

using SysExVector = std::vector<uint8_t>;

TEST(USBMIDI_Interface, readSysEx) {
    StrictMock<USBMIDI_Interface> midi;
    using Packet_t = USBMIDI_Interface::MIDIUSBPacket_t;
    EXPECT_CALL(midi, readUSBPacket())
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
    EXPECT_EQ(sysex.cable, 5);
}