#include <Banks/Bank.hpp>
#include <MIDI_Inputs/MCU/VU.hpp>
#include <gtest/gtest.h>

using ::testing::Mock;
using ::testing::Return;

USING_CS_NAMESPACE;

TEST(MCUVU, setValue) {
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    MCU::VU vu = {track, channel};
    ChannelMessage midimsg = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel,
        (track - 1) << 4 | 0xA,
        0,
    };
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(0));
    MIDIInputElementCP::updateAllWith(midimsg);
    EXPECT_EQ(vu.getValue(), 0xA);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(MCUVU, setOverload) {
    constexpr Channel channel = CHANNEL_2;
    constexpr uint8_t track = 6;
    MCU::VU vu = {track, channel};
    ChannelMessage midimsg = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel,
        (track - 1) << 4 | 0xE,
        0,
    };
    MIDIInputElementCP::updateAllWith(midimsg);
    EXPECT_EQ(vu.getValue(), 0);
    EXPECT_TRUE(vu.getOverload());

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(MCUVU, clearOverload) {
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    MCU::VU vu = {track, channel};
    ChannelMessage midimsgSet = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel,
        (track - 1) << 4 | 0xE,
        0,
    };
    MIDIInputElementCP::updateAllWith(midimsgSet);
    ChannelMessage midimsgClr = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel,
        (track - 1) << 4 | 0xF,
        0,
    };
    MIDIInputElementCP::updateAllWith(midimsgClr);
    EXPECT_EQ(vu.getValue(), 0);
    EXPECT_FALSE(vu.getOverload());

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(MCUVU, retainValueAfterOverload) {
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    MCU::VU vu = {track, channel};
    ChannelMessage midimsg = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel,
        (track - 1) << 4 | 0x6,
        0,
    };
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(0));
    MIDIInputElementCP::updateAllWith(midimsg);
    EXPECT_EQ(vu.getValue(), 0x6);
    ChannelMessage midimsgSet = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel,
        (track - 1) << 4 | 0xE,
        0,
    };
    MIDIInputElementCP::updateAllWith(midimsgSet);
    EXPECT_EQ(vu.getValue(), 0x6);
    ChannelMessage midimsgClr = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel,
        (track - 1) << 4 | 0xF,
        0,
    };
    MIDIInputElementCP::updateAllWith(midimsgClr);
    EXPECT_EQ(vu.getValue(), 0x6);
    EXPECT_FALSE(vu.getOverload());

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(MCUVU, decay) {
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    constexpr unsigned int decayTime = 300;
    MCU::VU vu = {track, channel, decayTime};
    ChannelMessage midimsg = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel,
        (track - 1) << 4 | 0xA,
        0,
    };
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(0));
    MIDIInputElementCP::updateAllWith(midimsg);
    EXPECT_EQ(vu.getValue(), 0xA);
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .WillOnce(Return(decayTime));
    MIDIInputElementCP::updateAll();
    EXPECT_EQ(vu.getValue(), 0x9);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(MCUVU, getFloatValue) {
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    constexpr unsigned int decayTime = 300;
    MCU::VU vu = {track, channel, decayTime};
    ChannelMessage midimsg = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel,
        (track - 1) << 4 | 0xA,
        0,
    };
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(0));
    MIDIInputElementCP::updateAllWith(midimsg);
    EXPECT_FLOAT_EQ(vu.getFloatValue(), 10.0f / 12);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(MCUVU, reset) {
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    constexpr unsigned int decayTime = 300;
    MCU::VU vu = {track, channel, decayTime};
    ChannelMessage midimsg = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel,
        (track - 1) << 4 | 0xA,
        0,
    };
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(0));
    MIDIInputElementCP::updateAllWith(midimsg);
    EXPECT_EQ(vu.getValue(), 0xA);
    MIDIInputElementCP::resetAll();
    EXPECT_EQ(vu.getValue(), 0x0);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

