#include <Banks/Bank.hpp>
#include <MIDI_Inputs/MCU/TimeDisplay.hpp>
#include <gtest-wrapper.h>

using namespace ::testing;
using namespace CS;

TEST(MCUTimeDisplay, setFirstLetter) {
    constexpr Channel channel = CHANNEL_2;
    MCU::TimeDisplay tdisp(channel);
    ChannelMessageMatcher midimsg = {CC, channel, 0x40 + 9, 'A' - 0x40};
    tdisp.updateWith(midimsg);
    EXPECT_EQ(tdisp.getCharacterAt(0), 'A');
    for (uint8_t i = 1; i < 10; i++)
        EXPECT_EQ(tdisp.getCharacterAt(i), ' ');
}

TEST(MCUTimeDisplay, getTextFull) {
    constexpr Channel channel = CHANNEL_2;
    MCU::TimeDisplay tdisp(channel);
    ChannelMessageMatcher midimsg = {CC, channel, 0x40 + 8, 'A' - 0x40};
    tdisp.updateWith(midimsg);
    char text[11];
    tdisp.getText(text);
    EXPECT_STREQ(text, " A        ");
}

TEST(MCUTimeDisplay, getTextOffset) {
    constexpr Channel channel = CHANNEL_2;
    MCU::TimeDisplay tdisp(channel);
    ChannelMessageMatcher midimsg = {CC, channel, 0x40 + 8, 'A' - 0x40};
    tdisp.updateWith(midimsg);
    char text[10];
    tdisp.getText(text, 1);
    EXPECT_STREQ(text, "A        ");
}

TEST(MCUTimeDisplay, getTextOffsetLength) {
    constexpr Channel channel = CHANNEL_2;
    MCU::TimeDisplay tdisp(channel);
    ChannelMessageMatcher midimsg = {CC, channel, 0x40 + 8, 'A' - 0x40};
    tdisp.updateWith(midimsg);
    char text[4];
    tdisp.getText(text, 1, 3);
    EXPECT_STREQ(text, "A  ");
}

TEST(MCUTimeDisplay, getTextOffsetLast) {
    constexpr Channel channel = CHANNEL_2;
    MCU::TimeDisplay tdisp(channel);
    ChannelMessageMatcher midimsg = {CC, channel, 0x40 + 0, 'A' - 0x40};
    tdisp.updateWith(midimsg);
    char text[2];
    tdisp.getText(text, 9);
    EXPECT_STREQ(text, "A");
}

TEST(MCUTimeDisplay, getTextOffsetTooBig) {
    constexpr Channel channel = CHANNEL_2;
    MCU::TimeDisplay tdisp(channel);
    ChannelMessageMatcher midimsg = {CC, channel, 0x40 + 0, 'A' - 0x40};
    tdisp.updateWith(midimsg);
    char text[2];
    tdisp.getText(text, 10);
    EXPECT_STREQ(text, "A");
}

TEST(MCUTimeDisplay, getTextLengthTooBig) {
    constexpr Channel channel = CHANNEL_2;
    MCU::TimeDisplay tdisp(channel);
    ChannelMessageMatcher midimsg = {CC, channel, 0x40 + 0, 'B' - 0x40};
    tdisp.updateWith(midimsg);
    char text[11];
    tdisp.getText(text, 0, 100);
    EXPECT_STREQ(text, "         B");
}

TEST(MCUTimeDisplay, getBarsGetBeatsGetFrames1Digit) {
    constexpr Channel channel = CHANNEL_2;
    MCU::TimeDisplay tdisp(channel);
    ChannelMessageMatcher midimsgs[] = {
        {CONTROL_CHANGE, channel, 0x40 + 9, ' '},
        {CONTROL_CHANGE, channel, 0x40 + 8, ' '},
        {CONTROL_CHANGE, channel, 0x40 + 7, '9'},
        {CONTROL_CHANGE, channel, 0x40 + 6, '0'},
        {CONTROL_CHANGE, channel, 0x40 + 5, '3'},
        {CONTROL_CHANGE, channel, 0x40 + 4, ' '},
        {CONTROL_CHANGE, channel, 0x40 + 3, ' '},
        {CONTROL_CHANGE, channel, 0x40 + 2, '5'},
        {CONTROL_CHANGE, channel, 0x40 + 1, '6'},
        {CONTROL_CHANGE, channel, 0x40 + 0, '7'},
    };
    for (auto &midimsg : midimsgs)
        tdisp.updateWith(midimsg);
    char barStr[6], beatStr[3], frameStr[4];
    tdisp.getBars(barStr);
    tdisp.getBeats(beatStr);
    tdisp.getFrames(frameStr);
    char text[11];
    tdisp.getText(text, 0, 10);
    EXPECT_STREQ(barStr, "  9");
    EXPECT_STREQ(beatStr, "03");
    EXPECT_STREQ(frameStr, "567");
    EXPECT_STREQ(text, "  903  567");
}

