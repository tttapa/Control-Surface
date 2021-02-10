#include <MIDI_Outputs/NoteButtonMatrix.hpp>
#include <MockMIDI_Interface.hpp>
#include <gmock/gmock.h>

using namespace ::testing;
using namespace CS;

TEST(NoteButtonMatrix, pressAndRelease) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();

    constexpr Channel channel = CHANNEL_7;
    constexpr Cable cn = CABLE_13;
    AddressMatrix<3, 2> addresses = {{{1, 3}, {5, 7}, {9, 11}}};
    NoteButtonMatrix<3, 2> matrix = {
        {2, 3, 4}, {5, 6}, addresses, {channel, cn}};

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, INPUT));

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(5, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(6, INPUT_PULLUP));

    matrix.begin();

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, INPUT));

    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(5))
        .Times(3)
        .WillOnce(Return(HIGH))
        .WillOnce(Return(LOW))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(6))
        .Times(3)
        .WillOnce(Return(HIGH))
        .WillOnce(Return(HIGH))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1000));

    EXPECT_CALL(
        midi, sendChannelMessageImpl(ChannelMessage(0x96, 5, 0x7F, CABLE_13)));

    matrix.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, INPUT));

    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(5))
        .Times(3)
        .WillOnce(Return(HIGH))
        .WillOnce(Return(HIGH))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(6))
        .Times(3)
        .WillOnce(Return(HIGH))
        .WillOnce(Return(HIGH))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(2000));

    EXPECT_CALL(
        midi, sendChannelMessageImpl(ChannelMessage(0x86, 5, 0x7F, CABLE_13)));

    matrix.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

// -------------------------------------------------------------------------- //

#include <MIDI_Outputs/Bankable/NoteButtonMatrix.hpp>

TEST(NoteButtonMatrixBankable, pressChangeSettingRelease) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();

    OutputBank bank(4);

    constexpr Channel channel = CHANNEL_7;
    constexpr Cable cn = CABLE_13;
    AddressMatrix<3, 2> addresses = {{{1, 3}, {5, 7}, {9, 11}}};

    Bankable::NoteButtonMatrix<3, 2> matrix = {
        bank, {2, 3, 4}, {5, 6}, addresses, {channel, cn}};

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, INPUT));

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(5, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(6, INPUT_PULLUP));

    matrix.begin();

    // Press

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, INPUT));

    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(5))
        .Times(3)
        .WillOnce(Return(HIGH))
        .WillOnce(Return(LOW))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(6))
        .Times(3)
        .WillOnce(Return(HIGH))
        .WillOnce(Return(HIGH))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1000));

    EXPECT_CALL(
        midi, sendChannelMessageImpl(ChannelMessage(0x96, 5, 0x7F, CABLE_13)));

    matrix.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
    Mock::VerifyAndClear(&midi);

    // Change bank setting

    bank.select(1);

    // Release

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, INPUT));

    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(5))
        .Times(3)
        .WillOnce(Return(HIGH))
        .WillOnce(Return(HIGH))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(6))
        .Times(3)
        .WillOnce(Return(HIGH))
        .WillOnce(Return(HIGH))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(2000));

    EXPECT_CALL(
        midi, sendChannelMessageImpl(ChannelMessage(0x86, 5, 0x7F, CABLE_13)));

    matrix.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
    Mock::VerifyAndClear(&midi);

    // Press again (with new setting)

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, INPUT));

    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(5))
        .Times(3)
        .WillOnce(Return(HIGH))
        .WillOnce(Return(LOW))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(6))
        .Times(3)
        .WillOnce(Return(HIGH))
        .WillOnce(Return(HIGH))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(3000));

    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0x96, 5 + 4, 0x7F, CABLE_13)));

    matrix.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
    Mock::VerifyAndClear(&midi);

    // Release again (with new setting)

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, INPUT));

    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(5))
        .Times(3)
        .WillOnce(Return(HIGH))
        .WillOnce(Return(HIGH))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(6))
        .Times(3)
        .WillOnce(Return(HIGH))
        .WillOnce(Return(HIGH))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(4000));

    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0x86, 5 + 4, 0x7F, CABLE_13)));

    matrix.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

// -------------------------------------------------------------------------- //

#include <MIDI_Outputs/ManyAddresses/NoteButtonMatrix.hpp>

TEST(NoteButtonMatrixManyAddresses, pressChangeSettingRelease) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();

    Bank<2> bank(4);

    Array<MIDIChannelCable, 2> channels = {{
        {CHANNEL_7, CABLE_13},
        {CHANNEL_2, CABLE_10},
    }};
    AddressMatrix<3, 2> addresses1 = {{{1, 3}, {5, 7}, {9, 11}}};
    AddressMatrix<3, 2> addresses2 = {{{10, 30}, {50, 70}, {90, 110}}};

    Bankable::ManyAddresses::NoteButtonMatrix<2, 3, 2> matrix = {
        bank, {2, 3, 4}, {5, 6}, {addresses1, addresses2}, channels};

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, INPUT));

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(5, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(6, INPUT_PULLUP));

    matrix.begin();

    // Press

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, INPUT));

    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(5))
        .Times(3)
        .WillOnce(Return(HIGH))
        .WillOnce(Return(LOW))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(6))
        .Times(3)
        .WillOnce(Return(HIGH))
        .WillOnce(Return(HIGH))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1000));

    EXPECT_CALL(
        midi, sendChannelMessageImpl(ChannelMessage(0x96, 5, 0x7F, CABLE_13)));

    matrix.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
    Mock::VerifyAndClear(&midi);

    // Change bank setting

    bank.select(1);

    // Release

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, INPUT));

    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(5))
        .Times(3)
        .WillOnce(Return(HIGH))
        .WillOnce(Return(HIGH))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(6))
        .Times(3)
        .WillOnce(Return(HIGH))
        .WillOnce(Return(HIGH))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(2000));

    EXPECT_CALL(
        midi, sendChannelMessageImpl(ChannelMessage(0x86, 5, 0x7F, CABLE_13)));

    matrix.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
    Mock::VerifyAndClear(&midi);

    // Press again (with new setting)

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, INPUT));

    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(5))
        .Times(3)
        .WillOnce(Return(HIGH))
        .WillOnce(Return(LOW))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(6))
        .Times(3)
        .WillOnce(Return(HIGH))
        .WillOnce(Return(HIGH))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(3000));

    EXPECT_CALL(
        midi, sendChannelMessageImpl(ChannelMessage(0x91, 50, 0x7F, CABLE_10)));

    matrix.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
    Mock::VerifyAndClear(&midi);

    // Release again (with new setting)

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, INPUT));

    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(5))
        .Times(3)
        .WillOnce(Return(HIGH))
        .WillOnce(Return(HIGH))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(6))
        .Times(3)
        .WillOnce(Return(HIGH))
        .WillOnce(Return(HIGH))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(4000));

    EXPECT_CALL(
        midi, sendChannelMessageImpl(ChannelMessage(0x81, 50, 0x7F, CABLE_10)));

    matrix.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}