// -------------------------------------------------------------------------- //

TEST(MCUVUBankable, setValueBankChangeAddress) {
    Bank<2> bank(4);
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    MCU::Bankable::VU<2> vu = {bank, track, channel};
    ChannelMessage midimsg = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel,
        (track + 4 - 1) << 4 | 0xA,
        0,
    };
    // Active bank is never updated, so decay timer is never reset, and millis
    // shouldn't be called.
    MIDIInputElementCP::updateAllWith(midimsg);
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
    ChannelMessage midimsg1 = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel + 4,
        (track - 1) << 4 | 0xA,
        0,
    };
    ChannelMessage midimsg2 = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel + 8,
        (track - 1) << 4 | 0xB,
        0,
    };
    // Active bank is never updated, so decay timer is never reset, and millis
    // shouldn't be called.
    MIDIInputElementCP::updateAllWith(midimsg1);
    MIDIInputElementCP::updateAllWith(midimsg2);
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
    ChannelMessage midimsg1 = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel,
        (track - 1) << 4 | 0xA,
        0,
        CABLE_5,
    };
    ChannelMessage midimsg2 = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel,
        (track - 1) << 4 | 0xB,
        0,
        CABLE_9,
    };
    // Active bank is never updated, so decay timer is never reset, and millis
    // shouldn't be called.
    MIDIInputElementCP::updateAllWith(midimsg1);
    MIDIInputElementCP::updateAllWith(midimsg2);
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
    ChannelMessage midimsgSet = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel,
        (track + 4 - 1) << 4 | 0xE,
        0,
    };
    MIDIInputElementCP::updateAllWith(midimsgSet);
    bank.select(0);
    EXPECT_FALSE(vu.getOverload());
    bank.select(1);
    EXPECT_TRUE(vu.getOverload());
    ChannelMessage midimsgClr = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel,
        (track + 4 - 1) << 4 | 0xF,
        0,
    };
    MIDIInputElementCP::updateAllWith(midimsgClr);
    bank.select(0);
    EXPECT_FALSE(vu.getOverload());
    bank.select(1);
    EXPECT_FALSE(vu.getOverload());

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

// // -------------------------------------------------------------------------- //

#include <MIDI_Inputs/LEDs/MCU/VULEDs.hpp>

TEST(MCUVULEDsBankable, displayOnBankChange) {
    Bank<2> bank(4);
    constexpr Channel channel = CHANNEL_3;
    constexpr uint8_t track = 5;
    constexpr unsigned int decayTime = 150;
    MCU::Bankable::VULEDs<2, 2> vu{bank, {0, 1}, track, channel, decayTime};

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(0, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(1, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(0, LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(1, LOW));

    MIDIInputElementCP::beginAll();
    Mock::VerifyAndClear(&ArduinoMock::getInstance());

    ChannelMessage midimsg1 = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel,
        (track + 0 - 1) << 4 | 0xC,
        0,
    };
    ChannelMessage midimsg2 = {
        MIDIMessageType::CHANNEL_PRESSURE,
        channel,
        (track + 4 - 1) << 4 | 0x6,
        0,
    };
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(0, HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(1, HIGH));
    MIDIInputElementCP::updateAllWith(midimsg1);
    Mock::VerifyAndClear(&ArduinoMock::getInstance());

    MIDIInputElementCP::updateAllWith(midimsg2);
    EXPECT_EQ(vu.getValue(), 0xC);
    Mock::VerifyAndClear(&ArduinoMock::getInstance());

    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(0, HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(1, LOW));
    bank.select(1); // marks dirty
    vu.update();    // notices dirty flag and updates the LEDs
    EXPECT_EQ(vu.getValue(), 0x6);
    Mock::VerifyAndClear(&ArduinoMock::getInstance());

    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(0, HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(1, HIGH));
    bank.select(0);
    vu.update();
    EXPECT_EQ(vu.getValue(), 0xC);
    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}