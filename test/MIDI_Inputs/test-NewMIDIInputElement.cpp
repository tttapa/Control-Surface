#include <gtest-wrapper.h>

#include <MIDI_Inputs/LEDs/NoteCCKPLED.hpp>
#include <MIDI_Inputs/MIDIInputElementMatchers.hpp>
#include <MIDI_Inputs/NoteCCKPRange.hpp>
#include <MIDI_Inputs/NoteCCKPValue.hpp>

using namespace CS;

TEST(TwoByteMIDIMatcher, NoteOnNoteOff) {
    struct M : MatchingMIDIInputElement<MIDIMessageType::NOTE_ON,
                                        TwoByteMIDIMatcher> {
        M(MIDIAddress a) : MatchingMIDIInputElement(a) {}
        MOCK_METHOD(void, handleUpdateHelper, (uint8_t));
        void handleUpdate(TwoByteMIDIMatcher::Result m) override {
            handleUpdateHelper(m.value);
        }
    } mn{{0x3C, CHANNEL_5}};
    mn.begin();

    EXPECT_CALL(mn, handleUpdateHelper(0x7E));
    NewMIDIInputElement<MIDIMessageType::NOTE_ON>::updateAllWith(
        {MIDIMessageType::NOTE_ON, CHANNEL_5, 0x3C, 0x7E});
    testing::Mock::VerifyAndClear(&mn);

    EXPECT_CALL(mn, handleUpdateHelper(0x00));
    NewMIDIInputElement<MIDIMessageType::NOTE_ON>::updateAllWith(
        {MIDIMessageType::NOTE_OFF, CHANNEL_5, 0x3C, 0x7E});
    testing::Mock::VerifyAndClear(&mn);
}

TEST(TwoByteMIDIMatcher, ControlChange) {
    struct M : MatchingMIDIInputElement<MIDIMessageType::CONTROL_CHANGE,
                                        TwoByteMIDIMatcher> {
        M(MIDIAddress a) : MatchingMIDIInputElement(a) {}
        MOCK_METHOD(void, handleUpdateHelper, (uint8_t));
        void handleUpdate(TwoByteMIDIMatcher::Result m) override {
            handleUpdateHelper(m.value);
        }
    } mn{{0x0C, CHANNEL_10}};
    mn.begin();

    EXPECT_CALL(mn, handleUpdateHelper(0x40));
    NewMIDIInputElement<MIDIMessageType::CONTROL_CHANGE>::updateAllWith(
        {MIDIMessageType::CONTROL_CHANGE, CHANNEL_10, 0x0C, 0x40});
    testing::Mock::VerifyAndClear(&mn);

    EXPECT_CALL(mn, handleUpdateHelper(0x00));
    NewMIDIInputElement<MIDIMessageType::CONTROL_CHANGE>::updateAllWith(
        {MIDIMessageType::NOTE_OFF, CHANNEL_10, 0x0C, 0x00});
    testing::Mock::VerifyAndClear(&mn);
}

TEST(TwoByteMIDIMatcher, ChannelPressure) {
    struct M : MatchingMIDIInputElement<MIDIMessageType::CHANNEL_PRESSURE,
                                        OneByteMIDIMatcher> {
        M(MIDIChannelCN a) : MatchingMIDIInputElement(a) {}
        MOCK_METHOD(void, handleUpdateHelper, (uint8_t));
        void handleUpdate(OneByteMIDIMatcher::Result m) override {
            handleUpdateHelper(m.value);
        }
    } mn{{CHANNEL_10, CABLE_7}};
    mn.begin();

    EXPECT_CALL(mn, handleUpdateHelper(0x0C));
    NewMIDIInputElement<MIDIMessageType::CHANNEL_PRESSURE>::updateAllWith(
        {MIDIMessageType::CHANNEL_PRESSURE, CHANNEL_10, 0x0C, 0xFF, CABLE_7});
    testing::Mock::VerifyAndClear(&mn);

    EXPECT_CALL(mn, handleUpdateHelper(0x00));
    NewMIDIInputElement<MIDIMessageType::CHANNEL_PRESSURE>::updateAllWith(
        {MIDIMessageType::CHANNEL_PRESSURE, CHANNEL_10, 0x00, 0xFF, CABLE_7});
    testing::Mock::VerifyAndClear(&mn);

    // Wrong channel
    NewMIDIInputElement<MIDIMessageType::CHANNEL_PRESSURE>::updateAllWith(
        {MIDIMessageType::CHANNEL_PRESSURE, CHANNEL_11, 0x00, 0xFF, CABLE_7});
    testing::Mock::VerifyAndClear(&mn);

    // Wrong cable
    NewMIDIInputElement<MIDIMessageType::CHANNEL_PRESSURE>::updateAllWith(
        {MIDIMessageType::CHANNEL_PRESSURE, CHANNEL_10, 0x00, 0xFF, CABLE_8});
    testing::Mock::VerifyAndClear(&mn);
}

