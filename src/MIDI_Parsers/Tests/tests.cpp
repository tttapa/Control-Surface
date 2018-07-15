// g++ -Wall -std=c++11 -o tests -I/home/pieter/lib/Catch2/single_include *.cpp ../*.cpp && ./tests

#if !defined(ARDUINO) && !defined(GOOGLETEST)

#include <catch.hpp>
using Catch::Matchers::Equals;

#include "../USBMIDI_Parser.h"
#include "../SerialMIDI_Parser.h"

typedef std::vector<uint8_t> SysExVector;

// --------------------------- USB PARSER TESTS --------------------------- //

USBMIDI_Parser uparser;

TEST_CASE("USB: Note Off message", "[CHANNEL_MESSAGE][USB]")
{
    uint8_t packet[4] = {0x08, 0x82, 0x20, 0x7F};
    REQUIRE(uparser.parse(packet) == CHANNEL_MESSAGE);
    MIDI_message msg = uparser.getChannelMessage();
    REQUIRE(msg.header == 0x82);
    REQUIRE(msg.data1 == 0x20);
    REQUIRE(msg.data2 == 0x7F);
}

TEST_CASE("USB: Note On message", "[CHANNEL_MESSAGE][USB]")
{
    uint8_t packet[4] = {0x09, 0x93, 0x2A, 0x7E};
    REQUIRE(uparser.parse(packet) == CHANNEL_MESSAGE);
    MIDI_message msg = uparser.getChannelMessage();
    REQUIRE(msg.header == 0x93);
    REQUIRE(msg.data1 == 0x2A);
    REQUIRE(msg.data2 == 0x7E);
}

TEST_CASE("USB: SysEx short 2B", "[SYSEX_MESSAGE][USB]")
{
    uint8_t packet[4] = {0x06, 0xF0, 0xF7, 0x00};
    REQUIRE(uparser.parse(packet) == SYSEX_MESSAGE);
    REQUIRE(uparser.getSysExLength() == 2);
    const SysExVector result(uparser.getSysExBuffer(),
                             uparser.getSysExBuffer() + 2);
    const SysExVector expected = {0xF0, 0xF7};
    REQUIRE_THAT( result, Equals(expected) );
}

TEST_CASE("USB: SysEx short 3B", "[SYSEX_MESSAGE][USB]")
{
    uint8_t packet[4] = {0x07, 0xF0, 0x10, 0xF7};
    REQUIRE(uparser.parse(packet) == SYSEX_MESSAGE);
    REQUIRE(uparser.getSysExLength() == 3);
    const SysExVector result(uparser.getSysExBuffer(),
                             uparser.getSysExBuffer() + 3);
    const SysExVector expected = {0xF0, 0x10, 0xF7};
    REQUIRE_THAT(result, Equals(expected));
}

TEST_CASE("USB: SysEx long 4B", "[SYSEX_MESSAGE][USB]")
{
    uint8_t packet1[4] = {0x04, 0xF0, 0x11, 0x12};
    REQUIRE(uparser.parse(packet1) == NO_MESSAGE);
    uint8_t packet2[4] = {0x05, 0xF7, 0x00, 0x00};
    REQUIRE(uparser.parse(packet2) == SYSEX_MESSAGE);
    REQUIRE(uparser.getSysExLength() == 4);
    const SysExVector result(uparser.getSysExBuffer(),
                             uparser.getSysExBuffer() + 4);
    const SysExVector expected = {0xF0, 0x11, 0x12, 0xF7};
    REQUIRE_THAT(result, Equals(expected));
}

TEST_CASE("USB: SysEx long 5B", "[SYSEX_MESSAGE][USB]")
{
    uint8_t packet1[4] = {0x04, 0xF0, 0x21, 0x22};
    REQUIRE(uparser.parse(packet1) == NO_MESSAGE);
    uint8_t packet2[4] = {0x06, 0x23, 0xF7, 0x00};
    REQUIRE(uparser.parse(packet2) == SYSEX_MESSAGE);
    REQUIRE(uparser.getSysExLength() == 5);
    const SysExVector result(uparser.getSysExBuffer(),
                             uparser.getSysExBuffer() + 5);
    const SysExVector expected = {0xF0, 0x21, 0x22, 0x23, 0xF7};
    REQUIRE_THAT( result, Equals(expected) );
}

