#include <MIDI_Inputs/ChannelMessageMatcher.hpp>
#include <gtest-wrapper.h>

using namespace ::testing;
using namespace CS;

TEST(ChannelMessageMatcher, ChannelMessageMatcher) {
    ChannelMessageMatcher a = {
        MIDIMessageType::CONTROL_CHANGE, CHANNEL_2, 0x49, 0x30, CABLE_10,
    };
    ChannelMessage chmsg = {
        MIDIMessageType::CONTROL_CHANGE, CHANNEL_2, 0x49, 0x30, CABLE_10,
    };
    ChannelMessageMatcher b = chmsg;

    EXPECT_EQ(a, b);
}