#include <Encoder.h>
#include <MIDI_Outputs/CCRotaryEncoder.hpp>
#include <MockMIDI_Interface.hpp>
#include <gtest-wrapper.h>

using namespace ::testing;
using namespace CS;

TEST(CCRotaryEncoder, turnOneStep) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();
    RelativeCCSender::setMode(relativeCCmode::TWOS_COMPLEMENT);

    EncoderMock encm;
    CCRotaryEncoder ccenc = {encm, {0x20, CHANNEL_7, 0xC}, 2, 4};

    EXPECT_CALL(encm, read()).WillOnce(Return(4));
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x20, 2, 0xC));

    ccenc.update();
}

TEST(CCRotaryEncoder, turnFourHalfSteps) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();
    RelativeCCSender::setMode(relativeCCmode::TWOS_COMPLEMENT);

    EncoderMock encm;
    CCRotaryEncoder ccenc = {encm, {0x20, CHANNEL_7, 0xC}, 2, 4};

    EXPECT_CALL(encm, read())
        .WillOnce(Return(2))
        .WillOnce(Return(4))
        .WillOnce(Return(5))
        .WillOnce(Return(6))
        .WillOnce(Return(10));

    // 0.5 steps × 2
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x20, 1, 0xC));
    ccenc.update();
    Mock::VerifyAndClear(&midi);

    // 1 step × 2
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x20, 1, 0xC));
    ccenc.update();
    Mock::VerifyAndClear(&midi);

    // 1.25 step × 2
    ccenc.update();
    Mock::VerifyAndClear(&midi);

    // 1.5 step × 2
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x20, 1, 0xC));
    ccenc.update();
    Mock::VerifyAndClear(&midi);

    // 2.5 step × 2
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x20, 2, 0xC));
    ccenc.update();
    Mock::VerifyAndClear(&midi);
}

TEST(CCRotaryEncoder, turnOneStepBackwards) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();
    RelativeCCSender::setMode(relativeCCmode::TWOS_COMPLEMENT);

    EncoderMock encm;
    CCRotaryEncoder ccenc = {encm, {0x20, CHANNEL_7, 0xC}, 2, 4};

    EXPECT_CALL(encm, read()).WillOnce(Return(-4));
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x20, (-2) & 0x7F, 0xC));

    ccenc.update();
}

TEST(CCRotaryEncoder, turnSixteenSteps) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();
    RelativeCCSender::setMode(relativeCCmode::TWOS_COMPLEMENT);

    EncoderMock encm;
    CCRotaryEncoder ccenc = {encm, {0x20, CHANNEL_7, 0xC}, 2, 4};

    // Should be sent in packets of value 15 max
    EXPECT_CALL(encm, read()).WillOnce(Return(4 * 16));
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x20, 15, 0xC)).Times(2);
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x20, 2, 0xC)).Times(1);

    ccenc.update();
}

TEST(CCRotaryEncoder, turnSixteenStepsBackwards) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();
    RelativeCCSender::setMode(relativeCCmode::TWOS_COMPLEMENT);

    EncoderMock encm;
    CCRotaryEncoder ccenc = {encm, {0x20, CHANNEL_7, 0xC}, 2, 4};

    // Should be sent in packets of value -15 minimum
    EXPECT_CALL(encm, read()).WillOnce(Return(-4 * 16));
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x20, (-15) & 0x7F, 0xC)).Times(2);
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x20, (-2) & 0x7F, 0xC)).Times(1);

    ccenc.update();
}

TEST(CCRotaryEncoder, turnSixteenStepsForwardsSignMagnitude) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();
    RelativeCCSender::setMode(relativeCCmode::TWOS_COMPLEMENT);

    EncoderMock encm;
    RelativeCCSender::setMode(relativeCCmode::SIGN_MAGNITUDE);
    CCRotaryEncoder ccenc = {encm, {0x20, CHANNEL_7, 0xC}, 2, 4};

    // Should be sent in packets of value 15 maximum
    EXPECT_CALL(encm, read()).WillOnce(Return(4 * 16));
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x20, 15, 0xC)).Times(2);
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x20, 2, 0xC)).Times(1);

    ccenc.update();
}