TEST_CASE("USB: SysEx long 6B", "[SYSEX_MESSAGE][USB]")
{
    uint8_t packet1[4] = {0x04, 0xF0, 0x31, 0x32};
    REQUIRE(uparser.parse(packet1) == NO_MESSAGE);
    uint8_t packet2[4] = {0x07, 0x33, 0x34, 0xF7};
    REQUIRE(uparser.parse(packet2) == SYSEX_MESSAGE);
    REQUIRE(uparser.getSysExLength() == 6);
    const SysExVector result(uparser.getSysExBuffer(),
                             uparser.getSysExBuffer() + 6);
    const SysExVector expected = {0xF0, 0x31, 0x32, 0x33, 0x34, 0xF7};
    REQUIRE_THAT( result, Equals(expected) );
}

TEST_CASE("USB: SysEx long 7B", "[SYSEX_MESSAGE][USB]")
{
    uint8_t packet1[4] = {0x04, 0xF0, 0x41, 0x42};
    REQUIRE(uparser.parse(packet1) == NO_MESSAGE);
    uint8_t packet2[4] = {0x04, 0x43, 0x44, 0x45};
    REQUIRE(uparser.parse(packet2) == NO_MESSAGE);
    uint8_t packet3[4] = {0x05, 0xF7, 0x00, 0x00};
    REQUIRE(uparser.parse(packet3) == SYSEX_MESSAGE);
    REQUIRE(uparser.getSysExLength() == 7);
    const SysExVector result(uparser.getSysExBuffer(),
                             uparser.getSysExBuffer() + 7);
    const SysExVector expected = {0xF0, 0x41, 0x42, 0x43, 0x44, 0x45, 0xF7};
    REQUIRE_THAT( result, Equals(expected) );
}

TEST_CASE("USB: SysEx long 12B", "[SYSEX_MESSAGE][USB]")
{
    uint8_t packet1[4] = {0x04, 0xF0, 0x51, 0x52};
    REQUIRE(uparser.parse(packet1) == NO_MESSAGE);
    uint8_t packet2[4] = {0x04, 0x53, 0x54, 0x55};
    REQUIRE(uparser.parse(packet2) == NO_MESSAGE);
    uint8_t packet3[4] = {0x04, 0x56, 0x57, 0x58};
    REQUIRE(uparser.parse(packet3) == NO_MESSAGE);
    uint8_t packet4[4] = {0x07, 0x59, 0x5A, 0xF7};
    REQUIRE(uparser.parse(packet4) == SYSEX_MESSAGE);
    REQUIRE(uparser.getSysExLength() == 12);
    const SysExVector result(uparser.getSysExBuffer(),
                             uparser.getSysExBuffer() + 12);
    const SysExVector expected = {0xF0, 0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A, 0xF7};
    REQUIRE_THAT( result, Equals(expected) );
}

TEST_CASE("USB: SysEx continues without starting", "[SYSEX_MESSAGE][USB]")
{
    uint8_t packet[4] = {0x07, 0x33, 0x34, 0xF7};
    REQUIRE(uparser.parse(packet) == NO_MESSAGE);
}

TEST_CASE("USB: SysEx ends without starting 1B", "[SYSEX_MESSAGE][USB]")
{
    uint8_t packet[4] = {0x05, 0xF7, 0x00, 0x00};
    REQUIRE(uparser.parse(packet) == NO_MESSAGE);
}

TEST_CASE("USB: SysEx ends without starting 2B", "[SYSEX_MESSAGE][USB]")
{
    uint8_t packet[4] = {0x06, 0x10, 0xF7, 0x00};
    REQUIRE(uparser.parse(packet) == NO_MESSAGE);
}

TEST_CASE("USB: SysEx ends without starting 3B", "[SYSEX_MESSAGE][USB]")
{
    uint8_t packet[4] = {0x07, 0x10, 0x11, 0xF7};
    REQUIRE(uparser.parse(packet) == NO_MESSAGE);
}

// --------------------------- SERIAL PARSER TESTS --------------------------- //

SerialMIDI_Parser sparser;

