// g++ -Wall -std=c++11 -o tests -I/home/pieter/lib/Catch2/single_include *.cpp && ./tests

#ifndef ARDUINO

#include <catch.hpp>

#include <iostream>
using namespace std;

#include "../MIDI_Input_Element.h"

TEST_CASE("Matching addresses: tracks per bank = 1", "[MATCH]")
{
    // address 0x10, channel 1, 4 addresses, 1 channel
    MIDI_Input_Element_CC mie(0x10, 1, 4, 1); 
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x0F, 0x40}));
    REQUIRE( mie.update({CONTROL_CHANGE, 0, 0x10, 0x40}));
    REQUIRE( mie.update({CONTROL_CHANGE, 0, 0x11, 0x40}));
    REQUIRE( mie.update({CONTROL_CHANGE, 0, 0x12, 0x40}));
    REQUIRE( mie.update({CONTROL_CHANGE, 0, 0x13, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x14, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 1, 0x10, 0x40}));
}
TEST_CASE("Matching addresses: tracks per bank = 4", "[MATCH]")
{
    // address 0x10, channel 1, 4 addresses, 1 channel
    MIDI_Input_Element_CC mie(0x10, 1, 4, 1); 
    mie.setChannelsPerBank(4);
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x00, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x01, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x02, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x03, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x04, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x05, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x06, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x07, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x08, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x09, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x0A, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x0B, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x0C, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x0D, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x0E, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x0F, 0x40}));
    REQUIRE( mie.update({CONTROL_CHANGE, 0, 0x10, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x11, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x12, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x13, 0x40}));
    REQUIRE( mie.update({CONTROL_CHANGE, 0, 0x14, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x15, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x16, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x17, 0x40}));
    REQUIRE( mie.update({CONTROL_CHANGE, 0, 0x18, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x19, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x1A, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x1B, 0x40}));
    REQUIRE( mie.update({CONTROL_CHANGE, 0, 0x1C, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x1D, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x1E, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x1F, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x20, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x21, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x22, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x23, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x24, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 1, 0x10, 0x40}));
}

TEST_CASE("Matching channels: tracks per bank = 1", "[MATCH]")
{
    // address 0x10, channel 1, 1 address, 4 channels
    MIDI_Input_Element_CC mie(0x10, 2, 1, 4); 
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x00, 0x40}));
    REQUIRE( mie.update({CONTROL_CHANGE, 1, 0x10, 0x40}));
    REQUIRE( mie.update({CONTROL_CHANGE, 2, 0x10, 0x40}));
    REQUIRE( mie.update({CONTROL_CHANGE, 3, 0x10, 0x40}));
    REQUIRE( mie.update({CONTROL_CHANGE, 4, 0x10, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 5, 0x10, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0, 0x11, 0x40}));
}
TEST_CASE("Matching channels: tracks per bank = 3", "[MATCH]")
{
    // address 0x10, channel 1, 1 address, 4 channels
    MIDI_Input_Element_CC mie(0x10, 2, 1, 4); 
    mie.setChannelsPerBank(3);
    REQUIRE(!mie.update({CONTROL_CHANGE, 0x0, 0x10, 0x40}));
    REQUIRE( mie.update({CONTROL_CHANGE, 0x1, 0x10, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0x2, 0x10, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0x3, 0x10, 0x40}));
    REQUIRE( mie.update({CONTROL_CHANGE, 0x4, 0x10, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0x5, 0x10, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0x6, 0x10, 0x40}));
    REQUIRE( mie.update({CONTROL_CHANGE, 0x7, 0x10, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0x8, 0x10, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0x9, 0x10, 0x40}));
    REQUIRE( mie.update({CONTROL_CHANGE, 0xA, 0x10, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0xB, 0x10, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0xC, 0x10, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0xD, 0x10, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0xE, 0x10, 0x40}));
    REQUIRE(!mie.update({CONTROL_CHANGE, 0xF, 0x10, 0x40}));
}
#endif // ifndef ARDUINO
