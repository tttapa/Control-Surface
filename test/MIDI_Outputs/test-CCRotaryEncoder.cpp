#include <MIDI_Outputs/CCRotaryEncoder.hpp>
#include <MockMIDI_Interface.hpp>
#include <gtest/gtest.h>

using namespace ::testing;
using namespace CS;

TEST(CCRotaryEncoder, turnOneStep) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();
    RelativeCCSender::setMode(relativeCCmode::TWOS_COMPLEMENT);

    EncoderMock encm;
    CCRotaryEncoder ccenc = {encm, {0x20, CHANNEL_7, CABLE_13}, 2, 4};

    EXPECT_CALL(encm, read()).WillOnce(Return(4));
    EXPECT_CALL(
        midi, sendChannelMessageImpl(ChannelMessage(0xB6, 0x20, 2, CABLE_13)));

    ccenc.update();
}

TEST(CCRotaryEncoder, turnFourHalfSteps) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();
    RelativeCCSender::setMode(relativeCCmode::TWOS_COMPLEMENT);

    EncoderMock encm;
    CCRotaryEncoder ccenc = {encm, {0x20, CHANNEL_7, CABLE_13}, 2, 4};

    EXPECT_CALL(encm, read())
        .WillOnce(Return(2))
        .WillOnce(Return(4))
        .WillOnce(Return(5))
        .WillOnce(Return(6))
        .WillOnce(Return(10));

    // 0.5 steps × 2
    EXPECT_CALL(
        midi, sendChannelMessageImpl(ChannelMessage(0xB6, 0x20, 1, CABLE_13)));
    ccenc.update();
    Mock::VerifyAndClear(&midi);

    // 1 step × 2
    EXPECT_CALL(
        midi, sendChannelMessageImpl(ChannelMessage(0xB6, 0x20, 1, CABLE_13)));
    ccenc.update();
    Mock::VerifyAndClear(&midi);

    // 1.25 step × 2
    ccenc.update();
    Mock::VerifyAndClear(&midi);

    // 1.5 step × 2
    EXPECT_CALL(
        midi, sendChannelMessageImpl(ChannelMessage(0xB6, 0x20, 1, CABLE_13)));
    ccenc.update();
    Mock::VerifyAndClear(&midi);

    // 2.5 step × 2
    EXPECT_CALL(
        midi, sendChannelMessageImpl(ChannelMessage(0xB6, 0x20, 2, CABLE_13)));
    ccenc.update();
    Mock::VerifyAndClear(&midi);
}

TEST(CCRotaryEncoder, highSpeedMultiply) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();
    RelativeCCSender::setMode(relativeCCmode::TWOS_COMPLEMENT);

    EncoderMock encm;
    CCRotaryEncoder ccenc = {encm, {0x20, CHANNEL_7, CABLE_13}, 10, 4};

    EXPECT_CALL(encm, read())
        .WillOnce(Return(1))
        .WillOnce(Return(2))
        .WillOnce(Return(3))
        .WillOnce(Return(4))
        .WillOnce(Return(8))
        .WillOnce(Return(10))
        .WillOnce(Return(12))
        .WillOnce(Return(21))
        .WillOnce(Return(21));

    // 0.25 steps × 10
    EXPECT_CALL(
        midi, sendChannelMessageImpl(ChannelMessage(0xB6, 0x20, 2, CABLE_13)));
    ccenc.update();
    Mock::VerifyAndClear(&midi);

    // 0.5 steps × 10
    EXPECT_CALL(
        midi, sendChannelMessageImpl(ChannelMessage(0xB6, 0x20, 3, CABLE_13)));
    ccenc.update();
    Mock::VerifyAndClear(&midi);

    // 0.75 steps × 10
    EXPECT_CALL(
        midi, sendChannelMessageImpl(ChannelMessage(0xB6, 0x20, 2, CABLE_13)));
    ccenc.update();
    Mock::VerifyAndClear(&midi);

    // 1 step × 10
    EXPECT_CALL(
        midi, sendChannelMessageImpl(ChannelMessage(0xB6, 0x20, 3, CABLE_13)));
    ccenc.update();
    Mock::VerifyAndClear(&midi);

    // 2 steps × 10
    EXPECT_CALL(
        midi, sendChannelMessageImpl(ChannelMessage(0xB6, 0x20, 10, CABLE_13)));
    ccenc.update();
    Mock::VerifyAndClear(&midi);

    // 2.5 steps × 10
    EXPECT_CALL(
        midi, sendChannelMessageImpl(ChannelMessage(0xB6, 0x20, 5, CABLE_13)));
    ccenc.update();
    Mock::VerifyAndClear(&midi);

    // 3 steps × 10
    EXPECT_CALL(
        midi, sendChannelMessageImpl(ChannelMessage(0xB6, 0x20, 5, CABLE_13)));
    ccenc.update();
    Mock::VerifyAndClear(&midi);

    // 5.25 steps × 10
    EXPECT_CALL(
        midi, sendChannelMessageImpl(ChannelMessage(0xB6, 0x20, 15, CABLE_13)));
    EXPECT_CALL(
        midi, sendChannelMessageImpl(ChannelMessage(0xB6, 0x20, 7, CABLE_13)));
    ccenc.update();
    Mock::VerifyAndClear(&midi);

    // 5.25 steps × 10
    ccenc.update();
    Mock::VerifyAndClear(&midi);
}

