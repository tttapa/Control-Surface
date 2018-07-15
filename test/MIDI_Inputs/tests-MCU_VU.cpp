#include <MIDI_Inputs/MCU_VU.hpp>
#include <gtest/gtest.h>

using namespace ::testing;

TEST(MCU_VU, setValue) {
    constexpr uint8_t channel = 3;
    constexpr uint8_t track = 5;
    MCU_VU vu(track, channel);
    MIDI_message_matcher midimsg = {CHANNEL_PRESSURE, channel - 1,
                                    (track - 1) << 4 | 0xA, 0};
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillOnce(Return(0));
    MIDI_Input_ChannelPressure::updateAllWith(midimsg);
    EXPECT_EQ(vu.getValue(), 0xA);
}

TEST(MCU_VU, setOverload) {
    constexpr uint8_t channel = 3;
    constexpr uint8_t track = 5;
    MCU_VU vu(track, channel);
    MIDI_message_matcher midimsg = {CHANNEL_PRESSURE, channel - 1,
                                    (track - 1) << 4 | 0xE, 0};
    MIDI_Input_ChannelPressure::updateAllWith(midimsg);
    EXPECT_EQ(vu.getValue(), 0);
    EXPECT_TRUE(vu.getOverload());
}

TEST(MCU_VU, clearOverload) {
    constexpr uint8_t channel = 3;
    constexpr uint8_t track = 5;
    MCU_VU vu(track, channel);
    MIDI_message_matcher midimsgSet = {CHANNEL_PRESSURE, channel - 1,
                                    (track - 1) << 4 | 0xE, 0};
    MIDI_Input_ChannelPressure::updateAllWith(midimsgSet);
    MIDI_message_matcher midimsgClr = {CHANNEL_PRESSURE, channel - 1,
                                    (track - 1) << 4 | 0xF, 0};
    MIDI_Input_ChannelPressure::updateAllWith(midimsgClr);
    EXPECT_EQ(vu.getValue(), 0);
    EXPECT_FALSE(vu.getOverload());
}

TEST(MCU_VU, retainValueAfterOverload) {
    constexpr uint8_t channel = 3;
    constexpr uint8_t track = 5;
    MCU_VU vu(track, channel);
    MIDI_message_matcher midimsg = {CHANNEL_PRESSURE, channel - 1,
                                    (track - 1) << 4 | 0x6, 0};
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillOnce(Return(0));
    MIDI_Input_ChannelPressure::updateAllWith(midimsg);
    EXPECT_EQ(vu.getValue(), 0x6);
    MIDI_message_matcher midimsgSet = {CHANNEL_PRESSURE, channel - 1,
                                    (track - 1) << 4 | 0xE, 0};
    MIDI_Input_ChannelPressure::updateAllWith(midimsgSet);
    EXPECT_EQ(vu.getValue(), 0x6);
    MIDI_message_matcher midimsgClr = {CHANNEL_PRESSURE, channel - 1,
                                    (track - 1) << 4 | 0xF, 0};
    MIDI_Input_ChannelPressure::updateAllWith(midimsgClr);
    EXPECT_EQ(vu.getValue(), 0x6);
    EXPECT_FALSE(vu.getOverload());
}