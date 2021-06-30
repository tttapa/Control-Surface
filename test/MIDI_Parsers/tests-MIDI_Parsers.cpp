#include <gtest/gtest.h>

#include <MIDI_Parsers/BufferPuller.hpp>
#include <MIDI_Parsers/SerialMIDI_Parser.hpp>
#include <MIDI_Parsers/USBMIDI_Parser.hpp>

using namespace CS;

using SysExVector = std::vector<uint8_t>;
using Packet_t = USBMIDI_Parser::MIDIUSBPacket_t;

// ---------------------------- USB PARSER TESTS ---------------------------- //

TEST(USBMIDIParser, noteOff) {
    USBMIDI_Parser uparser;
    Packet_t packets[] = {{0x08, 0x82, 0x20, 0x7F}};
    auto puller = BufferPuller(packets);
    EXPECT_EQ(uparser.pull(puller), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg = uparser.getChannelMessage();
    EXPECT_EQ(msg.header, 0x82);
    EXPECT_EQ(msg.data1, 0x20);
    EXPECT_EQ(msg.data2, 0x7F);
}

TEST(USBMIDIParser, noteOn) {
    USBMIDI_Parser uparser;
    Packet_t packets[] = {{0x09, 0x93, 0x2A, 0x7E}};
    auto puller = BufferPuller(packets);
    EXPECT_EQ(uparser.pull(puller), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg = uparser.getChannelMessage();
    EXPECT_EQ(msg.header, 0x93);
    EXPECT_EQ(msg.data1, 0x2A);
    EXPECT_EQ(msg.data2, 0x7E);
}

TEST(USBMIDIParser, noteOnCN) {
    USBMIDI_Parser uparser;
    Packet_t packets[] = {{0x59, 0x93, 0x2A, 0x7E}};
    auto puller = BufferPuller(packets);
    EXPECT_EQ(uparser.pull(puller), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg = uparser.getChannelMessage();
    EXPECT_EQ(msg.header, 0x93);
    EXPECT_EQ(msg.data1, 0x2A);
    EXPECT_EQ(msg.data2, 0x7E);
    EXPECT_EQ(msg.cable, CABLE_6);
}

TEST(USBMIDIParser, sysEx2Bytes) {
    USBMIDI_Parser uparser;
    Packet_t packets[] = {{0x06, 0xF0, 0xF7, 0x00}};
    auto puller = BufferPuller(packets);
    EXPECT_EQ(uparser.pull(puller), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(uparser.getSysExMessage(), SysExMessage({0xF0, 0xF7}));
}

TEST(USBMIDIParser, sysEx3Bytes) {
    USBMIDI_Parser uparser;
    Packet_t packets[] = {{0x07, 0xF0, 0x10, 0xF7}};
    auto puller = BufferPuller(packets);
    EXPECT_EQ(uparser.pull(puller), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(uparser.getSysExMessage(), SysExMessage({0xF0, 0x10, 0xF7}));
}

TEST(USBMIDIParser, sysEx4Bytes) {
    USBMIDI_Parser uparser;
    Packet_t packets[] = {
        {0x04, 0xF0, 0x11, 0x12},
        {0x05, 0xF7, 0x00, 0x00},
    };
    auto puller = BufferPuller(packets);
    EXPECT_EQ(uparser.pull(puller), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(uparser.getSysExMessage(),
              SysExMessage({0xF0, 0x11, 0x12, 0xF7}));
}

TEST(USBMIDIParser, sysEx5Bytes) {
    USBMIDI_Parser uparser;
    Packet_t packets[] = {
        {0x04, 0xF0, 0x21, 0x22},
        {0x06, 0x23, 0xF7, 0x00},
    };
    auto puller = BufferPuller(packets);
    EXPECT_EQ(uparser.pull(puller), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(uparser.getSysExMessage(),
              SysExMessage({0xF0, 0x21, 0x22, 0x23, 0xF7}));
}

TEST(USBMIDIParser, sysEx6Bytes) {
    USBMIDI_Parser uparser;
    Packet_t packets[] = {
        {0x04, 0xF0, 0x31, 0x32},
        {0x07, 0x33, 0x34, 0xF7},
    };
    auto puller = BufferPuller(packets);
    EXPECT_EQ(uparser.pull(puller), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(uparser.getSysExMessage(),
              SysExMessage({0xF0, 0x31, 0x32, 0x33, 0x34, 0xF7}));
}

TEST(USBMIDIParser, sysEx7Bytes) {
    USBMIDI_Parser uparser;
    Packet_t packets[] = {
        {0x04, 0xF0, 0x41, 0x42},
        {0x04, 0x43, 0x44, 0x45},
        {0x05, 0xF7, 0x00, 0x00},
    };
    auto puller = BufferPuller(packets);
    EXPECT_EQ(uparser.pull(puller), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(uparser.getSysExMessage(),
              SysExMessage({0xF0, 0x41, 0x42, 0x43, 0x44, 0x45, 0xF7}));
}

TEST(USBMIDIParser, sysEx12Bytes) {
    USBMIDI_Parser uparser;
    Packet_t packets[] = {
        {0x04, 0xF0, 0x51, 0x52},
        {0x04, 0x53, 0x54, 0x55},
        {0x04, 0x56, 0x57, 0x58},
        {0x07, 0x59, 0x5A, 0xF7},
    };
    auto puller = BufferPuller(packets);
    EXPECT_EQ(uparser.pull(puller), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(uparser.getSysExMessage(),
              SysExMessage({0xF0, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
                            0x58, 0x59, 0x5A, 0xF7}));
}

TEST(USBMIDIParser, Realtime) {
    USBMIDI_Parser uparser;
    Packet_t packets[] = {0x3F, 0xF8, 0x00, 0x00};
    auto puller = BufferPuller(packets);
    EXPECT_EQ(uparser.pull(puller), MIDIReadEvent::REALTIME_MESSAGE);
    RealTimeMessage expected = {MIDIMessageType::TIMING_CLOCK, CABLE_4};
    EXPECT_EQ(uparser.getRealTimeMessage(), expected);
}

TEST(USBMIDIParser, tuneRequest) {
    USBMIDI_Parser uparser;
    Packet_t packets[] = {0x45, 0xF6, 0x00, 0x00};
    auto puller = BufferPuller(packets);
    EXPECT_EQ(uparser.pull(puller), MIDIReadEvent::SYSCOMMON_MESSAGE);
    SysCommonMessage msg = uparser.getSysCommonMessage();
    EXPECT_EQ(msg.getMessageType(), MIDIMessageType::TUNE_REQUEST);
    EXPECT_EQ(msg.cable, CABLE_5);
}

TEST(USBMIDIParser, MTCTimeCode) {
    USBMIDI_Parser uparser;
    Packet_t packets[] = {0x42, 0xF1, 0x23, 0x00};
    auto puller = BufferPuller(packets);
    EXPECT_EQ(uparser.pull(puller), MIDIReadEvent::SYSCOMMON_MESSAGE);
    SysCommonMessage msg = uparser.getSysCommonMessage();
    EXPECT_EQ(msg.getMessageType(), MIDIMessageType::MTC_QUARTER_FRAME);
    EXPECT_EQ(msg.data1, 0x23);
    EXPECT_EQ(msg.cable, CABLE_5);
}

TEST(USBMIDIParser, songPositionPointer) {
    USBMIDI_Parser uparser;
    Packet_t packets[] = {0x43, 0xF2, 0x38, 0x57};
    auto puller = BufferPuller(packets);
    EXPECT_EQ(uparser.pull(puller), MIDIReadEvent::SYSCOMMON_MESSAGE);
    SysCommonMessage msg = uparser.getSysCommonMessage();
    EXPECT_EQ(msg.getMessageType(), MIDIMessageType::SONG_POSITION_POINTER);
    EXPECT_EQ(msg.data1, 0x38);
    EXPECT_EQ(msg.data2, 0x57);
    EXPECT_EQ(msg.cable, CABLE_5);
}

TEST(USBMIDIParser, sysExContinueWithoutStarting) {
    USBMIDI_Parser uparser;
    Packet_t packets[] = {0x04, 0x33, 0x34, 0xF7};
    auto puller = BufferPuller(packets);
    EXPECT_EQ(uparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(USBMIDIParser, sysExEndsWithoutStarting1Byte) {
    USBMIDI_Parser uparser;
    Packet_t packets[] = {0x05, 0xF7, 0x00, 0x00};
    auto puller = BufferPuller(packets);
    EXPECT_EQ(uparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(USBMIDIParser, sysExEndsWithoutStarting2Bytes) {
    USBMIDI_Parser uparser;
    Packet_t packets[] = {0x06, 0x10, 0xF7, 0x00};
    auto puller = BufferPuller(packets);
    EXPECT_EQ(uparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(USBMIDIParser, sysExEndsWithoutStarting3Bytes) {
    USBMIDI_Parser uparser;
    Packet_t packets[] = {0x07, 0x10, 0x11, 0xF7};
    auto puller = BufferPuller(packets);
    EXPECT_EQ(uparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

// -------------------------- SERIAL PARSER TESTS --------------------------- //

TEST(SerialMIDIParser, noteOff) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0x82, 0x20, 0x7F};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg = sparser.getChannelMessage();
    EXPECT_EQ(msg.header, 0x82);
    EXPECT_EQ(msg.data1, 0x20);
    EXPECT_EQ(msg.data2, 0x7F);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, noteOn) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0x93, 0x2A, 0x7E};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg = sparser.getChannelMessage();
    EXPECT_EQ(msg.header, 0x93);
    EXPECT_EQ(msg.data1, 0x2A);
    EXPECT_EQ(msg.data2, 0x7E);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, noteOnRunningStatus) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0x9A, 0x10, 0x11, 0x12, 0x13};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg1 = sparser.getChannelMessage();
    EXPECT_EQ(msg1.header, 0x9A);
    EXPECT_EQ(msg1.data1, 0x10);
    EXPECT_EQ(msg1.data2, 0x11);

    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg2 = sparser.getChannelMessage();
    EXPECT_EQ(msg2.header, 0x9A);
    EXPECT_EQ(msg2.data1, 0x12);
    EXPECT_EQ(msg2.data2, 0x13);

    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, noteOnRunningStatusSysCommon1) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0x9A, 0x10, 0x11, //
                      0xF6,             //
                      0x12, 0x13};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg1 = sparser.getChannelMessage();
    EXPECT_EQ(msg1.header, 0x9A);
    EXPECT_EQ(msg1.data1, 0x10);
    EXPECT_EQ(msg1.data2, 0x11);

    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSCOMMON_MESSAGE);
    SysCommonMessage msg2 = sparser.getSysCommonMessage();
    EXPECT_EQ(msg2.header, 0xF6);

    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, noteOnRunningStatusSysCommon2) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0x9A, 0x10, 0x11, //
                      0xF1, 0x55,       //
                      0x12, 0x13};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg1 = sparser.getChannelMessage();
    EXPECT_EQ(msg1.header, 0x9A);
    EXPECT_EQ(msg1.data1, 0x10);
    EXPECT_EQ(msg1.data2, 0x11);

    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSCOMMON_MESSAGE);
    SysCommonMessage msg2 = sparser.getSysCommonMessage();
    EXPECT_EQ(msg2.header, 0xF1);
    EXPECT_EQ(msg2.data1, 0x55);

    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, noteOnRunningStatusSysCommon3) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0x9A, 0x10, 0x11, //
                      0xF2, 0x55, 0x66, //
                      0x12, 0x13};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg1 = sparser.getChannelMessage();
    EXPECT_EQ(msg1.header, 0x9A);
    EXPECT_EQ(msg1.data1, 0x10);
    EXPECT_EQ(msg1.data2, 0x11);

    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSCOMMON_MESSAGE);
    SysCommonMessage msg2 = sparser.getSysCommonMessage();
    EXPECT_EQ(msg2.header, 0xF2);
    EXPECT_EQ(msg2.data1, 0x55);
    EXPECT_EQ(msg2.data2, 0x66);

    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, noteOnRunningStatusSysCommon1BLE) {
    SerialMIDI_Parser sparser{false};
    uint8_t data[] = {0x9A, 0x10, 0x11, //
                      0xF6,             //
                      0x12, 0x13};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg1 = sparser.getChannelMessage();
    EXPECT_EQ(msg1.header, 0x9A);
    EXPECT_EQ(msg1.data1, 0x10);
    EXPECT_EQ(msg1.data2, 0x11);

    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSCOMMON_MESSAGE);
    SysCommonMessage msg2 = sparser.getSysCommonMessage();
    EXPECT_EQ(msg2.header, 0xF6);

    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg3 = sparser.getChannelMessage();
    EXPECT_EQ(msg3.header, 0x9A);
    EXPECT_EQ(msg3.data1, 0x12);
    EXPECT_EQ(msg3.data2, 0x13);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, noteOnRunningStatusSysCommon2BLE) {
    SerialMIDI_Parser sparser{false};
    uint8_t data[] = {0x9A, 0x10, 0x11, //
                      0xF1, 0x55,       //
                      0x12, 0x13};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg1 = sparser.getChannelMessage();
    EXPECT_EQ(msg1.header, 0x9A);
    EXPECT_EQ(msg1.data1, 0x10);
    EXPECT_EQ(msg1.data2, 0x11);

    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSCOMMON_MESSAGE);
    SysCommonMessage msg2 = sparser.getSysCommonMessage();
    EXPECT_EQ(msg2.header, 0xF1);
    EXPECT_EQ(msg2.data1, 0x55);

    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg3 = sparser.getChannelMessage();
    EXPECT_EQ(msg3.header, 0x9A);
    EXPECT_EQ(msg3.data1, 0x12);
    EXPECT_EQ(msg3.data2, 0x13);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, noteOnRunningStatusSysCommon3BLE) {
    SerialMIDI_Parser sparser{false};
    uint8_t data[] = {0x9A, 0x10, 0x11, //
                      0xF2, 0x55, 0x66, //
                      0x12, 0x13};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg1 = sparser.getChannelMessage();
    EXPECT_EQ(msg1.header, 0x9A);
    EXPECT_EQ(msg1.data1, 0x10);
    EXPECT_EQ(msg1.data2, 0x11);

    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSCOMMON_MESSAGE);
    SysCommonMessage msg2 = sparser.getSysCommonMessage();
    EXPECT_EQ(msg2.header, 0xF2);
    EXPECT_EQ(msg2.data1, 0x55);
    EXPECT_EQ(msg2.data2, 0x66);

    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg3 = sparser.getChannelMessage();
    EXPECT_EQ(msg3.header, 0x9A);
    EXPECT_EQ(msg3.data1, 0x12);
    EXPECT_EQ(msg3.data2, 0x13);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, afterTouch) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0xA1, 0x01, 0x02};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg = sparser.getChannelMessage();
    EXPECT_EQ(msg.header, 0xA1);
    EXPECT_EQ(msg.data1, 0x01);
    EXPECT_EQ(msg.data2, 0x02);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, controlChange) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0xBB, 0x03, 0x04};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg = sparser.getChannelMessage();
    EXPECT_EQ(msg.header, 0xBB);
    EXPECT_EQ(msg.data1, 0x03);
    EXPECT_EQ(msg.data2, 0x04);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, programChange) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0xC6, 0x7A};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg = sparser.getChannelMessage();
    EXPECT_EQ(msg.header, 0xC6);
    EXPECT_EQ(msg.data1, 0x7A);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, programChangeRunningStatus) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0xC6, 0x7B, 0x7C};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg1 = sparser.getChannelMessage();
    EXPECT_EQ(msg1.header, 0xC6);
    EXPECT_EQ(msg1.data1, 0x7B);

    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg2 = sparser.getChannelMessage();
    EXPECT_EQ(msg2.header, 0xC6);
    EXPECT_EQ(msg2.data1, 0x7C);

    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, channelPressure) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0xD7, 0x16};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg = sparser.getChannelMessage();
    EXPECT_EQ(msg.header, 0xD7);
    EXPECT_EQ(msg.data1, 0x16);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, pitchBend) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0xE0, 0x55, 0x66};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg = sparser.getChannelMessage();
    EXPECT_EQ(msg.header, 0xE0);
    EXPECT_EQ(msg.data1, 0x55);
    EXPECT_EQ(msg.data2, 0x66);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, sysEx2Bytes) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0xF0, 0xF7};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(sparser.getSysExMessage(), SysExMessage(data));
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, sysEx3Bytes) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0xF0, 0x10, 0xF7};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSEX_MESSAGE);
    const SysExVector expected = {0xF0, 0x10, 0xF7};
    EXPECT_EQ(sparser.getSysExMessage(), SysExMessage(data));
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, sysEx7Bytes) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0xF0, 0x41, 0x42, 0x43, 0x44, 0x45, 0xF7};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(sparser.getSysExMessage(), SysExMessage(data));
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, sysEx2BytesBeforeOther) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0xF0, 0xF7, 0xF0, 0x12, 0xF7};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(sparser.getSysExMessage(), SysExMessage({0xF0, 0xF7}));
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(sparser.getSysExMessage(), SysExMessage({0xF0, 0x12, 0xF7}));
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, sysEx2BytesBeforeOtherNoSysExEnd) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0xF0, 0xF0, 0x12, 0xF7};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(sparser.getSysExMessage(), SysExMessage({0xF0, 0xF7}));
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(sparser.getSysExMessage(), SysExMessage({0xF0, 0x12, 0xF7}));
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, sysExDataAfterEnd) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0xF0, 0xF7, 0x12, 0x13, 0xF0, 0x14, 0x15, 0xF7,
                      0x16, 0x17, 0xF0, 0x18, 0x19, 0xF7, 0x1A};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(sparser.getSysExMessage(), SysExMessage({0xF0, 0xF7}));
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(sparser.getSysExMessage(),
              SysExMessage({0xF0, 0x14, 0x15, 0xF7}));
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(sparser.getSysExMessage(),
              SysExMessage({0xF0, 0x18, 0x19, 0xF7}));
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, RealTime) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0xF8};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::REALTIME_MESSAGE);
    EXPECT_EQ(sparser.getRealTimeMessage(),
              RealTimeMessage(MIDIMessageType::TIMING_CLOCK, CABLE_1));
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, MTCTimeCode) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0xF1, 0x56};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSCOMMON_MESSAGE);
    SysCommonMessage msg = sparser.getSysCommonMessage();
    EXPECT_EQ(msg.header, 0xF1);
    EXPECT_EQ(msg.data1, 0x56);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, songPositionCode) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0xF2, 0x45, 0x67};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSCOMMON_MESSAGE);
    SysCommonMessage msg = sparser.getSysCommonMessage();
    EXPECT_EQ(msg.header, 0xF2);
    EXPECT_EQ(msg.data1, 0x45);
    EXPECT_EQ(msg.data2, 0x67);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, dataAfterSongPositionCode) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0xF2, 0x45, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSCOMMON_MESSAGE);
    SysCommonMessage msg = sparser.getSysCommonMessage();
    EXPECT_EQ(msg.header, 0xF2);
    EXPECT_EQ(msg.data1, 0x45);
    EXPECT_EQ(msg.data2, 0x67);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, songSelect) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0xF3, 0x38};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSCOMMON_MESSAGE);
    SysCommonMessage msg = sparser.getSysCommonMessage();
    EXPECT_EQ(msg.header, 0xF3);
    EXPECT_EQ(msg.data1, 0x38);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, dataAfterSongSelect) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0xF3, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSCOMMON_MESSAGE);
    SysCommonMessage msg = sparser.getSysCommonMessage();
    EXPECT_EQ(msg.header, 0xF3);
    EXPECT_EQ(msg.data1, 0x38);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, tuneRequest) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0xF6};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSCOMMON_MESSAGE);
    SysCommonMessage msg = sparser.getSysCommonMessage();
    EXPECT_EQ(msg.header, 0xF6);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, dataAfterTuneRequest) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0xF6, 0x00, 0x00, 0x00, 0x00, 0x00};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSCOMMON_MESSAGE);
    SysCommonMessage msg = sparser.getSysCommonMessage();
    EXPECT_EQ(msg.header, 0xF6);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, noteOffInterruptedByRealTime) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0x82, 0xF8, 0x20, 0xF9, 0x7F};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::REALTIME_MESSAGE);
    RealTimeMessage rt1 = {MIDIMessageType::TIMING_CLOCK};
    EXPECT_EQ(sparser.getRealTimeMessage(), rt1);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::REALTIME_MESSAGE);
    RealTimeMessage rt2 = {MIDIMessageType::UNDEFINED_REALTIME_1};
    EXPECT_EQ(sparser.getRealTimeMessage(), rt2);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg = sparser.getChannelMessage();
    EXPECT_EQ(msg.header, 0x82);
    EXPECT_EQ(msg.data1, 0x20);
    EXPECT_EQ(msg.data2, 0x7F);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, sysExInterruptedByRealTime) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0xF0, 0x01, 0x02, 0xF8, 0x03, 0xF7};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::REALTIME_MESSAGE);
    RealTimeMessage rt1 = {MIDIMessageType::TIMING_CLOCK};
    EXPECT_EQ(sparser.getRealTimeMessage(), rt1);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(sparser.getSysExMessage(),
              SysExMessage({0xF0, 0x01, 0x02, 0x03, 0xF7}));
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, sysExEndsByChannelMessage) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0xF0, 0x01, 0x02, 0x03, 0x8F, 0x10, 0x12};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(sparser.getSysExMessage(),
              SysExMessage({0xF0, 0x01, 0x02, 0x03, 0xF7}));
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg = sparser.getChannelMessage();
    EXPECT_EQ(msg.header, 0x8F);
    EXPECT_EQ(msg.data1, 0x10);
    EXPECT_EQ(msg.data2, 0x12);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, sysExEndsBySysCommonTuneRequest) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0xF0, 0x01, 0x02, 0x03, 0xF6};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(sparser.getSysExMessage(),
              SysExMessage({0xF0, 0x01, 0x02, 0x03, 0xF7}));
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSCOMMON_MESSAGE);
    SysCommonMessage msg = sparser.getSysCommonMessage();
    EXPECT_EQ(msg.header, 0xF6);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, sysExEndsBySysCommonMTCTimeCode) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0xF0, 0x01, 0x02, 0x03, 0xF1, 0x04};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(sparser.getSysExMessage(),
              SysExMessage({0xF0, 0x01, 0x02, 0x03, 0xF7}));
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSCOMMON_MESSAGE);
    SysCommonMessage msg = sparser.getSysCommonMessage();
    EXPECT_EQ(msg.header, 0xF1);
    EXPECT_EQ(msg.data1, 0x04);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, sysExEndsBySysCommonSongPositionPointer) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0xF0, 0x01, 0x02, 0x03, 0xF2, 0x04, 0x05};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(sparser.getSysExMessage(),
              SysExMessage({0xF0, 0x01, 0x02, 0x03, 0xF7}));
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSCOMMON_MESSAGE);
    SysCommonMessage msg = sparser.getSysCommonMessage();
    EXPECT_EQ(msg.header, 0xF2);
    EXPECT_EQ(msg.data1, 0x04);
    EXPECT_EQ(msg.data2, 0x05);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, sysExEndsWithoutStarting1Byte) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0xF7, 0xF7, 0xF7};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, sysExEndsWithoutStarting2Bytes) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0x51, 0xF7, 0xF7, 0xF7};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, sysExEndsWithoutStarting3Bytes) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0x52, 0x53, 0xF7, 0xF7, 0xF7};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, sysExEndsMultipleTimesWithoutStarting) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0xF0, 0x10, 0xF7, 0x20, 0xF7, 0xF7};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, sysExStartsTwice) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0xF0, 0xF0, 0x20, 0x30, 0xF7};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(sparser.getSysExMessage(), SysExMessage({0xF0, 0xF7}));
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(sparser.getSysExMessage(),
              SysExMessage({0xF0, 0x20, 0x30, 0xF7}));
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, sysExEndsWithSysExStart) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {0xF0, 0x10, 0xF0, 0x20, 0x30, 0xF7};
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(sparser.getSysExMessage(), SysExMessage({0xF0, 0x10, 0xF7}));
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(sparser.getSysExMessage(),
              SysExMessage({0xF0, 0x20, 0x30, 0xF7}));
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

