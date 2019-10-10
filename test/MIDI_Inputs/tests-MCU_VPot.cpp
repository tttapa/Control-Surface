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

#include <MIDI_Inputs/LEDs/MCU/VPotRingLEDs.hpp>

TEST(MCUVPotLEDsBankable, displayOnBankChange) {
    Bank<2> bank(4);
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    MCU::Bankable::VPotRingLEDs<2> vpot{
        bank, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, track, channel};

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(0, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(1, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(5, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(6, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(7, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(8, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(9, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(10, OUTPUT));

    MIDIInputElementCC::beginAll();
    Mock::VerifyAndClear(&ArduinoMock::getInstance());

    ChannelMessageMatcher midimsg = {CONTROL_CHANGE, channel, 0x34 + 4, 0x59};

    MIDIInputElementCC::updateAllWith(midimsg);

    EXPECT_EQ(vpot.getPosition(), 0x0);
    EXPECT_EQ(vpot.getMode(), 0x0);
    EXPECT_EQ(vpot.getCenterLed(), false);

    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(0, LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(1, LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(3, LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(4, LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(5, HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(6, HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(7, HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(8, HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(9, LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(10, LOW));

    bank.select(1);

    EXPECT_EQ(vpot.getPosition(), 0x9);
    EXPECT_EQ(vpot.getMode(), 0x1);
    EXPECT_EQ(vpot.getCenterLed(), true);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());

    // TODO: test center led and more banks, updating active bank, etc.
}