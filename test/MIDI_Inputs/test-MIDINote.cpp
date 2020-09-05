#include <gtest-wrapper.h>

#include <MIDI_Inputs/NoteCCKPRange.hpp>
#include <MIDI_Inputs/NoteCCKPValue.hpp>

using namespace CS;

TEST(NoteValue, NoteOnNoteOff) {
    NoteValue mn({0x3C, CHANNEL_5});
    mn.begin();

    EXPECT_EQ(mn.getValue(), 0);
    EXPECT_FALSE(mn.getValue() > 0);

    MIDIInputElementNote::updateAllWith(
        {MIDIMessageType::NOTE_ON, CHANNEL_5, 0x3C, 0x7E});
    EXPECT_EQ(mn.getValue(), 0x7E);
    EXPECT_TRUE(mn.getValue() > 0);

    MIDIInputElementNote::updateAllWith(
        {MIDIMessageType::NOTE_OFF, CHANNEL_5, 0x3C, 0x7E});
    EXPECT_EQ(mn.getValue(), 0);
    EXPECT_FALSE(mn.getValue() > 0);
}

TEST(NoteRange, NoteOnNoteOff) {
    NoteRange<2> mn{{0x3C, CHANNEL_5}};
    mn.begin();

    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);

    MIDIInputElementNote::updateAllWith(
        {MIDIMessageType::NOTE_ON, CHANNEL_5, 0x3C, 0x7E});

    EXPECT_EQ(mn.getValue(0), 0x7E);
    EXPECT_EQ(mn.getValue(1), 0x00);

    MIDIInputElementNote::updateAllWith(
        {MIDIMessageType::NOTE_ON, CHANNEL_5, 0x3D, 0x7D});

    EXPECT_EQ(mn.getValue(0), 0x7E);
    EXPECT_EQ(mn.getValue(1), 0x7D);

    MIDIInputElementNote::updateAllWith(
        {MIDIMessageType::NOTE_OFF, CHANNEL_5, 0x3C, 0x7E});

    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x7D);
}

// ---- //

TEST(NoteRange, bankableInRange) {
    Bank<2> bank(4);
    Bankable::NoteRange<2, 3> mn = {bank, {0x10, CHANNEL_5}};
    mn.begin();

    // First bank, first address
    ChannelMessageMatcher midimsg1 = {
        MIDIMessageType::NOTE_ON,
        CHANNEL_5,
        0x10,
        0x20,
    };
    EXPECT_TRUE(mn.updateWith(midimsg1));
    EXPECT_EQ(mn.getValue(0), 0x20);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);

    // First bank, third address
    ChannelMessageMatcher midimsg2 = {
        MIDIMessageType::NOTE_ON,
        CHANNEL_5,
        0x10 + 2,
        0x21,
    };
    EXPECT_TRUE(mn.updateWith(midimsg2));
    EXPECT_EQ(mn.getValue(0), 0x20);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x21);

    // Second bank, second address
    ChannelMessageMatcher midimsg3 = {
        MIDIMessageType::NOTE_ON,
        CHANNEL_5,
        0x10 + 4 + 1,
        0x22,
    };
    EXPECT_TRUE(mn.updateWith(midimsg3));
    EXPECT_EQ(mn.getValue(0), 0x20);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x21);
    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x22);
    EXPECT_EQ(mn.getValue(2), 0x00);

    // First bank, first address
    ChannelMessageMatcher midimsg4 = {
        MIDIMessageType::NOTE_ON,
        CHANNEL_5,
        0x10,
        0x23,
    };
    EXPECT_TRUE(mn.updateWith(midimsg4));
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x22);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(0);
    EXPECT_EQ(mn.getValue(0), 0x23);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x21);
}

