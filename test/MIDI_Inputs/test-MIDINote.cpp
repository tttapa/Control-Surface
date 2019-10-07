#include <gtest-wrapper.h>

#include <MIDI_Inputs/MIDINoteCCRange.hpp>

using namespace CS;

TEST(NoteValue, NoteOnNoteOff) {
    NoteValue mn = {{0x3C, CHANNEL_5}};

    EXPECT_EQ(mn.getValue(), 0);
    EXPECT_FALSE(mn.getValue() > 0);

    ChannelMessageMatcher midimsg1 = {0x90, CHANNEL_5, 0x3C, 0x7E};
    MIDIInputElementNote::updateAllWith(midimsg1);

    EXPECT_EQ(mn.getValue(), 0x7E);
    EXPECT_TRUE(mn.getValue() > 0);

    ChannelMessageMatcher midimsg2 = {0x80, CHANNEL_5, 0x3C, 0x7E};
    MIDIInputElementNote::updateAllWith(midimsg2);

    EXPECT_EQ(mn.getValue(), 0);
    EXPECT_FALSE(mn.getValue() > 0);
}

TEST(NoteValue, NoteOnNoteOnZeroVelocity) {
    NoteValue mn = {{0x3C, CHANNEL_5}};

    ChannelMessageMatcher midimsg1 = {0x90, CHANNEL_5, 0x3C, 0x7E};
    MIDIInputElementNote::updateAllWith(midimsg1);

    ChannelMessageMatcher midimsg2 = {0x90, CHANNEL_5, 0x3C, 0x00};
    MIDIInputElementNote::updateAllWith(midimsg2);

    EXPECT_EQ(mn.getValue(), 0);
    EXPECT_FALSE(mn.getValue() > 0);
}

TEST(NoteValue, reset) {
    NoteValue mn = {{0x3C, CHANNEL_5}};

    ChannelMessageMatcher midimsg1 = {0x90, CHANNEL_5, 0x3C, 0x7E};
    MIDIInputElementNote::updateAllWith(midimsg1);

    mn.reset();

    EXPECT_EQ(mn.getValue(), 0);
    EXPECT_FALSE(mn.getValue() > 0);
}

TEST(NoteValue, resetAll) {
    NoteValue mn = {{0x3C, CHANNEL_5}};

    ChannelMessageMatcher midimsg1 = {0x90, CHANNEL_5, 0x3C, 0x7E};
    MIDIInputElementNote::updateAllWith(midimsg1);

    MIDIInputElementNote::resetAll();

    EXPECT_EQ(mn.getValue(), 0);
    EXPECT_FALSE(mn.getValue() > 0);
}

#include <MIDI_Inputs/LEDs/CCNoteRangeLEDs.hpp>

TEST(NoteValueLED, NoteOnNoteOff) {
    NoteValueLED mnl = {2, {0x3C, CHANNEL_5}};

    ::testing::Sequence seq;

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, OUTPUT)).InSequence(seq);
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, LOW))
        .InSequence(seq);
    MIDIInputElementNote::beginAll();

    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, HIGH))
        .InSequence(seq);
    ChannelMessageMatcher midimsg1 = {0x90, CHANNEL_5, 0x3C, 0x7E};
    MIDIInputElementNote::updateAllWith(midimsg1);

    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, LOW))
        .InSequence(seq);
    ChannelMessageMatcher midimsg2 = {0x80, CHANNEL_5, 0x3C, 0x7E};
    MIDIInputElementNote::updateAllWith(midimsg2);

    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
}