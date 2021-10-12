#include <MIDI_Outputs/Bankable/PBPotentiometer.hpp>
#include <MIDI_Outputs/PBPotentiometer.hpp>
#include <MockMIDI_Interface.hpp>
#include <gmock/gmock.h>

using namespace ::testing;
using namespace CS;

auto low = [](uint16_t x) { return (x * 128 & 0x7F) | (x >> 3); };
auto high = [](uint16_t x) { return x; };

TEST(PBPotentiometer, simple) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();

    PBPotentiometer pot(2, {CHANNEL_7, CABLE_13});
    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(2)).WillOnce(Return(0));
    pot.begin();

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(2))
        .Times(3)
        .WillRepeatedly(Return(512));
    InSequence s;
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xE6, low(16), high(16), CABLE_13)));
    pot.update();
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xE6, low(28), high(28), CABLE_13)));
    pot.update();
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xE6, low(37), high(37), CABLE_13)));
    pot.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(PBPotentiometer, mapping) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();

    PBPotentiometer pot(2, {CHANNEL_7, CABLE_13});
    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(2)).WillOnce(Return(0));
    pot.begin();
    pot.map([](analog_t x) -> analog_t { return x * 2; });

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(2))
        .Times(3)
        .WillRepeatedly(Return(512));
    InSequence s;
    EXPECT_CALL(midi, sendChannelMessageImpl(ChannelMessage(
                          0xE6, low(16 * 2), high(16 * 2), CABLE_13)));
    pot.update();
    EXPECT_CALL(midi, sendChannelMessageImpl(ChannelMessage(
                          0xE6, low(28 * 2), high(28 * 2), CABLE_13)));
    pot.update();
    EXPECT_CALL(midi, sendChannelMessageImpl(ChannelMessage(
                          0xE6, low(37 * 2), high(37 * 2), CABLE_13)));
    pot.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(PBPotentiometer, invert) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();

    PBPotentiometer pot(2, {CHANNEL_7, CABLE_13});
    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(2)).WillOnce(Return(0));
    pot.begin();
    pot.invert();

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(2))
        .Times(3)
        .WillRepeatedly(Return(512));
    InSequence s;
    EXPECT_CALL(midi, sendChannelMessageImpl(ChannelMessage(
                          0xE6, 127 - low(16), 127 - high(16), CABLE_13)));
    pot.update();
    EXPECT_CALL(midi, sendChannelMessageImpl(ChannelMessage(
                          0xE6, 127 - low(28), 127 - high(28), CABLE_13)));
    pot.update();
    EXPECT_CALL(midi, sendChannelMessageImpl(ChannelMessage(
                          0xE6, 127 - low(37), 127 - high(37), CABLE_13)));
    pot.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}