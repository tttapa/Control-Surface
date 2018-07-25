#include <Banks/Bank.h>
#include <MIDI_Inputs/MCU/TimeDisplay.hpp>
#include <gtest/gtest.h>

using namespace ::testing;

TEST(MCUTimeDisplay, setFirstLetter) {
    constexpr uint8_t channel = 2;
    MCU::TimeDisplay tdisp(channel);
    MIDI_message_matcher midimsg = {CC, channel - 1, 0x40 + 9, 'A' - 0x40};
    tdisp.updateWith(midimsg);
    EXPECT_EQ(tdisp.getCharacterAt(0), 'A');
    for (uint8_t i = 1; i < 10; i++)
        EXPECT_EQ(tdisp.getCharacterAt(i), ' ');
}

TEST(MCUTimeDisplay, getTextFull) {
    constexpr uint8_t channel = 2;
    MCU::TimeDisplay tdisp(channel);
    MIDI_message_matcher midimsg = {CC, channel - 1, 0x40 + 8, 'A' - 0x40};
    tdisp.updateWith(midimsg);
    char text[11];
    tdisp.getText(text);
    EXPECT_STREQ(text, " A        ");
}

TEST(MCUTimeDisplay, getTextOffset) {
    constexpr uint8_t channel = 2;
    MCU::TimeDisplay tdisp(channel);
    MIDI_message_matcher midimsg = {CC, channel - 1, 0x40 + 8, 'A' - 0x40};
    tdisp.updateWith(midimsg);
    char text[10];
    tdisp.getText(text, 1);
    EXPECT_STREQ(text, "A        ");
}

TEST(MCUTimeDisplay, getTextOffsetLength) {
    constexpr uint8_t channel = 2;
    MCU::TimeDisplay tdisp(channel);
    MIDI_message_matcher midimsg = {CC, channel - 1, 0x40 + 8, 'A' - 0x40};
    tdisp.updateWith(midimsg);
    char text[4];
    tdisp.getText(text, 1, 3);
    EXPECT_STREQ(text, "A  ");
}

TEST(MCUTimeDisplay, getTextOffsetLast) {
    constexpr uint8_t channel = 2;
    MCU::TimeDisplay tdisp(channel);
    MIDI_message_matcher midimsg = {CC, channel - 1, 0x40 + 0, 'A' - 0x40};
    tdisp.updateWith(midimsg);
    char text[2];
    tdisp.getText(text, 9);
    EXPECT_STREQ(text, "A");
}

TEST(MCUTimeDisplay, getTextOffsetTooBig) {
    constexpr uint8_t channel = 2;
    MCU::TimeDisplay tdisp(channel);
    MIDI_message_matcher midimsg = {CC, channel - 1, 0x40 + 0, 'A' - 0x40};
    tdisp.updateWith(midimsg);
    char text[2];
    tdisp.getText(text, 10);
    EXPECT_STREQ(text, "A");
}

TEST(MCUTimeDisplay, getTextLengthTooBig) {
    constexpr uint8_t channel = 2;
    MCU::TimeDisplay tdisp(channel);
    MIDI_message_matcher midimsg = {CC, channel - 1, 0x40 + 0, 'B' - 0x40};
    tdisp.updateWith(midimsg);
    char text[11];
    tdisp.getText(text, 0, 100);
    EXPECT_STREQ(text, "         B");
}

TEST(MCUTimeDisplay, getBarsGetBeatsGetFrames) {
    constexpr uint8_t channel = 2;
    MCU::TimeDisplay tdisp(channel);
    MIDI_message_matcher midimsgs[] = {
        {CONTROL_CHANGE, channel - 1, 0x40 + 9, ' '},
        {CONTROL_CHANGE, channel - 1, 0x40 + 8, ' '},
        {CONTROL_CHANGE, channel - 1, 0x40 + 7, '9'},
        {CONTROL_CHANGE, channel - 1, 0x40 + 6, '0'},
        {CONTROL_CHANGE, channel - 1, 0x40 + 5, '3'},
        {CONTROL_CHANGE, channel - 1, 0x40 + 4, ' '},
        {CONTROL_CHANGE, channel - 1, 0x40 + 3, ' '},
        {CONTROL_CHANGE, channel - 1, 0x40 + 2, '5'},
        {CONTROL_CHANGE, channel - 1, 0x40 + 1, '6'},
        {CONTROL_CHANGE, channel - 1, 0x40 + 0, '7'},
    };
    for (auto &midimsg : midimsgs)
        tdisp.updateWith(midimsg);
    char barStr[5], beatStr[3], frameStr[4];
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

TEST(MCUTimeDisplay, getBarsGetBeatsGetFramesSpacesOnly) {
    constexpr uint8_t channel = 2;
    MCU::TimeDisplay tdisp(channel);
    MIDI_message_matcher midimsgs[] = {
        {CONTROL_CHANGE, channel - 1, 0x40 + 9, ' '},
        {CONTROL_CHANGE, channel - 1, 0x40 + 8, ' '},
        {CONTROL_CHANGE, channel - 1, 0x40 + 7, ' '},
        {CONTROL_CHANGE, channel - 1, 0x40 + 6, ' '},
        {CONTROL_CHANGE, channel - 1, 0x40 + 5, ' '},
        {CONTROL_CHANGE, channel - 1, 0x40 + 4, ' '},
        {CONTROL_CHANGE, channel - 1, 0x40 + 3, ' '},
        {CONTROL_CHANGE, channel - 1, 0x40 + 2, ' '},
        {CONTROL_CHANGE, channel - 1, 0x40 + 1, ' '},
        {CONTROL_CHANGE, channel - 1, 0x40 + 0, ' '},
    };
    for (auto &midimsg : midimsgs)
        tdisp.updateWith(midimsg);
    char barStr[5], beatStr[3], frameStr[4];
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