TEST(CCRotaryEncoder, turnSixteenStepsBackwardsSignMagnitude) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();
    RelativeCCSender::setMode(relativeCCmode::TWOS_COMPLEMENT);

    EncoderMock encm;
    RelativeCCSender::setMode(relativeCCmode::SIGN_MAGNITUDE);
    CCRotaryEncoder ccenc = {encm, {0x20, CHANNEL_7, 0xC}, 2, 4};

    // Should be sent in packets of value -15 minimum
    EXPECT_CALL(encm, read()).WillOnce(Return(-4 * 16));
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x20, 15 | 0x40, 0xC)).Times(2);
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x20, 2 | 0x40, 0xC)).Times(1);

    ccenc.update();
}

TEST(CCRotaryEncoder, turnSixteenStepsForwardsNextAddress) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();
    RelativeCCSender::setMode(relativeCCmode::TWOS_COMPLEMENT);

    EncoderMock encm;
    RelativeCCSender::setMode(relativeCCmode::NEXT_ADDRESS);
    CCRotaryEncoder ccenc = {encm, {0x20, CHANNEL_7, 0xC}, 2, 4};

    // Should be sent in packets of value 15 maximum
    EXPECT_CALL(encm, read()).WillOnce(Return(4 * 16));
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x20, 15, 0xC)).Times(2);
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x20, 2, 0xC)).Times(1);

    ccenc.update();
}

TEST(CCRotaryEncoder, turnSixteenStepsBackwardsNextAddress) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();
    RelativeCCSender::setMode(relativeCCmode::TWOS_COMPLEMENT);

    EncoderMock encm;
    RelativeCCSender::setMode(relativeCCmode::NEXT_ADDRESS);
    CCRotaryEncoder ccenc = {encm, {0x20, CHANNEL_7, 0xC}, 2, 4};

    // Should be sent in packets of value -15 minimum
    EXPECT_CALL(encm, read()).WillOnce(Return(-4 * 16));
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x21, 15, 0xC)).Times(2);
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x21, 2, 0xC)).Times(1);

    ccenc.update();
}

TEST(CCRotaryEncoder, turnSixteenStepsBackwardsBinaryOffset) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();
    RelativeCCSender::setMode(relativeCCmode::TWOS_COMPLEMENT);

    EncoderMock encm;
    RelativeCCSender::setMode(relativeCCmode::BINARY_OFFSET);
    CCRotaryEncoder ccenc = {encm, {0x20, CHANNEL_7, 0xC}, 2, 4};

    // Should be sent in packets of value -15 minimum
    EXPECT_CALL(encm, read()).WillOnce(Return(-4 * 16));
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x20, 0x40 - 15, 0xC)).Times(2);
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x20, 0x40 - 2, 0xC)).Times(1);

    ccenc.update();
}

// -------------------------------------------------------------------------- //

#include <MIDI_Outputs/Bankable/CCRotaryEncoder.hpp>

TEST(CCRotaryEncoderBankable, turnOneStepChangeSettingTurnOneStep) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();
    RelativeCCSender::setMode(relativeCCmode::TWOS_COMPLEMENT);

    OutputBank bank(4);

    EncoderMock encm;
    Bankable::CCRotaryEncoder ccenc = {
        bank, encm, {0x20, CHANNEL_7, 0xC}, 2, 4};

    EXPECT_CALL(encm, read()).WillOnce(Return(4));
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x20, 2, 0xC));

    ccenc.update();

    bank.select(1);

    EXPECT_CALL(encm, read()).WillOnce(Return(8));
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x20 + 4, 2, 0xC));

    ccenc.update();
}