#include <Encoder.h>
#include <MIDI_Outputs/CCRotaryEncoder.hpp>
#include <MockMIDI_Interface.hpp>
#include <gtest/gtest.h>

using namespace ::testing;

TEST(CCRotaryEncoder, turnOneStep) {
    MockMIDI_Interface midi;

    EncoderMock encm;
    CCRotaryEncoder ccenc = {encm, 0x20, 7, 2, 4};

    EXPECT_CALL(encm, read()).WillOnce(Return(4));
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x20, 2));

    ccenc.update();
}

TEST(CCRotaryEncoder, turnFourHalfSteps) {
    MockMIDI_Interface midi;

    EncoderMock encm;
    CCRotaryEncoder ccenc = {encm, 0x20, 7, 2, 4};

    EXPECT_CALL(encm, read())
        .WillOnce(Return(2))
        .WillOnce(Return(4))
        .WillOnce(Return(6))
        .WillOnce(Return(8));

    ccenc.update(); // half a step

    EXPECT_CALL(midi, sendImpl(CC, 6, 0x20, 2));

    ccenc.update(); // one step

    Mock::VerifyAndClear(&midi);

    ccenc.update(); // one and a half step

    EXPECT_CALL(midi, sendImpl(CC, 6, 0x20, 2));

    ccenc.update(); // two steps
}

TEST(CCRotaryEncoder, turnOneStepBackwards) {
    MockMIDI_Interface midi;

    EncoderMock encm;
    CCRotaryEncoder ccenc = {encm, 0x20, 7, 2, 4};

    EXPECT_CALL(encm, read()).WillOnce(Return(-4));
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x20, (-2) & 0x7F));

    ccenc.update();
}

TEST(CCRotaryEncoder, turnSixteenSteps) {
    MockMIDI_Interface midi;

    EncoderMock encm;
    CCRotaryEncoder ccenc = {encm, 0x20, 7, 2, 4};

    // Should be send in packets of value 15 max
    EXPECT_CALL(encm, read()).WillOnce(Return(4 * 16));
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x20, 15)).Times(2);
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x20, 2)).Times(1);

    ccenc.update();
}

TEST(CCRotaryEncoder, turnSixteenStepsBackwards) {
    MockMIDI_Interface midi;

    EncoderMock encm;
    CCRotaryEncoder ccenc = {encm, 0x20, 7, 2, 4};

    // Should be send in packets of value -15 minimum
    EXPECT_CALL(encm, read()).WillOnce(Return(-4 * 16));
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x20, (-15) & 0x7F)).Times(2);
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x20, (-2) & 0x7F)).Times(1);

    ccenc.update();
}

// -------------------------------------------------------------------------- //

#include <MIDI_Outputs/Bankable/CCRotaryEncoder.hpp>

TEST(CCRotaryEncoderBankable, turnOneStepChangeSettingTurnOneStep) {
    MockMIDI_Interface midi;

    Bank bank(4);

    EncoderMock encm;
    Bankable::CCRotaryEncoder ccenc = {bank, encm, 0x20, 7, 2, 4};

    EXPECT_CALL(encm, read()).WillOnce(Return(4));
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x20, 2));

    ccenc.update();

    bank.setBankSetting(1);

    EXPECT_CALL(encm, read()).WillOnce(Return(8));
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x20 + 4, 2));

    ccenc.update();
}