TEST(NoteRange, bankableNotInRange) {
    Bank<2> bank(4);
    Bankable::NoteRange<2, 3> mn = {bank, {0x10, CHANNEL_5}};
    mn.begin();

    // Before first bank
    ChannelMessageMatcher midimsg1 = {
        MIDIMessageType::NOTE_ON,
        CHANNEL_5,
        0x10 - 1,
        0x20,
    };
    EXPECT_FALSE(mn.updateWith(midimsg1));
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(0);

    // First bank, fourth address
    ChannelMessageMatcher midimsg2 = {
        MIDIMessageType::NOTE_ON,
        CHANNEL_5,
        0x10 + 3,
        0x20,
    };
    EXPECT_FALSE(mn.updateWith(midimsg2));
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(0);

    // Second bank, fourth address
    ChannelMessageMatcher midimsg3 = {MIDIMessageType::NOTE_ON, CHANNEL_5,
                                      0x10 + 4 + 3, 0x20};
    EXPECT_FALSE(mn.updateWith(midimsg3));
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(0);

    // Before first bank, first address
    ChannelMessageMatcher midimsg4 = {
        MIDIMessageType::NOTE_ON,
        CHANNEL_5,
        0x10 - 4,
        0x20,
    };
    EXPECT_FALSE(mn.updateWith(midimsg4));
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(0);

    // After last bank, first address
    ChannelMessageMatcher midimsg5 = {
        MIDIMessageType::NOTE_ON,
        CHANNEL_5,
        0x10 + 8,
        0x20,
    };
    EXPECT_FALSE(mn.updateWith(midimsg5));
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(0);

    // Different channel
    ChannelMessageMatcher midimsg6 = {
        MIDIMessageType::NOTE_ON,
        CHANNEL_5 + 1,
        0x10,
        0x20,
    };
    EXPECT_FALSE(mn.updateWith(midimsg6));
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(0);
}

TEST(NoteRange, bankableInRangeChangeChannel) {
    Bank<2> bank(4);
    Bankable::NoteRange<2, 3> mn = {{bank, CHANGE_CHANNEL}, {0x10, CHANNEL_5}};
    mn.begin();

    // First bank, first address
    ChannelMessageMatcher midimsg1 = {
        MIDIMessageType::NOTE_ON,
        CHANNEL_5,
        0x10,
        0x20,
    };
    EXPECT_TRUE(mn.updateWith(midimsg1));
    EXPECT_EQ(mn.getValue(0), 0x20);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);

    // First bank, third address
    ChannelMessageMatcher midimsg2 = {
        MIDIMessageType::NOTE_ON,
        CHANNEL_5,
        0x10 + 2,
        0x21,
    };
    EXPECT_TRUE(mn.updateWith(midimsg2));
    EXPECT_EQ(mn.getValue(0), 0x20);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x21);

    // Second bank, second address
    ChannelMessageMatcher midimsg3 = {
        MIDIMessageType::NOTE_ON,
        CHANNEL_5 + 4,
        0x10 + 1,
        0x22,
    };
    EXPECT_TRUE(mn.updateWith(midimsg3));
    EXPECT_EQ(mn.getValue(0), 0x20);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x21);
    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x22);
    EXPECT_EQ(mn.getValue(2), 0x00);

    // First bank, first address
    ChannelMessageMatcher midimsg4 = {
        MIDIMessageType::NOTE_ON,
        CHANNEL_5,
        0x10,
        0x23,
    };
    EXPECT_TRUE(mn.updateWith(midimsg4));
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x22);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(0);
    EXPECT_EQ(mn.getValue(0), 0x23);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x21);
}

TEST(NoteRange, bankableNotInRangeChangeChannel) {
    Bank<2> bank(4);
    Bankable::NoteRange<2, 3> mn = {{bank, CHANGE_CHANNEL}, {0x10, CHANNEL_5}};
    mn.begin();

    // Before first bank
    ChannelMessageMatcher midimsg1 = {
        MIDIMessageType::NOTE_ON,
        CHANNEL_5,
        0x10 - 1,
        0x20,
    };
    EXPECT_FALSE(mn.updateWith(midimsg1));
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(0);

    // First bank, fourth address
    ChannelMessageMatcher midimsg2 = {
        MIDIMessageType::NOTE_ON,
        CHANNEL_5,
        0x10 + 3,
        0x20,
    };
    EXPECT_FALSE(mn.updateWith(midimsg2));
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(0);

    // Before second bank
    ChannelMessageMatcher midimsg3 = {
        MIDIMessageType::NOTE_ON,
        CHANNEL_5 + 4,
        0x10 - 1,
        0x20,
    };
    EXPECT_FALSE(mn.updateWith(midimsg3));
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(0);

    // Second bank, fourth address
    ChannelMessageMatcher midimsg4 = {
        MIDIMessageType::NOTE_ON,
        CHANNEL_5 + 4,
        0x10 + 3,
        0x20,
    };
    EXPECT_FALSE(mn.updateWith(midimsg4));
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(0);

    // Before first bank, first address
    ChannelMessageMatcher midimsg5 = {
        MIDIMessageType::NOTE_ON,
        CHANNEL_5 - 4,
        0x10,
        0x20,
    };
    EXPECT_FALSE(mn.updateWith(midimsg5));
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(0);

    // After last bank, first address
    ChannelMessageMatcher midimsg6 = {
        MIDIMessageType::NOTE_ON,
        CHANNEL_5 + 8,
        0x10,
        0x20,
    };
    EXPECT_FALSE(mn.updateWith(midimsg6));
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(0);
}