#include <random>

TEST(SerialMIDIParser, sysExMultipleChunks) {
    SerialMIDI_Parser sparser;
    std::vector<uint8_t> data(2 * SYSEX_BUFFER_SIZE + 1);
    data.front() = 0xF0;
    data.back() = 0xF7;
    std::mt19937 rnd(0);
    std::uniform_int_distribution<uint8_t> dist(0, 127);
    std::generate(data.begin() + 1, data.end() - 1, std::bind(dist, rnd));
    auto puller = BufferPuller(data);

    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSEX_CHUNK);
    EXPECT_EQ(sparser.getSysExMessage(),
              SysExMessage(data.data(), SYSEX_BUFFER_SIZE));
    EXPECT_TRUE(sparser.getSysExMessage().isFirstChunk());
    EXPECT_FALSE(sparser.getSysExMessage().isLastChunk());
    EXPECT_FALSE(sparser.getSysExMessage().isCompleteMessage());
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSEX_CHUNK);
    EXPECT_EQ(sparser.getSysExMessage(),
              SysExMessage(data.data() + SYSEX_BUFFER_SIZE, SYSEX_BUFFER_SIZE));
    EXPECT_FALSE(sparser.getSysExMessage().isFirstChunk());
    EXPECT_FALSE(sparser.getSysExMessage().isLastChunk());
    EXPECT_FALSE(sparser.getSysExMessage().isCompleteMessage());
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(sparser.getSysExMessage(),
              SysExMessage(data.data() + 2 * SYSEX_BUFFER_SIZE, 1));
    EXPECT_FALSE(sparser.getSysExMessage().isFirstChunk());
    EXPECT_TRUE(sparser.getSysExMessage().isLastChunk());
    EXPECT_FALSE(sparser.getSysExMessage().isCompleteMessage());
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}

