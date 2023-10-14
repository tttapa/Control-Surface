#include <gtest/gtest.h>

#include <MIDI_Inputs/LEDs/NoteCCKPLED.hpp>
#include <MIDI_Inputs/MIDIInputElementMatchers.hpp>
#include <MIDI_Inputs/NoteCCKPRange.hpp>
#include <MIDI_Inputs/NoteCCKPValue.hpp>

using namespace cs;

TEST(TwoByteMIDIMatcher, NoteOnNoteOff) {
    struct M : MatchingMIDIInputElement<MIDIMessageType::NoteOn,
                                        TwoByteMIDIMatcher> {
        M(MIDIAddress a) : MatchingMIDIInputElement(a) {}
        MOCK_METHOD(void, handleUpdateHelper, (uint8_t));
        void handleUpdate(TwoByteMIDIMatcher::Result m) override {
            handleUpdateHelper(m.value);
        }
    } mn {{0x3C, Channel_5}};
    mn.begin();

    EXPECT_CALL(mn, handleUpdateHelper(0x7E));
    MIDIInputElement<MIDIMessageType::NoteOn>::updateAllWith(
        {MIDIMessageType::NoteOn, Channel_5, 0x3C, 0x7E});
    testing::Mock::VerifyAndClear(&mn);

    EXPECT_CALL(mn, handleUpdateHelper(0x00));
    MIDIInputElement<MIDIMessageType::NoteOn>::updateAllWith(
        {MIDIMessageType::NoteOff, Channel_5, 0x3C, 0x7E});
    testing::Mock::VerifyAndClear(&mn);
}

TEST(TwoByteMIDIMatcher, ControlChange) {
    struct M : MatchingMIDIInputElement<MIDIMessageType::ControlChange,
                                        TwoByteMIDIMatcher> {
        M(MIDIAddress a) : MatchingMIDIInputElement(a) {}
        MOCK_METHOD(void, handleUpdateHelper, (uint8_t));
        void handleUpdate(TwoByteMIDIMatcher::Result m) override {
            handleUpdateHelper(m.value);
        }
    } mn {{0x0C, Channel_10}};
    mn.begin();

    EXPECT_CALL(mn, handleUpdateHelper(0x40));
    MIDIInputElement<MIDIMessageType::ControlChange>::updateAllWith(
        {MIDIMessageType::ControlChange, Channel_10, 0x0C, 0x40});
    testing::Mock::VerifyAndClear(&mn);

    EXPECT_CALL(mn, handleUpdateHelper(0x00));
    MIDIInputElement<MIDIMessageType::ControlChange>::updateAllWith(
        {MIDIMessageType::NoteOff, Channel_10, 0x0C, 0x00});
    testing::Mock::VerifyAndClear(&mn);
}

TEST(TwoByteMIDIMatcher, ChannelPressure) {
    struct M : MatchingMIDIInputElement<MIDIMessageType::ChannelPressure,
                                        OneByteMIDIMatcher> {
        M(MIDIChannelCable a) : MatchingMIDIInputElement(a) {}
        MOCK_METHOD(void, handleUpdateHelper, (uint8_t));
        void handleUpdate(OneByteMIDIMatcher::Result m) override {
            handleUpdateHelper(m.value);
        }
    } mn {{Channel_10, Cable_7}};
    mn.begin();

    EXPECT_CALL(mn, handleUpdateHelper(0x0C));
    MIDIInputElement<MIDIMessageType::ChannelPressure>::updateAllWith(
        {MIDIMessageType::ChannelPressure, Channel_10, 0x0C, 0xFF, Cable_7});
    testing::Mock::VerifyAndClear(&mn);

    EXPECT_CALL(mn, handleUpdateHelper(0x00));
    MIDIInputElement<MIDIMessageType::ChannelPressure>::updateAllWith(
        {MIDIMessageType::ChannelPressure, Channel_10, 0x00, 0xFF, Cable_7});
    testing::Mock::VerifyAndClear(&mn);

    // Wrong channel
    MIDIInputElement<MIDIMessageType::ChannelPressure>::updateAllWith(
        {MIDIMessageType::ChannelPressure, Channel_11, 0x00, 0xFF, Cable_7});
    testing::Mock::VerifyAndClear(&mn);

    // Wrong cable
    MIDIInputElement<MIDIMessageType::ChannelPressure>::updateAllWith(
        {MIDIMessageType::ChannelPressure, Channel_10, 0x00, 0xFF, Cable_8});
    testing::Mock::VerifyAndClear(&mn);
}

TEST(TwoByteRangeMIDIMatcher, ControlChange) {
    struct M : MatchingMIDIInputElement<MIDIMessageType::ControlChange,
                                        TwoByteRangeMIDIMatcher> {
        M(MIDIAddress a) : MatchingMIDIInputElement({a, 8}) {}
        MOCK_METHOD(void, handleUpdateHelper, (uint8_t, uint8_t));
        void handleUpdate(TwoByteRangeMIDIMatcher::Result m) override {
            handleUpdateHelper(m.value, m.index);
        }
    } mn {{0x10, Channel_10}};
    mn.begin();

    MIDIInputElement<MIDIMessageType::ControlChange>::updateAllWith(
        {MIDIMessageType::ControlChange, Channel_10, 0x0F, 0x40});
    testing::Mock::VerifyAndClear(&mn);

    EXPECT_CALL(mn, handleUpdateHelper(0x40, 0));
    MIDIInputElement<MIDIMessageType::ControlChange>::updateAllWith(
        {MIDIMessageType::ControlChange, Channel_10, 0x10, 0x40});
    testing::Mock::VerifyAndClear(&mn);

    EXPECT_CALL(mn, handleUpdateHelper(0x41, 1));
    MIDIInputElement<MIDIMessageType::ControlChange>::updateAllWith(
        {MIDIMessageType::ControlChange, Channel_10, 0x11, 0x41});
    testing::Mock::VerifyAndClear(&mn);

    EXPECT_CALL(mn, handleUpdateHelper(0x42, 7));
    MIDIInputElement<MIDIMessageType::ControlChange>::updateAllWith(
        {MIDIMessageType::ControlChange, Channel_10, 0x17, 0x42});
    testing::Mock::VerifyAndClear(&mn);

    MIDIInputElement<MIDIMessageType::ControlChange>::updateAllWith(
        {MIDIMessageType::ControlChange, Channel_10, 0x18, 0x43});
    testing::Mock::VerifyAndClear(&mn);
}