TEST(MCUTimeDisplay, getBarsGetBeatsGetFrames2Digits) {
    constexpr Channel channel = CHANNEL_2;
    MCU::TimeDisplay tdisp(channel);
    ChannelMessageMatcher midimsgs[] = {
        {CONTROL_CHANGE, channel, 0x40 + 9, ' '},
        {CONTROL_CHANGE, channel, 0x40 + 8, '8'},
        {CONTROL_CHANGE, channel, 0x40 + 7, '9'},
        {CONTROL_CHANGE, channel, 0x40 + 6, '0'},
        {CONTROL_CHANGE, channel, 0x40 + 5, '3'},
        {CONTROL_CHANGE, channel, 0x40 + 4, ' '},
        {CONTROL_CHANGE, channel, 0x40 + 3, ' '},
        {CONTROL_CHANGE, channel, 0x40 + 2, '5'},
        {CONTROL_CHANGE, channel, 0x40 + 1, '6'},
        {CONTROL_CHANGE, channel, 0x40 + 0, '7'},
    };
    for (auto &midimsg : midimsgs)
        tdisp.updateWith(midimsg);
    char barStr[6], beatStr[3], frameStr[4];
    tdisp.getBars(barStr);
    tdisp.getBeats(beatStr);
    tdisp.getFrames(frameStr);
    char text[11];
    tdisp.getText(text, 0, 10);
    EXPECT_STREQ(barStr, " 89");
    EXPECT_STREQ(beatStr, "03");
    EXPECT_STREQ(frameStr, "567");
    EXPECT_STREQ(text, " 8903  567");
}

TEST(MCUTimeDisplay, getBarsGetBeatsGetFrames3Digits) {
    constexpr Channel channel = CHANNEL_2;
    MCU::TimeDisplay tdisp(channel);
    ChannelMessageMatcher midimsgs[] = {
        {CONTROL_CHANGE, channel, 0x40 + 9, '1'},
        {CONTROL_CHANGE, channel, 0x40 + 8, '8'},
        {CONTROL_CHANGE, channel, 0x40 + 7, '9'},
        {CONTROL_CHANGE, channel, 0x40 + 6, '0'},
        {CONTROL_CHANGE, channel, 0x40 + 5, '3'},
        {CONTROL_CHANGE, channel, 0x40 + 4, ' '},
        {CONTROL_CHANGE, channel, 0x40 + 3, ' '},
        {CONTROL_CHANGE, channel, 0x40 + 2, '5'},
        {CONTROL_CHANGE, channel, 0x40 + 1, '6'},
        {CONTROL_CHANGE, channel, 0x40 + 0, '7'},
    };
    for (auto &midimsg : midimsgs)
        tdisp.updateWith(midimsg);
    char barStr[6], beatStr[3], frameStr[4];
    tdisp.getBars(barStr);
    tdisp.getBeats(beatStr);
    tdisp.getFrames(frameStr);
    char text[11];
    tdisp.getText(text, 0, 10);
    EXPECT_STREQ(barStr, "189");
    EXPECT_STREQ(beatStr, "03");
    EXPECT_STREQ(frameStr, "567");
    EXPECT_STREQ(text, "18903  567");
}

