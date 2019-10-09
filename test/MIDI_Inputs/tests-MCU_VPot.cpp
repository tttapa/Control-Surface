#include <Banks/Bank.hpp>
#include <MIDI_Inputs/MCU/VPotRing.hpp>
#include <gtest-wrapper.h>

using namespace ::testing;
using namespace CS;

// -------------------------------------------------------------------------- //

TEST(MCUVPot, update) {
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    MCU::VPotRing vpot = {track, channel};

    ChannelMessageMatcher midimsg = {CONTROL_CHANGE, channel, 0x34, 0x2A};
    MIDIInputElementCC::updateAllWith(midimsg);
    EXPECT_EQ(vpot.getPosition(), 0xA);
    EXPECT_EQ(vpot.getMode(), 0x2);
    EXPECT_EQ(vpot.getCenterLed(), false);

    midimsg = {CONTROL_CHANGE, channel, 0x34, 0x58};
    MIDIInputElementCC::updateAllWith(midimsg);
    EXPECT_EQ(vpot.getPosition(), 0x8);
    EXPECT_EQ(vpot.getMode(), 0x1);
    EXPECT_EQ(vpot.getCenterLed(), true);
}

// -------------------------------------------------------------------------- //

TEST(MCUVPotBankable, setValueBankChangeAddress) {
    Bank<2> bank(4);
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    MCU::Bankable::VPotRing<2> vpot = {bank, track, channel};

    ChannelMessageMatcher midimsg = {CONTROL_CHANGE, channel, 0x34 + 4, 0x5A};
    MIDIInputElementCC::updateAllWith(midimsg);
    EXPECT_EQ(vpot.getPosition(), 0x0);
    EXPECT_EQ(vpot.getMode(), 0x0);
    EXPECT_EQ(vpot.getCenterLed(), false);
    bank.select(1);
    EXPECT_EQ(vpot.getPosition(), 0xA);
    EXPECT_EQ(vpot.getMode(), 0x1);
    EXPECT_EQ(vpot.getCenterLed(), true);
}

TEST(MCUVPotBankable, setValueBankChangeChannel) {
    Bank<2> bank(4);
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    MCU::Bankable::VPotRing<2> vpot = {{bank, CHANGE_CHANNEL}, track, channel};

    ChannelMessageMatcher midimsg = {CONTROL_CHANGE, channel + 4, 0x34, 0x5A};
    MIDIInputElementCC::updateAllWith(midimsg);
    EXPECT_EQ(vpot.getPosition(), 0x0);
    EXPECT_EQ(vpot.getMode(), 0x0);
    EXPECT_EQ(vpot.getCenterLed(), false);
    bank.select(1);
    EXPECT_EQ(vpot.getPosition(), 0xA);
    EXPECT_EQ(vpot.getMode(), 0x1);
    EXPECT_EQ(vpot.getCenterLed(), true);
}


TEST(MCUVPotBankable, setValueBankChangeCN) {
    Bank<2> bank(4);
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    MCU::Bankable::VPotRing<2> vpot = {{bank, CHANGE_CABLENB}, track, channel};

    ChannelMessageMatcher midimsg = {CONTROL_CHANGE, channel, 0x34, 0x5A, 4};
    MIDIInputElementCC::updateAllWith(midimsg);
    EXPECT_EQ(vpot.getPosition(), 0x0);
    EXPECT_EQ(vpot.getMode(), 0x0);
    EXPECT_EQ(vpot.getCenterLed(), false);
    bank.select(1);
    EXPECT_EQ(vpot.getPosition(), 0xA);
    EXPECT_EQ(vpot.getMode(), 0x1);
    EXPECT_EQ(vpot.getCenterLed(), true);
}

// -------------------------------------------------------------------------- //

#if 0

#include <MIDI_Inputs/LEDs/MCU/VULEDs.hpp>

TEST(MCUVULEDsBankable, displayOnBankChange) {
    Bank<2> bank(4);
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    constexpr unsigned int decayTime = 150;
    MCU::Bankable::VULEDs<2, 2> vu{bank, track, channel, decayTime, {{{0, 1}}}};

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

#endif