// -------------------------------------------------------------------------- //

#include <MIDI_Inputs/LEDs/NoteCCKPLED.hpp>

TEST(NoteLED, NoteOnNoteOff) {
    NoteLED mnl = {2, {0x3C, CHANNEL_5}};

    ::testing::InSequence seq;

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, LOW));
    MIDIInputElementNote::beginAll();

    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, HIGH));
    ChannelMessageMatcher midimsg1 = {
        MIDIMessageType::NOTE_ON,
        CHANNEL_5,
        0x3C,
        0x7E,
    };
    MIDIInputElementNote::updateAllWith(midimsg1);

    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, LOW));
    ChannelMessageMatcher midimsg2 = {
        MIDIMessageType::NOTE_OFF,
        CHANNEL_5,
        0x3C,
        0x7E,
    };
    MIDIInputElementNote::updateAllWith(midimsg2);

    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(BankableNoteLED, NoteOnNoteOff) {
    Bank<2> bank;
    Bankable::NoteLED<2> mnl = {bank, 2, {0x3C, CHANNEL_5}};

    ::testing::InSequence seq;

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, LOW));
    MIDIInputElementNote::beginAll();

    // Bank 0 ON
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, HIGH));
    ChannelMessageMatcher midimsg1 = {
        MIDIMessageType::NOTE_ON,
        CHANNEL_5,
        0x3C,
        0x7E,
    };
    MIDIInputElementNote::updateAllWith(midimsg1);
    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());

    // Bank 0 OFF
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, LOW));
    ChannelMessageMatcher midimsg2 = {
        MIDIMessageType::NOTE_OFF,
        CHANNEL_5,
        0x3C,
        0x7E,
    };
    MIDIInputElementNote::updateAllWith(midimsg2);
    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());

    // Bank 1 ON
    ChannelMessageMatcher midimsg3 = {
        MIDIMessageType::NOTE_ON,
        CHANNEL_5,
        0x3D,
        0x7E,
    };
    MIDIInputElementNote::updateAllWith(midimsg3);
    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());

    // Switch to Bank 1
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, HIGH));
    bank.select(1);
    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

/// @todo TODO
TEST(NoteLED, PWM) {
    //     NoteLEDPWM mnl = {2, {0x3C, CHANNEL_5}};

    //     ::testing::InSequence seq;

    //     EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, OUTPUT));
    //     EXPECT_CALL(ArduinoMock::getInstance(), analogWrite(2, 0));
    //     MIDIInputElementNote::beginAll();

    //     EXPECT_CALL(ArduinoMock::getInstance(), analogWrite(2, 255));
    //     ChannelMessageMatcher midimsg1 = {
    //         MIDIMessageType::NOTE_ON,
    //         CHANNEL_5,
    //         0x3C,
    //         0x7F,
    //     };
    //     MIDIInputElementNote::updateAllWith(midimsg1);

    //     EXPECT_CALL(ArduinoMock::getInstance(), analogWrite(2, 16));
    //     ChannelMessageMatcher midimsg2 = {
    //         MIDIMessageType::NOTE_ON,
    //         CHANNEL_5,
    //         0x3C,
    //         0x08,
    //     };
    //     MIDIInputElementNote::updateAllWith(midimsg2);

    //     EXPECT_CALL(ArduinoMock::getInstance(), analogWrite(2, 0));
    //     ChannelMessageMatcher midimsg3 = {
    //         MIDIMessageType::NOTE_OFF,
    //         CHANNEL_5,
    //         0x3C,
    //         0x7F,
    //     };
    //     MIDIInputElementNote::updateAllWith(midimsg3);

    //     ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
}