TEST_CASE("Serial: Note Off message", "[CHANNEL_MESSAGE][Serial]")
{
    REQUIRE(sparser.parse(0x82) == NO_MESSAGE);
    REQUIRE(sparser.parse(0x20) == NO_MESSAGE);
    REQUIRE(sparser.parse(0x7F) == CHANNEL_MESSAGE);
    MIDI_message msg = sparser.getChannelMessage();
    REQUIRE(msg.header == 0x82);
    REQUIRE(msg.data1 == 0x20);
    REQUIRE(msg.data2 == 0x7F);
}

TEST_CASE("Serial: Note On message", "[CHANNEL_MESSAGE][Serial]")
{
    REQUIRE(sparser.parse(0x93) == NO_MESSAGE);
    REQUIRE(sparser.parse(0x2A) == NO_MESSAGE);
    REQUIRE(sparser.parse(0x7E) == CHANNEL_MESSAGE);
    MIDI_message msg = sparser.getChannelMessage();
    REQUIRE(msg.header == 0x93);
    REQUIRE(msg.data1 == 0x2A);
    REQUIRE(msg.data2 == 0x7E);
}

TEST_CASE("Serial: Running status Note On", "[CHANNEL_MESSAGE][Serial]")
{
    REQUIRE(sparser.parse(0x9A) == NO_MESSAGE);
    REQUIRE(sparser.parse(0x10) == NO_MESSAGE);
    REQUIRE(sparser.parse(0x11) == CHANNEL_MESSAGE);
    MIDI_message msg1 = sparser.getChannelMessage();
    REQUIRE(msg1.header == 0x9A);
    REQUIRE(msg1.data1 == 0x10);
    REQUIRE(msg1.data2 == 0x11);
    
    REQUIRE(sparser.parse(0x12) == NO_MESSAGE);
    REQUIRE(sparser.parse(0x13) == CHANNEL_MESSAGE);
    MIDI_message msg2 = sparser.getChannelMessage();
    REQUIRE(msg2.header == 0x9A);
    REQUIRE(msg2.data1 == 0x12);
    REQUIRE(msg2.data2 == 0x13);
}

TEST_CASE("Serial: Aftertouch message", "[CHANNEL_MESSAGE][Serial]")
{
    REQUIRE(sparser.parse(0xA1) == NO_MESSAGE);
    REQUIRE(sparser.parse(0x01) == NO_MESSAGE);
    REQUIRE(sparser.parse(0x02) == CHANNEL_MESSAGE);
    MIDI_message msg = sparser.getChannelMessage();
    REQUIRE(msg.header == 0xA1);
    REQUIRE(msg.data1 == 0x01);
    REQUIRE(msg.data2 == 0x02);
}

TEST_CASE("Serial: Control Change message", "[CHANNEL_MESSAGE][Serial]")
{
    REQUIRE(sparser.parse(0xBB) == NO_MESSAGE);
    REQUIRE(sparser.parse(0x03) == NO_MESSAGE);
    REQUIRE(sparser.parse(0x04) == CHANNEL_MESSAGE);
    MIDI_message msg = sparser.getChannelMessage();
    REQUIRE(msg.header == 0xBB);
    REQUIRE(msg.data1 == 0x03);
    REQUIRE(msg.data2 == 0x04);
}

TEST_CASE("Serial: Program Change message", "[CHANNEL_MESSAGE][Serial]")
{
    REQUIRE(sparser.parse(0xC6) == NO_MESSAGE);
    REQUIRE(sparser.parse(0x7A) == CHANNEL_MESSAGE);
    MIDI_message msg = sparser.getChannelMessage();
    REQUIRE(msg.header == 0xC6);
    REQUIRE(msg.data1 == 0x7A);
}

TEST_CASE("Serial: Running status Program Change", "[CHANNEL_MESSAGE][Serial]")
{
    REQUIRE(sparser.parse(0xC6) == NO_MESSAGE);
    REQUIRE(sparser.parse(0x7B) == CHANNEL_MESSAGE);
    MIDI_message msg1 = sparser.getChannelMessage();
    REQUIRE(msg1.header == 0xC6);
    REQUIRE(msg1.data1 == 0x7B);
    
    REQUIRE(sparser.parse(0x7C) == CHANNEL_MESSAGE);
    MIDI_message msg2 = sparser.getChannelMessage();
    REQUIRE(msg2.header == 0xC6);
    REQUIRE(msg2.data1 == 0x7C);
}