TEST(TwoByteRangeMIDIMatcher, ControlChange) {
    struct M : MatchingMIDIInputElement<MIDIMessageType::CONTROL_CHANGE,
                                        TwoByteRangeMIDIMatcher> {
        M(MIDIAddress a) : MatchingMIDIInputElement({a, 8}) {}
        MOCK_METHOD(void, handleUpdateHelper, (uint8_t, uint8_t));
        void handleUpdate(TwoByteRangeMIDIMatcher::Result m) override {
            handleUpdateHelper(m.value, m.index);
        }
    } mn{{0x10, CHANNEL_10}};
    mn.begin();

    NewMIDIInputElement<MIDIMessageType::CONTROL_CHANGE>::updateAllWith(
        {MIDIMessageType::CONTROL_CHANGE, CHANNEL_10, 0x0F, 0x40});
    testing::Mock::VerifyAndClear(&mn);

    EXPECT_CALL(mn, handleUpdateHelper(0x40, 0));
    NewMIDIInputElement<MIDIMessageType::CONTROL_CHANGE>::updateAllWith(
        {MIDIMessageType::CONTROL_CHANGE, CHANNEL_10, 0x10, 0x40});
    testing::Mock::VerifyAndClear(&mn);

    EXPECT_CALL(mn, handleUpdateHelper(0x41, 1));
    NewMIDIInputElement<MIDIMessageType::CONTROL_CHANGE>::updateAllWith(
        {MIDIMessageType::CONTROL_CHANGE, CHANNEL_10, 0x11, 0x41});
    testing::Mock::VerifyAndClear(&mn);

    EXPECT_CALL(mn, handleUpdateHelper(0x42, 7));
    NewMIDIInputElement<MIDIMessageType::CONTROL_CHANGE>::updateAllWith(
        {MIDIMessageType::CONTROL_CHANGE, CHANNEL_10, 0x17, 0x42});
    testing::Mock::VerifyAndClear(&mn);

    NewMIDIInputElement<MIDIMessageType::CONTROL_CHANGE>::updateAllWith(
        {MIDIMessageType::CONTROL_CHANGE, CHANNEL_10, 0x18, 0x43});
    testing::Mock::VerifyAndClear(&mn);
}

// ---- //
// ---- //

TEST(NewNoteValue, NoteOnNoteOff) {
    NewNoteValue mn({0x3C, CHANNEL_5});
    mn.begin();

    EXPECT_EQ(mn.getValue(), 0);
    EXPECT_FALSE(mn.getValue() > 0);

    NewMIDIInputElement<MIDIMessageType::NOTE_ON>::updateAllWith(
        {MIDIMessageType::NOTE_ON, CHANNEL_5, 0x3C, 0x7E});
    EXPECT_EQ(mn.getValue(), 0x7E);
    EXPECT_TRUE(mn.getValue() > 0);

    NewMIDIInputElement<MIDIMessageType::NOTE_ON>::updateAllWith(
        {MIDIMessageType::NOTE_OFF, CHANNEL_5, 0x3C, 0x7E});
    EXPECT_EQ(mn.getValue(), 0);
    EXPECT_FALSE(mn.getValue() > 0);
}

TEST(NewNoteRange, NoteOnNoteOff) {
    NewNoteRange<2> mn{{0x3C, CHANNEL_5}};
    mn.begin();

    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);

    NewMIDIInputElement<MIDIMessageType::NOTE_ON>::updateAllWith(
        {MIDIMessageType::NOTE_ON, CHANNEL_5, 0x3C, 0x7E});

    EXPECT_EQ(mn.getValue(0), 0x7E);
    EXPECT_EQ(mn.getValue(1), 0x00);

    NewMIDIInputElement<MIDIMessageType::NOTE_ON>::updateAllWith(
        {MIDIMessageType::NOTE_ON, CHANNEL_5, 0x3D, 0x7D});

    EXPECT_EQ(mn.getValue(0), 0x7E);
    EXPECT_EQ(mn.getValue(1), 0x7D);

    NewMIDIInputElement<MIDIMessageType::NOTE_ON>::updateAllWith(
        {MIDIMessageType::NOTE_OFF, CHANNEL_5, 0x3C, 0x7E});

    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x7D);
}

