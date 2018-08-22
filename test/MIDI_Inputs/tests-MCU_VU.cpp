#include <Banks/Bank.h>
#include <MIDI_Inputs/MCU/VU.hpp>
#include <gtest/gtest.h>

using namespace ::testing;

// -------------------------------------------------------------------------- //

TEST(MCUVU, setValue) {
    constexpr uint8_t channel = 3;
    constexpr uint8_t track = 5;
    MCU::VU vu(track, channel);
    MIDI_message_matcher midimsg = {CHANNEL_PRESSURE, channel - 1,
                                    (track - 1) << 4 | 0xA, 0};
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(0));
    MIDIInputElementChannelPressure::updateAllWith(midimsg);
    EXPECT_EQ(vu.getValue(), 0xA);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(MCUVU, setOverload) {
    constexpr uint8_t channel = 2;
    constexpr uint8_t track = 6;
    MCU::VU vu(track, channel);
    MIDI_message_matcher midimsg = {CHANNEL_PRESSURE, channel - 1,
                                    (track - 1) << 4 | 0xE, 0};
    MIDIInputElementChannelPressure::updateAllWith(midimsg);
    EXPECT_EQ(vu.getValue(), 0);
    EXPECT_TRUE(vu.getOverload());

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(MCUVU, clearOverload) {
    constexpr uint8_t channel = 3;
    constexpr uint8_t track = 5;
    MCU::VU vu(track, channel);
    MIDI_message_matcher midimsgSet = {CHANNEL_PRESSURE, channel - 1,
                                       (track - 1) << 4 | 0xE, 0};
    MIDIInputElementChannelPressure::updateAllWith(midimsgSet);
    MIDI_message_matcher midimsgClr = {CHANNEL_PRESSURE, channel - 1,
                                       (track - 1) << 4 | 0xF, 0};
    MIDIInputElementChannelPressure::updateAllWith(midimsgClr);
    EXPECT_EQ(vu.getValue(), 0);
    EXPECT_FALSE(vu.getOverload());

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(MCUVU, retainValueAfterOverload) {
    constexpr uint8_t channel = 3;
    constexpr uint8_t track = 5;
    MCU::VU vu(track, channel);
    MIDI_message_matcher midimsg = {CHANNEL_PRESSURE, channel - 1,
                                    (track - 1) << 4 | 0x6, 0};
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(0));
    MIDIInputElementChannelPressure::updateAllWith(midimsg);
    EXPECT_EQ(vu.getValue(), 0x6);
    MIDI_message_matcher midimsgSet = {CHANNEL_PRESSURE, channel - 1,
                                       (track - 1) << 4 | 0xE, 0};
    MIDIInputElementChannelPressure::updateAllWith(midimsgSet);
    EXPECT_EQ(vu.getValue(), 0x6);
    MIDI_message_matcher midimsgClr = {CHANNEL_PRESSURE, channel - 1,
                                       (track - 1) << 4 | 0xF, 0};
    MIDIInputElementChannelPressure::updateAllWith(midimsgClr);
    EXPECT_EQ(vu.getValue(), 0x6);
    EXPECT_FALSE(vu.getOverload());

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(MCUVU, decay) {
    constexpr uint8_t channel = 3;
    constexpr uint8_t track = 5;
    constexpr unsigned int decayTime = 300;
    MCU::VU vu(track, channel, decayTime);
    MIDI_message_matcher midimsg = {CHANNEL_PRESSURE, channel - 1,
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

// -------------------------------------------------------------------------- //

TEST(MCUVUBankable, setValueBankChangeAddress) {
    Bank<2> bank(4);
    constexpr uint8_t channel = 3;
    constexpr uint8_t track = 5;
    MCU::Bankable::VU<2> vu(bank, track, channel);
    MIDI_message_matcher midimsg = {CHANNEL_PRESSURE, channel - 1,
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
    constexpr uint8_t channel = 3;
    constexpr uint8_t track = 5;
    MCU::Bankable::VU<3> vu({bank, CHANGE_CHANNEL}, track, channel);
    MIDI_message_matcher midimsg1 = {CHANNEL_PRESSURE, channel + 4 - 1,
                                     (track - 1) << 4 | 0xA, 0};
    MIDI_message_matcher midimsg2 = {CHANNEL_PRESSURE, channel + 8 - 1,
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

TEST(MCUVUBankable, overloadBankChangeAddress) {
    Bank<2> bank(4);
    constexpr uint8_t channel = 3;
    constexpr uint8_t track = 5;
    MCU::Bankable::VU<2> vu(bank, track, channel);
    EXPECT_FALSE(vu.getOverload());
    bank.select(1);
    EXPECT_FALSE(vu.getOverload());
    MIDI_message_matcher midimsgSet = {CHANNEL_PRESSURE, channel - 1,
                                       (track + 4 - 1) << 4 | 0xE, 0};
    MIDIInputElementChannelPressure::updateAllWith(midimsgSet);
    bank.select(0);
    EXPECT_FALSE(vu.getOverload());
    bank.select(1);
    EXPECT_TRUE(vu.getOverload());
    MIDI_message_matcher midimsgClr = {CHANNEL_PRESSURE, channel - 1,
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
    constexpr uint8_t channel = 3;
    constexpr uint8_t track = 5;
    constexpr unsigned int decayTime = 150;
    MCU::Bankable::VULEDs<2, 2> vu = {
        bank, {{0, 1}}, track, channel, decayTime};

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(0, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(1, OUTPUT));

    MIDIInputElementChannelPressure::beginAll();

    MIDI_message_matcher midimsg1 = {CHANNEL_PRESSURE, channel - 1,
                                     (track + 0 - 1) << 4 | 0xC, 0};
    MIDI_message_matcher midimsg2 = {CHANNEL_PRESSURE, channel - 1,
                                     (track + 4 - 1) << 4 | 0x6, 0};
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(0, HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(1, HIGH));
    MIDIInputElementChannelPressure::updateAllWith(midimsg1);
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(0));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(0, HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(1, HIGH));
    MIDIInputElementChannelPressure::updateAllWith(midimsg2);

    EXPECT_EQ(vu.getValue(), 0xC);
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(0, HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(1, LOW));
    bank.select(1);
    EXPECT_EQ(vu.getValue(), 0x6);
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(0, HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(1, HIGH));
    bank.select(0);

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}