TEST_CASE("Serial: Channel Pressure message", "[CHANNEL_MESSAGE][Serial]")
{
    REQUIRE(sparser.parse(0xD7) == NO_MESSAGE);
    REQUIRE(sparser.parse(0x16) == CHANNEL_MESSAGE);
    MIDI_message msg = sparser.getChannelMessage();
    REQUIRE(msg.header == 0xD7);
    REQUIRE(msg.data1 == 0x16);
}

TEST_CASE("Serial: Pitch Bend message", "[CHANNEL_MESSAGE][Serial]")
{
    REQUIRE(sparser.parse(0xE0) == NO_MESSAGE);
    REQUIRE(sparser.parse(0x55) == NO_MESSAGE);
    REQUIRE(sparser.parse(0x66) == CHANNEL_MESSAGE);
    MIDI_message msg = sparser.getChannelMessage();
    REQUIRE(msg.header == 0xE0);
    REQUIRE(msg.data1 == 0x55);
    REQUIRE(msg.data2 == 0x66);
}

TEST_CASE("Serial: SysEx short 2B", "[SYSEX_MESSAGE][Serial]")
{
    REQUIRE(sparser.parse(0xF0) == NO_MESSAGE);
    REQUIRE(sparser.parse(0xF7) == SYSEX_MESSAGE);
    REQUIRE(sparser.getSysExLength() == 2);
    const SysExVector result(sparser.getSysExBuffer(),
                             sparser.getSysExBuffer() + 2);
    const SysExVector expected = {0xF0, 0xF7};
    REQUIRE_THAT( result, Equals(expected) );
}

TEST_CASE("Serial: SysEx short 3B", "[SYSEX_MESSAGE][Serial]")
{
    REQUIRE(sparser.parse(0xF0) == NO_MESSAGE);
    REQUIRE(sparser.parse(0x10) == NO_MESSAGE);
    REQUIRE(sparser.parse(0xF7) == SYSEX_MESSAGE);
    REQUIRE(sparser.getSysExLength() == 3);
    const SysExVector result(sparser.getSysExBuffer(),
                             sparser.getSysExBuffer() + 3);
    const SysExVector expected = {0xF0, 0x10, 0xF7};
    REQUIRE_THAT(result, Equals(expected));
}

TEST_CASE("Serial: SysEx long 7B", "[SYSEX_MESSAGE][Serial]")
{
    REQUIRE(sparser.parse(0xF0) == NO_MESSAGE);
    REQUIRE(sparser.parse(0x41) == NO_MESSAGE);
    REQUIRE(sparser.parse(0x42) == NO_MESSAGE);
    REQUIRE(sparser.parse(0x43) == NO_MESSAGE);
    REQUIRE(sparser.parse(0x44) == NO_MESSAGE);
    REQUIRE(sparser.parse(0x45) == NO_MESSAGE);
    REQUIRE(sparser.parse(0xF7) == SYSEX_MESSAGE);
    REQUIRE(sparser.getSysExLength() == 7);
    const SysExVector result(sparser.getSysExBuffer(),
                             sparser.getSysExBuffer() + 7);
    const SysExVector expected = {0xF0, 0x41, 0x42, 0x43, 0x44, 0x45, 0xF7};
    REQUIRE_THAT( result, Equals(expected) );
}

TEST_CASE("Serial: SysEx ends without starting 1B", "[SYSEX_MESSAGE][Serial]")
{
    REQUIRE(sparser.parse(0xF7) == NO_MESSAGE);
}

TEST_CASE("Serial: SysEx ends without starting 2B", "[SYSEX_MESSAGE][Serial]")
{
    REQUIRE(sparser.parse(0x51) == NO_MESSAGE);
    REQUIRE(sparser.parse(0xF7) == NO_MESSAGE);
}

TEST_CASE("Serial: SysEx ends without starting 3B", "[SYSEX_MESSAGE][Serial]")
{
    REQUIRE(sparser.parse(0x52) == NO_MESSAGE);
    REQUIRE(sparser.parse(0x53) == NO_MESSAGE);
    REQUIRE(sparser.parse(0xF7) == NO_MESSAGE);
}

#endif // ifndef ARDUINO