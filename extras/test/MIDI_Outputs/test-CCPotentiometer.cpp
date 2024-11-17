#include <MIDI_Outputs/Bankable/CCPotentiometer.hpp>
#include <MIDI_Outputs/CCPotentiometer.hpp>
#include <MockMIDI_Interface.hpp>
#include <gmock/gmock.h>

using namespace ::testing;
using namespace cs;

TEST(CCPotentiometer, simple) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();

    CCPotentiometer pot(2, {0x3C, Channel_7, Cable_13});
    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(2)).WillOnce(Return(0));
    pot.begin();

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(2))
        .Times(3)
        .WillRepeatedly(Return(512));
    InSequence s;
    EXPECT_CALL(
        midi, sendChannelMessageImpl(ChannelMessage(0xB6, 0x3C, 16, Cable_13)));
    pot.update();
    EXPECT_CALL(
        midi, sendChannelMessageImpl(ChannelMessage(0xB6, 0x3C, 28, Cable_13)));
    pot.update();
    EXPECT_CALL(
        midi, sendChannelMessageImpl(ChannelMessage(0xB6, 0x3C, 37, Cable_13)));
    pot.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(CCPotentiometer, mapping) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();

    CCPotentiometer pot(2, {0x3C, Channel_7, Cable_13});
    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(2)).WillOnce(Return(0));
    pot.begin();
    pot.map([](analog_t x) -> analog_t { return x * 2; });

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(2))
        .Times(3)
        .WillRepeatedly(Return(512));
    InSequence s;
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x3C, 16 * 2, Cable_13)));
    pot.update();
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x3C, 28 * 2, Cable_13)));
    pot.update();
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x3C, 37 * 2, Cable_13)));
    pot.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(CCPotentiometer, invert) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();

    CCPotentiometer pot(2, {0x3C, Channel_7, Cable_13});
    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(2)).WillOnce(Return(0));
    pot.begin();
    pot.invert();

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(2))
        .Times(3)
        .WillRepeatedly(Return(512));
    InSequence s;
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x3C, 127 - 16, Cable_13)));
    pot.update();
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x3C, 127 - 28, Cable_13)));
    pot.update();
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x3C, 127 - 37, Cable_13)));
    pot.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}