TEST(CCRotaryEncoder, highSpeedMultiplyBackwards) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();
    RelativeCCSender::setMode(relativeCCmode::TWOS_COMPLEMENT);

    EncoderMock encm;
    CCRotaryEncoder ccenc = {encm, {0x20, CHANNEL_7, CABLE_13}, 10, 4};

    EXPECT_CALL(encm, read())
        .WillOnce(Return(-1))
        .WillOnce(Return(-2))
        .WillOnce(Return(-3))
        .WillOnce(Return(-4))
        .WillOnce(Return(-8))
        .WillOnce(Return(-10))
        .WillOnce(Return(-12))
        .WillOnce(Return(-21))
        .WillOnce(Return(-21));

    // 0.25 steps × 10
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x20, 128 - 2, CABLE_13)));
    ccenc.update();
    Mock::VerifyAndClear(&midi);

    // 0.5 steps × 10
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x20, 128 - 3, CABLE_13)));
    ccenc.update();
    Mock::VerifyAndClear(&midi);

    // 0.75 steps × 10
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x20, 128 - 2, CABLE_13)));
    ccenc.update();
    Mock::VerifyAndClear(&midi);

    // 1 step × 10
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x20, 128 - 3, CABLE_13)));
    ccenc.update();
    Mock::VerifyAndClear(&midi);

    // 2 steps × 10
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x20, 128 - 10, CABLE_13)));
    ccenc.update();
    Mock::VerifyAndClear(&midi);

    // 2.5 steps × 10
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x20, 128 - 5, CABLE_13)));
    ccenc.update();
    Mock::VerifyAndClear(&midi);

    // 3 steps × 10
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x20, 128 - 5, CABLE_13)));
    ccenc.update();
    Mock::VerifyAndClear(&midi);

    // 5.25 steps × 10
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x20, 128 - 15, CABLE_13)));
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x20, 128 - 7, CABLE_13)));
    ccenc.update();
    Mock::VerifyAndClear(&midi);

    // 5.25 steps × 10
    ccenc.update();
    Mock::VerifyAndClear(&midi);
}

TEST(CCRotaryEncoder, turnOneStepBackwards) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();
    RelativeCCSender::setMode(relativeCCmode::TWOS_COMPLEMENT);

    EncoderMock encm;
    CCRotaryEncoder ccenc = {encm, {0x20, CHANNEL_7, CABLE_13}, 2, 4};

    EXPECT_CALL(encm, read()).WillOnce(Return(-4));
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x20, (-2) & 0x7F, CABLE_13)));

    ccenc.update();
}

TEST(CCRotaryEncoder, turnSixteenSteps) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();
    RelativeCCSender::setMode(relativeCCmode::TWOS_COMPLEMENT);

    EncoderMock encm;
    CCRotaryEncoder ccenc = {encm, {0x20, CHANNEL_7, CABLE_13}, 2, 4};

    // Should be sent in packets of value 15 max
    EXPECT_CALL(encm, read()).WillOnce(Return(4 * 16));
    EXPECT_CALL(
        midi, sendChannelMessageImpl(ChannelMessage(0xB6, 0x20, 15, CABLE_13)))
        .Times(2);
    EXPECT_CALL(midi,
                sendChannelMessageImpl(ChannelMessage(0xB6, 0x20, 2, CABLE_13)))
        .Times(1);

    ccenc.update();
}

TEST(CCRotaryEncoder, turnSixteenStepsBackwards) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();
    RelativeCCSender::setMode(relativeCCmode::TWOS_COMPLEMENT);

    EncoderMock encm;
    CCRotaryEncoder ccenc = {encm, {0x20, CHANNEL_7, CABLE_13}, 2, 4};

    // Should be sent in packets of value -15 minimum
    EXPECT_CALL(encm, read()).WillOnce(Return(-4 * 16));
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x20, (-15) & 0x7F, CABLE_13)))
        .Times(2);
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x20, (-2) & 0x7F, CABLE_13)))
        .Times(1);

    ccenc.update();
}

TEST(CCRotaryEncoder, turnSixteenStepsForwardsSignMagnitude) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();
    RelativeCCSender::setMode(relativeCCmode::TWOS_COMPLEMENT);

    EncoderMock encm;
    RelativeCCSender::setMode(relativeCCmode::SIGN_MAGNITUDE);
    CCRotaryEncoder ccenc = {encm, {0x20, CHANNEL_7, CABLE_13}, 2, 4};

    // Should be sent in packets of value 15 maximum
    EXPECT_CALL(encm, read()).WillOnce(Return(4 * 16));
    EXPECT_CALL(
        midi, sendChannelMessageImpl(ChannelMessage(0xB6, 0x20, 15, CABLE_13)))
        .Times(2);
    EXPECT_CALL(midi,
                sendChannelMessageImpl(ChannelMessage(0xB6, 0x20, 2, CABLE_13)))
        .Times(1);

    ccenc.update();
}

