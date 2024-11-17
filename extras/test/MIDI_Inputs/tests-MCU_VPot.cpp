#include <Banks/Bank.hpp>
#include <MIDI_Inputs/MCU/VPotRing.hpp>
#include <gtest/gtest.h>

using namespace ::testing;
using namespace cs;

// -------------------------------------------------------------------------- //

TEST(MCUVPot, update) {
    constexpr Channel channel = Channel_3;
    constexpr uint8_t track = 5;
    MCU::VPotRing vpot = {track, channel};

    ChannelMessage midimsg = {
        MIDIMessageType::ControlChange,
        channel,
        0x34,
        0x2A,
    };
    MIDIInputElementCC::updateAllWith(midimsg);
    EXPECT_EQ(vpot.getPosition(), 0xA);
    EXPECT_EQ(vpot.getMode(), 0x2);
    EXPECT_EQ(vpot.getCenterLed(), false);

    midimsg = {
        MIDIMessageType::ControlChange,
        channel,
        0x34,
        0x58,
    };
    MIDIInputElementCC::updateAllWith(midimsg);
    EXPECT_EQ(vpot.getPosition(), 0x8);
    EXPECT_EQ(vpot.getMode(), 0x1);
    EXPECT_EQ(vpot.getCenterLed(), true);
}

// -------------------------------------------------------------------------- //

TEST(MCUVPotBankable, setValueBankChangeAddress) {
    Bank<2> bank(4);
    constexpr Channel channel = Channel_3;
    constexpr uint8_t track = 5;
    MCU::Bankable::VPotRing<2> vpot = {bank, track, channel};

    ChannelMessage midimsg = {
        MIDIMessageType::ControlChange,
        channel,
        0x34 + 4,
        0x5A,
    };
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
    constexpr Channel channel = Channel_3;
    constexpr uint8_t track = 5;
    MCU::Bankable::VPotRing<2> vpot = {{bank, ChangeChannel}, track, channel};

    ChannelMessage midimsg = {
        MIDIMessageType::ControlChange,
        channel + 4,
        0x34,
        0x5A,
    };
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
    constexpr Channel channel = Channel_3;
    constexpr uint8_t track = 5;
    MCU::Bankable::VPotRing<2> vpot = {{bank, ChangeCable}, track, channel};

    ChannelMessage midimsg = {
        MIDIMessageType::ControlChange, channel, 0x34, 0x5A, Cable_5,
    };
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
    constexpr Channel channel = Channel_3;
    constexpr uint8_t track = 5;
    MCU::Bankable::VPotRingLEDs<2> vpot {
        bank, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, track, channel};

    for (uint8_t i = 0; i <= 10; ++i) {
        EXPECT_CALL(ArduinoMock::getInstance(), pinMode(i, OUTPUT));
        EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(i, LOW));
    }
    MIDIInputElementCC::beginAll();
    Mock::VerifyAndClear(&ArduinoMock::getInstance());

    ChannelMessage midimsg1 = {
        MIDIMessageType::ControlChange,
        channel,
        0x34 + 4,
        0x59,
    };

    MIDIInputElementCC::updateAllWith(midimsg1);

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

    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(0, LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(1, HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(3, HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(4, HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(5, HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(6, LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(7, LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(8, LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(9, LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(10, LOW));

    ChannelMessage midimsg2 = {
        MIDIMessageType::ControlChange,
        channel,
        0x34 + 4,
        0x12,
    };

    MIDIInputElementCC::updateAllWith(midimsg2);

    EXPECT_EQ(vpot.getPosition(), 0x2);
    EXPECT_EQ(vpot.getMode(), 0x1);
    EXPECT_EQ(vpot.getCenterLed(), false);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());

    // TODO: test center led and more banks, updating active bank, etc.
}