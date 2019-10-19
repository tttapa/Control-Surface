#include <MIDI_Outputs/Bankable/PBPotentiometer.hpp>
#include <MIDI_Outputs/PBPotentiometer.hpp>
#include <MockMIDI_Interface.hpp>
#include <gmock-wrapper.h>

using namespace ::testing;
using namespace CS;

auto low = [](uint16_t x) { return (x * 128 & 0x7F) | (x >> 3); };
auto high = [](uint16_t x) { return x; };

TEST(PBPotentiometer, simple) {
    MockMIDI_Interface midi;

    PBPotentiometer pot(2, {CHANNEL_7, 0xC});
    pot.begin();

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(2))
        .Times(3)
        .WillRepeatedly(Return(512));
    InSequence s;
    EXPECT_CALL(midi, sendImpl(PITCH_BEND, 6, low(16), high(16), 0xC));
    pot.update();
    EXPECT_CALL(midi, sendImpl(PITCH_BEND, 6, low(28), high(28), 0xC));
    pot.update();
    EXPECT_CALL(midi, sendImpl(PITCH_BEND, 6, low(37), high(37), 0xC));
    pot.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(PBPotentiometer, mapping) {
    MockMIDI_Interface midi;

    PBPotentiometer pot(2, {CHANNEL_7, 0xC});
    pot.begin();
    pot.map([](analog_t x) -> analog_t { return x * 2; });

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(2))
        .Times(3)
        .WillRepeatedly(Return(512));
    InSequence s;
    EXPECT_CALL(midi, sendImpl(PITCH_BEND, 6, low(16 * 2), high(16 * 2), 0xC));
    pot.update();
    EXPECT_CALL(midi, sendImpl(PITCH_BEND, 6, low(28 * 2), high(28 * 2), 0xC));
    pot.update();
    EXPECT_CALL(midi, sendImpl(PITCH_BEND, 6, low(37 * 2), high(37 * 2), 0xC));
    pot.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(PBPotentiometer, invert) {
    MockMIDI_Interface midi;

    PBPotentiometer pot(2, {CHANNEL_7, 0xC});
    pot.begin();
    pot.invert();

    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(2))
        .Times(3)
        .WillRepeatedly(Return(512));
    InSequence s;
    EXPECT_CALL(midi,
                sendImpl(PITCH_BEND, 6, 127 - low(16), 127 - high(16), 0xC));
    pot.update();
    EXPECT_CALL(midi,
                sendImpl(PITCH_BEND, 6, 127 - low(28), 127 - high(28), 0xC));
    pot.update();
    EXPECT_CALL(midi,
                sendImpl(PITCH_BEND, 6, 127 - low(37), 127 - high(37), 0xC));
    pot.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}