TEST(MCUTimeDisplay, getBarsGetBeatsGetFrames4Digits) {
    constexpr Channel channel = CHANNEL_2;
    MCU::TimeDisplay tdisp(channel);
    ChannelMessageMatcher midimsgs[] = {
        {CONTROL_CHANGE, channel, 0x40 + 9, '2'},
        {CONTROL_CHANGE, channel, 0x40 + 8, '5'},
        {CONTROL_CHANGE, channel, 0x40 + 7, '9'},
        {CONTROL_CHANGE, channel, 0x40 + 6, '3'},
        {CONTROL_CHANGE, channel, 0x40 + 5, '0'},
        {CONTROL_CHANGE, channel, 0x40 + 4, '1'},
        {CONTROL_CHANGE, channel, 0x40 + 3, ' '},
        {CONTROL_CHANGE, channel, 0x40 + 2, ' '},
        {CONTROL_CHANGE, channel, 0x40 + 1, '7'},
        {CONTROL_CHANGE, channel, 0x40 + 0, '6'},
    };
    for (auto &midimsg : midimsgs)
        tdisp.updateWith(midimsg);
    char barStr[6], beatStr[3], frameStr[4];
    tdisp.getBars(barStr);
    tdisp.getBeats(beatStr);
    tdisp.getFrames(frameStr);
    char text[11];
    tdisp.getText(text, 0, 10);
    EXPECT_STREQ(barStr, "2593");
    EXPECT_STREQ(beatStr, "01");
    EXPECT_STREQ(frameStr, "76");
    EXPECT_STREQ(text, "259301  76");
}

TEST(MCUTimeDisplay, getBarsGetBeatsGetFrames5Digits) {
    constexpr Channel channel = CHANNEL_2;
    MCU::TimeDisplay tdisp(channel);
    ChannelMessageMatcher midimsgs[] = {
        {CONTROL_CHANGE, channel, 0x40 + 9, '4'},
        {CONTROL_CHANGE, channel, 0x40 + 8, '5'},
        {CONTROL_CHANGE, channel, 0x40 + 7, '8'},
        {CONTROL_CHANGE, channel, 0x40 + 6, '2'},
        {CONTROL_CHANGE, channel, 0x40 + 5, '6'},
        {CONTROL_CHANGE, channel, 0x40 + 4, '0'},
        {CONTROL_CHANGE, channel, 0x40 + 3, '2'},
        {CONTROL_CHANGE, channel, 0x40 + 2, ' '},
        {CONTROL_CHANGE, channel, 0x40 + 1, ' '},
        {CONTROL_CHANGE, channel, 0x40 + 0, '9'},
    };
    for (auto &midimsg : midimsgs)
        tdisp.updateWith(midimsg);
    char barStr[6], beatStr[3], frameStr[4];
    tdisp.getBars(barStr);
    tdisp.getBeats(beatStr);
    tdisp.getFrames(frameStr);
    char text[11];
    tdisp.getText(text, 0, 10);
    EXPECT_STREQ(barStr, "45826");
    EXPECT_STREQ(beatStr, "02");
    EXPECT_STREQ(frameStr, "9");
    EXPECT_STREQ(text, "4582602  9");
}

TEST(MCUTimeDisplay, getBarsGetBeatsGetFramesSpacesOnly) {
    constexpr Channel channel = CHANNEL_2;
    MCU::TimeDisplay tdisp(channel);
    ChannelMessageMatcher midimsgs[] = {
        {CONTROL_CHANGE, channel, 0x40 + 9, ' '},
        {CONTROL_CHANGE, channel, 0x40 + 8, ' '},
        {CONTROL_CHANGE, channel, 0x40 + 7, ' '},
        {CONTROL_CHANGE, channel, 0x40 + 6, ' '},
        {CONTROL_CHANGE, channel, 0x40 + 5, ' '},
        {CONTROL_CHANGE, channel, 0x40 + 4, ' '},
        {CONTROL_CHANGE, channel, 0x40 + 3, ' '},
        {CONTROL_CHANGE, channel, 0x40 + 2, ' '},
        {CONTROL_CHANGE, channel, 0x40 + 1, ' '},
        {CONTROL_CHANGE, channel, 0x40 + 0, ' '},
    };
    for (auto &midimsg : midimsgs)
        tdisp.updateWith(midimsg);
    char barStr[6], beatStr[3], frameStr[4];
    tdisp.getBars(barStr);
    tdisp.getBeats(beatStr);
    tdisp.getFrames(frameStr);
    char text[11];
    tdisp.getText(text, 0, 10);
    EXPECT_STREQ(barStr, "   ");
    EXPECT_STREQ(beatStr, "  ");
    EXPECT_STREQ(frameStr, "   ");
    EXPECT_STREQ(text, "          ");
}