// ---- //

TEST(NewNoteRange, bankableInRange) {
    Bank<2> bank(4);
    Bankable::NewNoteRange<2, 3> mn = {bank, {0x10, CHANNEL_5}};
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

TEST(NewNoteRange, bankableNotInRange) {
    Bank<2> bank(4);
    Bankable::NewNoteRange<2, 3> mn = {bank, {0x10, CHANNEL_5}};
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

TEST(NewNoteRange, bankableInRangeChangeChannel) {
    Bank<2> bank(4);
    Bankable::NewNoteRange<2, 3> mn = {{bank, CHANGE_CHANNEL},
                                       {0x10, CHANNEL_5}};
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

TEST(NewNoteRange, bankableNotInRangeChangeChannel) {
    Bank<2> bank(4);
    Bankable::NewNoteRange<2, 3> mn = {{bank, CHANGE_CHANNEL},
                                       {0x10, CHANNEL_5}};
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

// ---- ///

TEST(NewNoteLED, NoteOnNoteOff) {
    NewNoteLED mnl = {2, {0x3C, CHANNEL_5}};

    ::testing::InSequence seq;

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, LOW));
    NewMIDIInputElement<MIDIMessageType::NOTE_ON>::beginAll();

    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, HIGH));
    ChannelMessageMatcher midimsg1 = {
        MIDIMessageType::NOTE_ON,
        CHANNEL_5,
        0x3C,
        0x7E,
    };
    NewMIDIInputElement<MIDIMessageType::NOTE_ON>::updateAllWith(midimsg1);

    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, LOW));
    ChannelMessageMatcher midimsg2 = {
        MIDIMessageType::NOTE_OFF,
        CHANNEL_5,
        0x3C,
        0x7E,
    };
    NewMIDIInputElement<MIDIMessageType::NOTE_ON>::updateAllWith(midimsg2);

    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(BankableNewNoteLED, NoteOnNoteOff) {
    Bank<2> bank;
    Bankable::NewNoteLED<2> mnl = {bank, 2, {0x3C, CHANNEL_5}};

    ::testing::InSequence seq;

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, LOW));
    NewMIDIInputElement<MIDIMessageType::NOTE_ON>::beginAll();

    // Bank 0 ON
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, HIGH));
    ChannelMessageMatcher midimsg1 = {
        MIDIMessageType::NOTE_ON,
        CHANNEL_5,
        0x3C,
        0x7E,
    };
    NewMIDIInputElement<MIDIMessageType::NOTE_ON>::updateAllWith(midimsg1);
    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());

    // Bank 0 OFF
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, LOW));
    ChannelMessageMatcher midimsg2 = {
        MIDIMessageType::NOTE_OFF,
        CHANNEL_5,
        0x3C,
        0x7E,
    };
    NewMIDIInputElement<MIDIMessageType::NOTE_ON>::updateAllWith(midimsg2);
    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());

    // Bank 1 ON
    ChannelMessageMatcher midimsg3 = {
        MIDIMessageType::NOTE_ON,
        CHANNEL_5,
        0x3D,
        0x7E,
    };
    NewMIDIInputElement<MIDIMessageType::NOTE_ON>::updateAllWith(midimsg3);
    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());

    // Switch to Bank 1
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, HIGH));
    bank.select(1);
    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

// TEST(NewNoteLED, PWM) {
//     NewNoteLEDPWM mnl = {2, {0x3C, CHANNEL_5}};

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
// }

// ---- //

// ---- //

#include <MIDI_Inputs/MCU/NewVU.hpp>

using ::testing::Mock;
using ::testing::Return;

