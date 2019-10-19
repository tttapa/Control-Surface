#include <MIDI_Outputs/Bankable/CCPotentiometer.hpp>
#include <MIDI_Outputs/CCPotentiometer.hpp>
#include <MockMIDI_Interface.hpp>
#include <gmock-wrapper.h>

using namespace ::testing;
using namespace CS;

TEST(CCPotentiometer, simple) {
    MockMIDI_Interface midi;

    CCPotentiometer pot(2, {0x3C, CHANNEL_7, 0xC});
    pot.begin();

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(2))
        .Times(3)
        .WillRepeatedly(Return(512));
    InSequence s;
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x3C, 16, 0xC));
    pot.update();
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x3C, 28, 0xC));
    pot.update();
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x3C, 37, 0xC));
    pot.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(CCPotentiometer, mapping) {
    MockMIDI_Interface midi;

    CCPotentiometer pot(2, {0x3C, CHANNEL_7, 0xC});
    pot.begin();
    pot.map([](analog_t x) -> analog_t { return x * 2; });

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(2))
        .Times(3)
        .WillRepeatedly(Return(512));
    InSequence s;
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x3C, 16 * 2, 0xC));
    pot.update();
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x3C, 28 * 2, 0xC));
    pot.update();
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x3C, 37 * 2, 0xC));
    pot.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(CCPotentiometer, invert) {
    MockMIDI_Interface midi;

    CCPotentiometer pot(2, {0x3C, CHANNEL_7, 0xC});
    pot.begin();
    pot.invert();

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(2))
        .Times(3)
        .WillRepeatedly(Return(512));
    InSequence s;
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x3C, 127 - 16, 0xC));
    pot.update();
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x3C, 127 - 28, 0xC));
    pot.update();
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x3C, 127 - 37, 0xC));
    pot.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}