TEST(CCRotaryEncoder, turnSixteenStepsBackwardsSignMagnitude) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();
    RelativeCCSender::setMode(relativeCCmode::TWOS_COMPLEMENT);

    EncoderMock encm;
    RelativeCCSender::setMode(relativeCCmode::SIGN_MAGNITUDE);
    CCRotaryEncoder ccenc = {encm, {0x20, CHANNEL_7, CABLE_13}, 2, 4};

    // Should be sent in packets of value -15 minimum
    EXPECT_CALL(encm, read()).WillOnce(Return(-4 * 16));
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x20, 15 | 0x40, CABLE_13)))
        .Times(2);
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x20, 2 | 0x40, CABLE_13)))
        .Times(1);

    ccenc.update();
}

TEST(CCRotaryEncoder, turnSixteenStepsForwardsNextAddress) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();
    RelativeCCSender::setMode(relativeCCmode::TWOS_COMPLEMENT);

    EncoderMock encm;
    RelativeCCSender::setMode(relativeCCmode::NEXT_ADDRESS);
    CCRotaryEncoder ccenc = {encm, {0x20, CHANNEL_7, CABLE_13}, 2, 4};

    // Should be sent in packets of value 15 maximum
    EXPECT_CALL(encm, read()).WillOnce(Return(4 * 16));
    EXPECT_CALL(
        midi, sendChannelMessageImpl(ChannelMessage(0xB6, 0x20, 15, CABLE_13)))
        .Times(2);
    EXPECT_CALL(midi,
                sendChannelMessageImpl(ChannelMessage(0xB6, 0x20, 2, CABLE_13)))
        .Times(1);

    ccenc.update();
}

TEST(CCRotaryEncoder, turnSixteenStepsBackwardsNextAddress) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();
    RelativeCCSender::setMode(relativeCCmode::TWOS_COMPLEMENT);

    EncoderMock encm;
    RelativeCCSender::setMode(relativeCCmode::NEXT_ADDRESS);
    CCRotaryEncoder ccenc = {encm, {0x20, CHANNEL_7, CABLE_13}, 2, 4};

    // Should be sent in packets of value -15 minimum
    EXPECT_CALL(encm, read()).WillOnce(Return(-4 * 16));
    EXPECT_CALL(
        midi, sendChannelMessageImpl(ChannelMessage(0xB6, 0x21, 15, CABLE_13)))
        .Times(2);
    EXPECT_CALL(midi,
                sendChannelMessageImpl(ChannelMessage(0xB6, 0x21, 2, CABLE_13)))
        .Times(1);

    ccenc.update();
}

TEST(CCRotaryEncoder, turnSixteenStepsBackwardsBinaryOffset) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();
    RelativeCCSender::setMode(relativeCCmode::TWOS_COMPLEMENT);

    EncoderMock encm;
    RelativeCCSender::setMode(relativeCCmode::BINARY_OFFSET);
    CCRotaryEncoder ccenc = {encm, {0x20, CHANNEL_7, CABLE_13}, 2, 4};

    // Should be sent in packets of value -15 minimum
    EXPECT_CALL(encm, read()).WillOnce(Return(-4 * 16));
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x20, 0x40 - 15, CABLE_13)))
        .Times(2);
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x20, 0x40 - 2, CABLE_13)))
        .Times(1);

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
        bank, encm, {0x20, CHANNEL_7, CABLE_13}, 2, 4};

    EXPECT_CALL(encm, read()).WillOnce(Return(4));
    EXPECT_CALL(
        midi, sendChannelMessageImpl(ChannelMessage(0xB6, 0x20, 2, CABLE_13)));

    ccenc.update();

    bank.select(1);

    EXPECT_CALL(encm, read()).WillOnce(Return(8));
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x20 + 4, 2, CABLE_13)));

    ccenc.update();
}

// -------------------------------------------------------------------------- //

#include <MIDI_Outputs/CCAbsoluteEncoder.hpp>

TEST(CCAbsoluteEncoder, simple) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();
    RelativeCCSender::setMode(relativeCCmode::TWOS_COMPLEMENT);

    EncoderMock encm;
    CCAbsoluteEncoder ccenc {encm, {0x20, CHANNEL_7, CABLE_13}, 3, 4};

    ccenc.setValue(0x40);
    EXPECT_CALL(encm, read()).WillOnce(Return(4));
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x20, 0x43, CABLE_13)));

    ccenc.update();

    EXPECT_EQ(ccenc.getValue(), 0x43);
}