TEST(MCUNewVU, setValue) {
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    MCU::NewVU vu = {track, channel};
    ChannelMessageMatcher midimsg = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel,
        (track - 1) << 4 | 0xA,
        0,
    };
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(0));
    NewMIDIInputElement<MIDIMessageType::CHANNEL_PRESSURE>::updateAllWith(
        midimsg);
    EXPECT_EQ(vu.getValue(), 0xA);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(MCUNewVU, setOverload) {
    constexpr Channel channel = CHANNEL_2;
    constexpr uint8_t track = 6;
    MCU::NewVU vu = {track, channel};
    ChannelMessageMatcher midimsg = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel,
        (track - 1) << 4 | 0xE,
        0,
    };
    NewMIDIInputElement<MIDIMessageType::CHANNEL_PRESSURE>::updateAllWith(
        midimsg);
    EXPECT_EQ(vu.getValue(), 0);
    EXPECT_TRUE(vu.getOverload());

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(MCUNewVU, clearOverload) {
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    MCU::NewVU vu = {track, channel};
    ChannelMessageMatcher midimsgSet = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel,
        (track - 1) << 4 | 0xE,
        0,
    };
    NewMIDIInputElement<MIDIMessageType::CHANNEL_PRESSURE>::updateAllWith(
        midimsgSet);
    ChannelMessageMatcher midimsgClr = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel,
        (track - 1) << 4 | 0xF,
        0,
    };
    NewMIDIInputElement<MIDIMessageType::CHANNEL_PRESSURE>::updateAllWith(
        midimsgClr);
    EXPECT_EQ(vu.getValue(), 0);
    EXPECT_FALSE(vu.getOverload());

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(MCUNewVU, retainValueAfterOverload) {
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    MCU::NewVU vu = {track, channel};
    ChannelMessageMatcher midimsg = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel,
        (track - 1) << 4 | 0x6,
        0,
    };
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(0));
    NewMIDIInputElement<MIDIMessageType::CHANNEL_PRESSURE>::updateAllWith(
        midimsg);
    EXPECT_EQ(vu.getValue(), 0x6);
    ChannelMessageMatcher midimsgSet = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel,
        (track - 1) << 4 | 0xE,
        0,
    };
    NewMIDIInputElement<MIDIMessageType::CHANNEL_PRESSURE>::updateAllWith(
        midimsgSet);
    EXPECT_EQ(vu.getValue(), 0x6);
    ChannelMessageMatcher midimsgClr = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel,
        (track - 1) << 4 | 0xF,
        0,
    };
    NewMIDIInputElement<MIDIMessageType::CHANNEL_PRESSURE>::updateAllWith(
        midimsgClr);
    EXPECT_EQ(vu.getValue(), 0x6);
    EXPECT_FALSE(vu.getOverload());

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(MCUNewVU, decay) {
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    constexpr unsigned int decayTime = 300;
    MCU::NewVU vu = {track, channel, decayTime};
    ChannelMessageMatcher midimsg = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel,
        (track - 1) << 4 | 0xA,
        0,
    };
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(0));
    NewMIDIInputElement<MIDIMessageType::CHANNEL_PRESSURE>::updateAllWith(
        midimsg);
    EXPECT_EQ(vu.getValue(), 0xA);
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillOnce(Return(decayTime));
    NewMIDIInputElement<MIDIMessageType::CHANNEL_PRESSURE>::updateAll();
    EXPECT_EQ(vu.getValue(), 0x9);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(MCUNewVU, getFloatValue) {
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    constexpr unsigned int decayTime = 300;
    MCU::NewVU vu = {track, channel, decayTime};
    ChannelMessageMatcher midimsg = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel,
        (track - 1) << 4 | 0xA,
        0,
    };
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(0));
    NewMIDIInputElement<MIDIMessageType::CHANNEL_PRESSURE>::updateAllWith(
        midimsg);
    EXPECT_FLOAT_EQ(vu.getFloatValue(), 10.0f / 12);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(MCUNewVU, reset) {
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    constexpr unsigned int decayTime = 300;
    MCU::NewVU vu = {track, channel, decayTime};
    ChannelMessageMatcher midimsg = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel,
        (track - 1) << 4 | 0xA,
        0,
    };
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(0));
    NewMIDIInputElement<MIDIMessageType::CHANNEL_PRESSURE>::updateAllWith(
        midimsg);
    EXPECT_EQ(vu.getValue(), 0xA);
    vu.reset();
    EXPECT_EQ(vu.getValue(), 0x0);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

// -------------------------------------------------------------------------- //

