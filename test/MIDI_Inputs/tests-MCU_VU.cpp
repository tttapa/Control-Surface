#include <Banks/Bank.hpp>
#include <MIDI_Inputs/MCU/VU.hpp>
#include <gtest-wrapper.h>

using namespace ::testing;
using namespace CS;

// -------------------------------------------------------------------------- //

TEST(MCUVU, setValue) {
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    MCU::VU vu = {track, channel};
    ChannelMessageMatcher midimsg = {CHANNEL_PRESSURE, channel,
                                     (track - 1) << 4 | 0xA, 0};
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(0));
    MIDIInputElementChannelPressure::updateAllWith(midimsg);
    EXPECT_EQ(vu.getValue(), 0xA);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(MCUVU, setOverload) {
    constexpr Channel channel = CHANNEL_2;
    constexpr uint8_t track = 6;
    MCU::VU vu = {track, channel};
    ChannelMessageMatcher midimsg = {CHANNEL_PRESSURE, channel,
                                     (track - 1) << 4 | 0xE, 0};
    MIDIInputElementChannelPressure::updateAllWith(midimsg);
    EXPECT_EQ(vu.getValue(), 0);
    EXPECT_TRUE(vu.getOverload());

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(MCUVU, clearOverload) {
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    MCU::VU vu = {track, channel};
    ChannelMessageMatcher midimsgSet = {CHANNEL_PRESSURE, channel,
                                        (track - 1) << 4 | 0xE, 0};
    MIDIInputElementChannelPressure::updateAllWith(midimsgSet);
    ChannelMessageMatcher midimsgClr = {CHANNEL_PRESSURE, channel,
                                        (track - 1) << 4 | 0xF, 0};
    MIDIInputElementChannelPressure::updateAllWith(midimsgClr);
    EXPECT_EQ(vu.getValue(), 0);
    EXPECT_FALSE(vu.getOverload());

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(MCUVU, retainValueAfterOverload) {
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    MCU::VU vu = {track, channel};
    ChannelMessageMatcher midimsg = {CHANNEL_PRESSURE, channel,
                                     (track - 1) << 4 | 0x6, 0};
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(0));
    MIDIInputElementChannelPressure::updateAllWith(midimsg);
    EXPECT_EQ(vu.getValue(), 0x6);
    ChannelMessageMatcher midimsgSet = {CHANNEL_PRESSURE, channel,
                                        (track - 1) << 4 | 0xE, 0};
    MIDIInputElementChannelPressure::updateAllWith(midimsgSet);
    EXPECT_EQ(vu.getValue(), 0x6);
    ChannelMessageMatcher midimsgClr = {CHANNEL_PRESSURE, channel,
                                        (track - 1) << 4 | 0xF, 0};
    MIDIInputElementChannelPressure::updateAllWith(midimsgClr);
    EXPECT_EQ(vu.getValue(), 0x6);
    EXPECT_FALSE(vu.getOverload());

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(MCUVU, decay) {
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    constexpr unsigned int decayTime = 300;
    MCU::VU vu = {track, channel, decayTime};
    ChannelMessageMatcher midimsg = {CHANNEL_PRESSURE, channel,
                                     (track - 1) << 4 | 0xA, 0};
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(0));
    MIDIInputElementChannelPressure::updateAllWith(midimsg);
    EXPECT_EQ(vu.getValue(), 0xA);
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillOnce(Return(decayTime));
    MIDIInputElementChannelPressure::updateAll();
    EXPECT_EQ(vu.getValue(), 0x9);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(MCUVU, getFloatValue) {
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    constexpr unsigned int decayTime = 300;
    MCU::VU vu = {track, channel, decayTime};
    ChannelMessageMatcher midimsg = {CHANNEL_PRESSURE, channel,
                                     (track - 1) << 4 | 0xA, 0};
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(0));
    MIDIInputElementChannelPressure::updateAllWith(midimsg);
    EXPECT_FLOAT_EQ(vu.getFloatValue(), 10.0f / 12);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(MCUVU, reset) {
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    constexpr unsigned int decayTime = 300;
    MCU::VU vu = {track, channel, decayTime};
    ChannelMessageMatcher midimsg = {CHANNEL_PRESSURE, channel,
                                     (track - 1) << 4 | 0xA, 0};
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(0));
    MIDIInputElementChannelPressure::updateAllWith(midimsg);
    EXPECT_EQ(vu.getValue(), 0xA);
    vu.reset();
    EXPECT_EQ(vu.getValue(), 0x0);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

// -------------------------------------------------------------------------- //

TEST(MCUVUBankable, setValueBankChangeAddress) {
    Bank<2> bank(4);
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    MCU::Bankable::VU<2> vu = {bank, track, channel};
    ChannelMessageMatcher midimsg = {CHANNEL_PRESSURE, channel,
                                     (track + 4 - 1) << 4 | 0xA, 0};
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(0));
    MIDIInputElementChannelPressure::updateAllWith(midimsg);
    EXPECT_EQ(vu.getValue(), 0x0);
    bank.select(1);
    EXPECT_EQ(vu.getValue(), 0xA);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(MCUVUBankable, setValueBankChangeChannel) {
    Bank<3> bank(4);
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    MCU::Bankable::VU<3> vu = {{bank, CHANGE_CHANNEL}, track, channel};
    ChannelMessageMatcher midimsg1 = {CHANNEL_PRESSURE, channel + 4,
                                      (track - 1) << 4 | 0xA, 0};
    ChannelMessageMatcher midimsg2 = {CHANNEL_PRESSURE, channel + 8,
                                      (track - 1) << 4 | 0xB, 0};
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillOnce(Return(0))
        .WillOnce(Return(0));
    MIDIInputElementChannelPressure::updateAllWith(midimsg1);
    MIDIInputElementChannelPressure::updateAllWith(midimsg2);
    EXPECT_EQ(vu.getValue(), 0x0);
    bank.select(1);
    EXPECT_EQ(vu.getValue(), 0xA);
    bank.select(2);
    EXPECT_EQ(vu.getValue(), 0xB);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(MCUVUBankable, setValueBankChangeCN) {
    Bank<3> bank(4);
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    MCU::Bankable::VU<3> vu = {{bank, CHANGE_CABLENB}, track, channel};
    ChannelMessageMatcher midimsg1 = {CHANNEL_PRESSURE, channel,
                                      (track - 1) << 4 | 0xA, 0, 4};
    ChannelMessageMatcher midimsg2 = {CHANNEL_PRESSURE, channel,
                                      (track - 1) << 4 | 0xB, 0, 8};
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillOnce(Return(0))
        .WillOnce(Return(0));
    MIDIInputElementChannelPressure::updateAllWith(midimsg1);
    MIDIInputElementChannelPressure::updateAllWith(midimsg2);
    EXPECT_EQ(vu.getValue(), 0x0);
    bank.select(1);
    EXPECT_EQ(vu.getValue(), 0xA);
    bank.select(2);
    EXPECT_EQ(vu.getValue(), 0xB);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(MCUVUBankable, overloadBankChangeAddress) {
    Bank<2> bank(4);
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    MCU::Bankable::VU<2> vu = {bank, track, channel};
    EXPECT_FALSE(vu.getOverload());
    bank.select(1);
    EXPECT_FALSE(vu.getOverload());
    ChannelMessageMatcher midimsgSet = {CHANNEL_PRESSURE, channel,
                                        (track + 4 - 1) << 4 | 0xE, 0};
    MIDIInputElementChannelPressure::updateAllWith(midimsgSet);
    bank.select(0);
    EXPECT_FALSE(vu.getOverload());
    bank.select(1);
    EXPECT_TRUE(vu.getOverload());
    ChannelMessageMatcher midimsgClr = {CHANNEL_PRESSURE, channel,
                                        (track + 4 - 1) << 4 | 0xF, 0};
    MIDIInputElementChannelPressure::updateAllWith(midimsgClr);
    bank.select(0);
    EXPECT_FALSE(vu.getOverload());
    bank.select(1);
    EXPECT_FALSE(vu.getOverload());

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

// -------------------------------------------------------------------------- //

#include <MIDI_Inputs/LEDs/MCU/VULEDs.hpp>

TEST(MCUVULEDsBankable, displayOnBankChange) {
    Bank<2> bank(4);
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    constexpr unsigned int decayTime = 150;
    MCU::Bankable::VULEDs<2, 2> vu{bank, {0, 1}, track, channel, decayTime};

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(0, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(1, OUTPUT));

    MIDIInputElementChannelPressure::beginAll();
    Mock::VerifyAndClear(&ArduinoMock::getInstance());

    ChannelMessageMatcher midimsg1 = {CHANNEL_PRESSURE, channel,
                                      (track + 0 - 1) << 4 | 0xC, 0};
    ChannelMessageMatcher midimsg2 = {CHANNEL_PRESSURE, channel,
                                      (track + 4 - 1) << 4 | 0x6, 0};
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(0, HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(1, HIGH));
    MIDIInputElementChannelPressure::updateAllWith(midimsg1);
    Mock::VerifyAndClear(&ArduinoMock::getInstance());

    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(0, HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(1, HIGH));
    MIDIInputElementChannelPressure::updateAllWith(midimsg2);
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