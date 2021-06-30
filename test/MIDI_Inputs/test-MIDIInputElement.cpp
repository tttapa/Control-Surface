#include <gtest/gtest.h>

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
    MIDIInputElement<MIDIMessageType::NOTE_ON>::updateAllWith(
        {MIDIMessageType::NOTE_ON, CHANNEL_5, 0x3C, 0x7E});
    testing::Mock::VerifyAndClear(&mn);

    EXPECT_CALL(mn, handleUpdateHelper(0x00));
    MIDIInputElement<MIDIMessageType::NOTE_ON>::updateAllWith(
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
    MIDIInputElement<MIDIMessageType::CONTROL_CHANGE>::updateAllWith(
        {MIDIMessageType::CONTROL_CHANGE, CHANNEL_10, 0x0C, 0x40});
    testing::Mock::VerifyAndClear(&mn);

    EXPECT_CALL(mn, handleUpdateHelper(0x00));
    MIDIInputElement<MIDIMessageType::CONTROL_CHANGE>::updateAllWith(
        {MIDIMessageType::NOTE_OFF, CHANNEL_10, 0x0C, 0x00});
    testing::Mock::VerifyAndClear(&mn);
}

TEST(TwoByteMIDIMatcher, ChannelPressure) {
    struct M : MatchingMIDIInputElement<MIDIMessageType::CHANNEL_PRESSURE,
                                        OneByteMIDIMatcher> {
        M(MIDIChannelCable a) : MatchingMIDIInputElement(a) {}
        MOCK_METHOD(void, handleUpdateHelper, (uint8_t));
        void handleUpdate(OneByteMIDIMatcher::Result m) override {
            handleUpdateHelper(m.value);
        }
    } mn{{CHANNEL_10, CABLE_7}};
    mn.begin();

    EXPECT_CALL(mn, handleUpdateHelper(0x0C));
    MIDIInputElement<MIDIMessageType::CHANNEL_PRESSURE>::updateAllWith(
        {MIDIMessageType::CHANNEL_PRESSURE, CHANNEL_10, 0x0C, 0xFF, CABLE_7});
    testing::Mock::VerifyAndClear(&mn);

    EXPECT_CALL(mn, handleUpdateHelper(0x00));
    MIDIInputElement<MIDIMessageType::CHANNEL_PRESSURE>::updateAllWith(
        {MIDIMessageType::CHANNEL_PRESSURE, CHANNEL_10, 0x00, 0xFF, CABLE_7});
    testing::Mock::VerifyAndClear(&mn);

    // Wrong channel
    MIDIInputElement<MIDIMessageType::CHANNEL_PRESSURE>::updateAllWith(
        {MIDIMessageType::CHANNEL_PRESSURE, CHANNEL_11, 0x00, 0xFF, CABLE_7});
    testing::Mock::VerifyAndClear(&mn);

    // Wrong cable
    MIDIInputElement<MIDIMessageType::CHANNEL_PRESSURE>::updateAllWith(
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

    MIDIInputElement<MIDIMessageType::CONTROL_CHANGE>::updateAllWith(
        {MIDIMessageType::CONTROL_CHANGE, CHANNEL_10, 0x0F, 0x40});
    testing::Mock::VerifyAndClear(&mn);

    EXPECT_CALL(mn, handleUpdateHelper(0x40, 0));
    MIDIInputElement<MIDIMessageType::CONTROL_CHANGE>::updateAllWith(
        {MIDIMessageType::CONTROL_CHANGE, CHANNEL_10, 0x10, 0x40});
    testing::Mock::VerifyAndClear(&mn);

    EXPECT_CALL(mn, handleUpdateHelper(0x41, 1));
    MIDIInputElement<MIDIMessageType::CONTROL_CHANGE>::updateAllWith(
        {MIDIMessageType::CONTROL_CHANGE, CHANNEL_10, 0x11, 0x41});
    testing::Mock::VerifyAndClear(&mn);

    EXPECT_CALL(mn, handleUpdateHelper(0x42, 7));
    MIDIInputElement<MIDIMessageType::CONTROL_CHANGE>::updateAllWith(
        {MIDIMessageType::CONTROL_CHANGE, CHANNEL_10, 0x17, 0x42});
    testing::Mock::VerifyAndClear(&mn);

    MIDIInputElement<MIDIMessageType::CONTROL_CHANGE>::updateAllWith(
        {MIDIMessageType::CONTROL_CHANGE, CHANNEL_10, 0x18, 0x43});
    testing::Mock::VerifyAndClear(&mn);
}