TEST(MCUNewVUBankable, setValueBankChangeAddress) {
    Bank<2> bank(4);
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    MCU::Bankable::NewVU<2> vu = {bank, track, channel};
    ChannelMessageMatcher midimsg = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel,
        (track + 4 - 1) << 4 | 0xA,
        0,
    };
    // Active bank is never updated, so decay timer is never reset, and millis
    // shouldn't be called.
    NewMIDIInputElement<MIDIMessageType::CHANNEL_PRESSURE>::updateAllWith(
        midimsg);
    EXPECT_EQ(vu.getValue(), 0x0);
    bank.select(1);
    EXPECT_EQ(vu.getValue(), 0xA);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(MCUNewVUBankable, setValueBankChangeChannel) {
    Bank<3> bank(4);
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    MCU::Bankable::NewVU<3> vu = {{bank, CHANGE_CHANNEL}, track, channel};
    ChannelMessageMatcher midimsg1 = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel + 4,
        (track - 1) << 4 | 0xA,
        0,
    };
    ChannelMessageMatcher midimsg2 = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel + 8,
        (track - 1) << 4 | 0xB,
        0,
    };
    // Active bank is never updated, so decay timer is never reset, and millis
    // shouldn't be called.
    NewMIDIInputElement<MIDIMessageType::CHANNEL_PRESSURE>::updateAllWith(
        midimsg1);
    NewMIDIInputElement<MIDIMessageType::CHANNEL_PRESSURE>::updateAllWith(
        midimsg2);
    EXPECT_EQ(vu.getValue(), 0x0);
    bank.select(1);
    EXPECT_EQ(vu.getValue(), 0xA);
    bank.select(2);
    EXPECT_EQ(vu.getValue(), 0xB);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(MCUNewVUBankable, setValueBankChangeCN) {
    Bank<3> bank(4);
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    MCU::Bankable::NewVU<3> vu = {{bank, CHANGE_CABLENB}, track, channel};
    ChannelMessageMatcher midimsg1 = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel,
        (track - 1) << 4 | 0xA,
        0,
        CABLE_5,
    };
    ChannelMessageMatcher midimsg2 = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel,
        (track - 1) << 4 | 0xB,
        0,
        CABLE_9,
    };
    // Active bank is never updated, so decay timer is never reset, and millis
    // shouldn't be called.
    NewMIDIInputElement<MIDIMessageType::CHANNEL_PRESSURE>::updateAllWith(
        midimsg1);
    NewMIDIInputElement<MIDIMessageType::CHANNEL_PRESSURE>::updateAllWith(
        midimsg2);
    EXPECT_EQ(vu.getValue(), 0x0);
    bank.select(1);
    EXPECT_EQ(vu.getValue(), 0xA);
    bank.select(2);
    EXPECT_EQ(vu.getValue(), 0xB);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(MCUNewVUBankable, overloadBankChangeAddress) {
    Bank<2> bank(4);
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    MCU::Bankable::NewVU<2> vu = {bank, track, channel};
    EXPECT_FALSE(vu.getOverload());
    bank.select(1);
    EXPECT_FALSE(vu.getOverload());
    ChannelMessageMatcher midimsgSet = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel,
        (track + 4 - 1) << 4 | 0xE,
        0,
    };
    NewMIDIInputElement<MIDIMessageType::CHANNEL_PRESSURE>::updateAllWith(
        midimsgSet);
    bank.select(0);
    EXPECT_FALSE(vu.getOverload());
    bank.select(1);
    EXPECT_TRUE(vu.getOverload());
    ChannelMessageMatcher midimsgClr = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel,
        (track + 4 - 1) << 4 | 0xF,
        0,
    };
    NewMIDIInputElement<MIDIMessageType::CHANNEL_PRESSURE>::updateAllWith(
        midimsgClr);
    bank.select(0);
    EXPECT_FALSE(vu.getOverload());
    bank.select(1);
    EXPECT_FALSE(vu.getOverload());

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

// // -------------------------------------------------------------------------- //

#include <MIDI_Inputs/LEDs/MCU/NewVULEDs.hpp>

TEST(MCUNewVULEDsBankable, displayOnBankChange) {
    Bank<2> bank(4);
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    constexpr unsigned int decayTime = 150;
    MCU::Bankable::NewVULEDs<2, 2> vu{bank, {0, 1}, track, channel, decayTime};

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(0, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(1, OUTPUT));

    NewMIDIInputElement<MIDIMessageType::CHANNEL_PRESSURE>::beginAll();
    Mock::VerifyAndClear(&ArduinoMock::getInstance());

    ChannelMessageMatcher midimsg1 = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel,
        (track + 0 - 1) << 4 | 0xC,
        0,
    };
    ChannelMessageMatcher midimsg2 = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel,
        (track + 4 - 1) << 4 | 0x6,
        0,
    };
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(0, HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(1, HIGH));
    NewMIDIInputElement<MIDIMessageType::CHANNEL_PRESSURE>::updateAllWith(
        midimsg1);
    Mock::VerifyAndClear(&ArduinoMock::getInstance());

    NewMIDIInputElement<MIDIMessageType::CHANNEL_PRESSURE>::updateAllWith(
        midimsg2);
    Mock::VerifyAndClear(&ArduinoMock::getInstance());

    EXPECT_EQ(vu.getValue(), 0xC);
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(0, HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(1, LOW));
    bank.select(1);
    Mock::VerifyAndClear(&ArduinoMock::getInstance());
    EXPECT_EQ(vu.getValue(), 0x6);
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(0, HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(1, HIGH));
    bank.select(0);
    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}