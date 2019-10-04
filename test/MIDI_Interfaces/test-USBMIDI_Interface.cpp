#include <MIDI_Interfaces/USBMIDI_Interface.hpp>
#include <gmock-wrapper.h>
#include <gtest-wrapper.h>

USING_CS_NAMESPACE;
using ::testing::Return;
using ::testing::Sequence;

TEST(USBMIDI_Interface, SysExSend3B) {
    USBMIDI_Interface midi;
    Sequence seq;
    EXPECT_CALL(midi, writeUSBPacket(8, 0x7, 0xF0, 0x55, 0xF7)).InSequence(seq);
    uint8_t sysex[] = {0xF0, 0x55, 0xF7};
    midi.send(sysex, 8);
}

TEST(USBMIDI_Interface, SysExSend4B) {
    USBMIDI_Interface midi;
    Sequence seq;
    EXPECT_CALL(midi, writeUSBPacket(9, 0x4, 0xF0, 0x55, 0x66)).InSequence(seq);
    EXPECT_CALL(midi, writeUSBPacket(9, 0x5, 0xF7, 0x00, 0x00)).InSequence(seq);
    uint8_t sysex[] = {0xF0, 0x55, 0x66, 0xF7};
    midi.send(sysex, 9);
}

TEST(USBMIDI_Interface, SysExSend5B) {
    USBMIDI_Interface midi;
    Sequence seq;
    EXPECT_CALL(midi, writeUSBPacket(9, 0x4, 0xF0, 0x55, 0x66)).InSequence(seq);
    EXPECT_CALL(midi, writeUSBPacket(9, 0x6, 0x77, 0xF7, 0x00)).InSequence(seq);
    uint8_t sysex[] = {0xF0, 0x55, 0x66, 0x77, 0xF7};
    midi.send(sysex, 9);
}

TEST(USBMIDI_Interface, SysExSend6B) {
    USBMIDI_Interface midi;
    Sequence seq;
    EXPECT_CALL(midi, writeUSBPacket(9, 0x4, 0xF0, 0x55, 0x66)).InSequence(seq);
    EXPECT_CALL(midi, writeUSBPacket(9, 0x7, 0x77, 0x11, 0xF7)).InSequence(seq);
    uint8_t sysex[] = {0xF0, 0x55, 0x66, 0x77, 0x11, 0xF7};
    midi.send(sysex, 9);
}

TEST(USBMIDI_Interface, SysExSend7B) {
    USBMIDI_Interface midi;
    Sequence seq;
    EXPECT_CALL(midi, writeUSBPacket(9, 0x4, 0xF0, 0x55, 0x66)).InSequence(seq);
    EXPECT_CALL(midi, writeUSBPacket(9, 0x4, 0x77, 0x11, 0x22)).InSequence(seq);
    EXPECT_CALL(midi, writeUSBPacket(9, 0x5, 0xF7, 0x00, 0x00)).InSequence(seq);
    uint8_t sysex[] = {0xF0, 0x55, 0x66, 0x77, 0x11, 0x22, 0xF7};
    midi.send(sysex, 9);
}

TEST(USBMIDI_Interface, SysExSend8B) {
    USBMIDI_Interface midi;
    Sequence seq;
    EXPECT_CALL(midi, writeUSBPacket(9, 0x4, 0xF0, 0x55, 0x66)).InSequence(seq);
    EXPECT_CALL(midi, writeUSBPacket(9, 0x4, 0x77, 0x11, 0x22)).InSequence(seq);
    EXPECT_CALL(midi, writeUSBPacket(9, 0x6, 0x33, 0xF7, 0x00)).InSequence(seq);
    uint8_t sysex[] = {0xF0, 0x55, 0x66, 0x77, 0x11, 0x22, 0x33, 0xF7};
    midi.send(sysex, 9);
}

TEST(USBMIDI_Interface, SysExSend9B) {
    USBMIDI_Interface midi;
    Sequence seq;
    EXPECT_CALL(midi, writeUSBPacket(9, 0x4, 0xF0, 0x55, 0x66)).InSequence(seq);
    EXPECT_CALL(midi, writeUSBPacket(9, 0x4, 0x77, 0x11, 0x22)).InSequence(seq);
    EXPECT_CALL(midi, writeUSBPacket(9, 0x7, 0x33, 0x44, 0xF7)).InSequence(seq);
    uint8_t sysex[] = {0xF0, 0x55, 0x66, 0x77, 0x11, 0x22, 0x33, 0x44, 0xF7};
    midi.send(sysex, 9);
}

TEST(USBMIDI_Interface, SysExSend0B) {
    USBMIDI_Interface midi;
    midi.send(SysExMessage{});
}

TEST(USBMIDI_Interface, SysExSend2B) {
    USBMIDI_Interface midi;
    uint8_t sysex[] = {0xF0, 0xF7};
    try {
        midi.send(sysex);
        FAIL();
    } catch (ErrorException &e) {
        EXPECT_EQ(e.getErrorCode(), 0x7F7F);
    }
}