TEST(USBMIDIParser, sysExMultipleChunks) {
    USBMIDI_Parser uparser;
    const size_t buffsize3 = (SYSEX_BUFFER_SIZE / 3) * 3;
    const size_t size = buffsize3 + SYSEX_BUFFER_SIZE + 1;
    std::vector<uint8_t> data(size);
    std::vector<uint8_t> packet(4);
    data.front() = 0xF0;
    data.back() = 0xF7;
    std::mt19937 rnd(0);
    std::uniform_int_distribution<uint8_t> dist(0, 127);
    std::generate(data.begin() + 1, data.end() - 1, std::bind(dist, rnd));

    std::vector<Packet_t> packets;
    {
        packets.reserve(size / 3 + 3);
        size_t length = size;
        auto it = data.begin();
        while (length > 3) {
            packets.push_back({0x54, it[0], it[1], it[2]});
            it += 3;
            length -= 3;
        }
        switch (length) {
            case 3: packets.push_back({0x57, it[0], it[1], it[2]}); break;
            case 2: packets.push_back({0x56, it[0], it[1], 0}); break;
            case 1: packets.push_back({0x55, it[0], 0, 0}); break;
            default: break;
        }
    }
    auto puller = BufferPuller(packets);

    EXPECT_EQ(uparser.pull(puller), MIDIReadEvent::SYSEX_CHUNK);
    auto size1 = uparser.getSysExMessage().length;
    EXPECT_GE(size1, buffsize3);
    EXPECT_LE(size1, SYSEX_BUFFER_SIZE);
    EXPECT_EQ(uparser.getSysExMessage(),
              SysExMessage(data.data(), size1, CABLE_6));
    EXPECT_TRUE(uparser.getSysExMessage().isFirstChunk());
    EXPECT_FALSE(uparser.getSysExMessage().isLastChunk());
    EXPECT_FALSE(uparser.getSysExMessage().isCompleteMessage());

    EXPECT_EQ(uparser.pull(puller), MIDIReadEvent::SYSEX_CHUNK);
    auto size2 = uparser.getSysExMessage().length;
    EXPECT_GE(size2, buffsize3);
    EXPECT_LE(size2, SYSEX_BUFFER_SIZE);
    EXPECT_EQ(uparser.getSysExMessage(),
              SysExMessage(data.data() + size1, size2, CABLE_6));
    EXPECT_FALSE(uparser.getSysExMessage().isFirstChunk());
    EXPECT_FALSE(uparser.getSysExMessage().isFirstChunk());
    EXPECT_FALSE(uparser.getSysExMessage().isLastChunk());
    EXPECT_FALSE(uparser.getSysExMessage().isCompleteMessage());

    EXPECT_EQ(uparser.pull(puller), MIDIReadEvent::SYSEX_MESSAGE);
    auto size3 = uparser.getSysExMessage().length;
    EXPECT_EQ(size1 + size2 + size3, size);
    EXPECT_EQ(uparser.getSysExMessage(),
              SysExMessage(data.data() + size1 + size2, size3, CABLE_6));
    EXPECT_FALSE(uparser.getSysExMessage().isFirstChunk());
    EXPECT_TRUE(uparser.getSysExMessage().isLastChunk());
    EXPECT_FALSE(uparser.getSysExMessage().isCompleteMessage());
}

TEST(SerialMIDIParser, realSysEx) {
    SerialMIDI_Parser sparser;
    uint8_t data[] = {
        0xF0, 0x0,  0x0,  0x66, 0x14, 0x12, 0x0,  0x20, 0x20, 0x20, 0x20, 0x20,
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x2d, 0x20, 0x4d, 0x61,
        0x63, 0x6b, 0x69, 0x65, 0x20, 0x43, 0x6f, 0x6e, 0x74, 0x72, 0x6f, 0x6c,
        0x20, 0x55, 0x6e, 0x69, 0x76, 0x65, 0x72, 0x73, 0x61, 0x6c, 0x20, 0x2d,
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xF7,
    };
    auto puller = BufferPuller(data);
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(sparser.getSysExMessage(), SysExMessage(data));
    EXPECT_EQ(sparser.pull(puller), MIDIReadEvent::NO_MESSAGE);
}