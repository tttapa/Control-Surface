#include <gtest-wrapper.h>

#include <MIDI_Inputs/MIDINote.hpp>

TEST(MIDINote, NoteOnNoteOff) {
    MIDINote mn = {{0x3C, CHANNEL_5}};

    EXPECT_EQ(mn.getValue(), 0);
    EXPECT_FALSE(mn.getState());

    MIDI_message_matcher midimsg1 = {0x90, CHANNEL_5, 0x3C, 0x7E};
    MIDIInputElementNote::updateAllWith(midimsg1);

    EXPECT_EQ(mn.getValue(), 0x7E);
    EXPECT_TRUE(mn.getState());

    MIDI_message_matcher midimsg2 = {0x80, CHANNEL_5, 0x3C, 0x7E};
    MIDIInputElementNote::updateAllWith(midimsg2);

    EXPECT_EQ(mn.getValue(), 0);
    EXPECT_FALSE(mn.getState());
}

TEST(MIDINote, NoteOnNoteOnZeroVelocity) {
    MIDINote mn = {{0x3C, CHANNEL_5}};

    MIDI_message_matcher midimsg1 = {0x90, CHANNEL_5, 0x3C, 0x7E};
    MIDIInputElementNote::updateAllWith(midimsg1);

    MIDI_message_matcher midimsg2 = {0x90, CHANNEL_5, 0x3C, 0x00};
    MIDIInputElementNote::updateAllWith(midimsg2);

    EXPECT_EQ(mn.getValue(), 0);
    EXPECT_FALSE(mn.getState());
}

TEST(MIDINote, reset) {
    MIDINote mn = {{0x3C, CHANNEL_5}};

    MIDI_message_matcher midimsg1 = {0x90, CHANNEL_5, 0x3C, 0x7E};
    MIDIInputElementNote::updateAllWith(midimsg1);

    mn.reset();

    EXPECT_EQ(mn.getValue(), 0);
    EXPECT_FALSE(mn.getState());
}

TEST(MIDINote, resetAll) {
    MIDINote mn = {{0x3C, CHANNEL_5}};

    MIDI_message_matcher midimsg1 = {0x90, CHANNEL_5, 0x3C, 0x7E};
    MIDIInputElementNote::updateAllWith(midimsg1);

    MIDIInputElementNote::resetAll();

    EXPECT_EQ(mn.getValue(), 0);
    EXPECT_FALSE(mn.getState());
}

#include <MIDI_Inputs/LEDs/MIDINoteLED.hpp>

TEST(MIDINoteLED, NoteOnNoteOff) {
    MIDINoteLED mnl = {2, {0x3C, CHANNEL_5}};

    ::testing::Sequence seq;

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, OUTPUT)).InSequence(seq);
    MIDIInputElementNote::beginAll();

    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, HIGH))
        .InSequence(seq);
    MIDI_message_matcher midimsg1 = {0x90, CHANNEL_5, 0x3C, 0x7E};
    MIDIInputElementNote::updateAllWith(midimsg1);

    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, LOW))
        .InSequence(seq);
    MIDI_message_matcher midimsg2 = {0x80, CHANNEL_5, 0x3C, 0x7E};
    MIDIInputElementNote::updateAllWith(midimsg2);

    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
}