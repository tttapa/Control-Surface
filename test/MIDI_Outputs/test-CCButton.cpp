#include <MIDI_Outputs/Bankable/CCButton.hpp>
#include <MIDI_Outputs/Bankable/CCButtons.hpp>
#include <MIDI_Outputs/CCButton.hpp>
#include <MIDI_Outputs/CCButtons.hpp>
#include <MockMIDI_Interface.hpp>
#include <gmock/gmock.h>

using namespace ::testing;
using namespace CS;

TEST(CCButton, pressAndRelease) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();

    CCButton button(2, {0x3C, CHANNEL_7, CABLE_13});
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    button.begin();

    // Still released
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1000));
    button.update();

    // Pressing
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(2000));
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x3C, 0x7F, CABLE_13)));
    button.update();

    // Still pressed
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(3000));
    button.update();

    // Releasing
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(4000));
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x3C, 0x00, CABLE_13)));
    button.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(CCButtonBankable, pressAndRelease) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();

    OutputBank bank(4);

    Bankable::CCButton button(bank, 2, {0x3C, CHANNEL_7, CABLE_13});
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    button.begin();

    // Still released
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1000));
    button.update();

    // Pressing
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(2000));
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x3C, 0x7F, CABLE_13)));
    button.update();

    // Still pressed
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(3000));
    button.update();

    // Releasing
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(4000));
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x3C, 0x00, CABLE_13)));
    button.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(CCButtonBankable, changeSettingAndPressAndRelease) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();

    OutputBank bank(4);

    Bankable::CCButton button(bank, 2, {0x3C, CHANNEL_7, CABLE_13});
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    button.begin();

    // Change bank setting
    bank.select(1);

    // Still released
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1000));
    button.update();

    // Pressing
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(2000));
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x3C + 4, 0x7F, CABLE_13)));
    button.update();

    // Still pressed
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(3000));
    button.update();

    // Releasing
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(4000));
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x3C + 4, 0x00, CABLE_13)));
    button.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(CCButtonBankable, pressAndChangeSettingAndRelease) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();

    OutputBank bank(4);

    Bankable::CCButton button(bank, 2, {0x3C, CHANNEL_7, CABLE_13});
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    button.begin();

    // Still released
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1000));
    button.update();

    // Pressing
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(2000));
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x3C, 0x7F, CABLE_13)));
    button.update();

    // Change bank setting
    bank.select(1);

    // Still pressed
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(3000));
    button.update();

    // Releasing
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(4000));
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x3C, 0x00, CABLE_13)));
    button.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

// -------------------------------------------------------------------------- //

TEST(CCButtons, pressAndRelease) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();

    CCButtons<2> buttons = {{2, 3}, {0x3C, CHANNEL_7, CABLE_13}, {1, 2}};
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT_PULLUP));
    buttons.begin();

    // 2 & 3 still released
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(1000));
    buttons.update();

    // Pressing 2 & 3 still released
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(2000));
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x3C, 0x7F, CABLE_13)));
    buttons.update();

    // 2 still pressed & pressing 3
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(3000));
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6 + 2, 0x3C + 1, 0x7F, CABLE_13)));
    buttons.update();

    // 2 still pressed & 3 still pressed
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(4000));
    buttons.update();

    // Releasing 2 & 3 still pressed
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(5000));
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x3C, 0x00, CABLE_13)));
    buttons.update();

    // 2 still released & releasing 3
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(6000));
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6 + 2, 0x3C + 1, 0x00, CABLE_13)));
    buttons.update();

    // 2 still released & 3 still released
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(7000));
    buttons.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

// -------------------------------------------------------------------------- //

TEST(CCButtonsBankable, pressChangeBankAndRelease) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();

    OutputBank bank(4);

    Bankable::CCButtons<2> buttons = {
        bank, {2, 3}, {0x3C, CHANNEL_7, CABLE_13}, {1, 2}};
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT_PULLUP));
    buttons.begin();

    // 2 & 3 still released
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(1000));
    buttons.update();

    // Pressing 2 & 3 still released
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(2000));
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x3C, 0x7F, CABLE_13)));
    buttons.update();

    bank.select(1);

    // 2 still pressed & pressing 3
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(3000));
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6 + 2, 0x3C + 1, 0x7F, CABLE_13)));
    buttons.update();

    // 2 still pressed & 3 still pressed
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(4000));
    buttons.update();

    // Releasing 2 & 3 still pressed
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(5000));
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x3C, 0x00, CABLE_13)));
    buttons.update();

    // 2 still released & releasing 3
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(6000));
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6 + 2, 0x3C + 1, 0x00, CABLE_13)));
    buttons.update();

    // 2 still released & 3 still released
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(7000));
    buttons.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(CCButtonsBankable, pressChangeBankAndReleaseAndPress) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();

    OutputBank bank(4);

    Bankable::CCButtons<2> buttons = {
        bank, {2, 3}, {0x3C, CHANNEL_7, CABLE_13}, {1, 2}};
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT_PULLUP));
    buttons.begin();

    // 2 & 3 still released
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(1000));
    buttons.update();

    // Pressing 2 & 3 still released
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(2000));
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x3C, 0x7F, CABLE_13)));
    buttons.update();

    bank.select(1);

    // 2 still pressed & pressing 3
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(3000));
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6 + 2, 0x3C + 1, 0x7F, CABLE_13)));
    buttons.update();

    // 2 still pressed & 3 still pressed
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(4000));
    buttons.update();

    // Releasing 2 & 3 still pressed
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(5000));
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6, 0x3C, 0x00, CABLE_13)));
    buttons.update();

    // 2 still released & releasing 3
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(6000));
    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xB6 + 2, 0x3C + 1, 0x00, CABLE_13)));
    buttons.update();

    // 2 still released & 3 still released
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(7000));
    buttons.update();

    // 2 still released & pressing 3
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(8000));
    EXPECT_CALL(midi, sendChannelMessageImpl(ChannelMessage(
                          0xB6 + 2, 0x3C + 1 + 4, 0x7F, CABLE_13)));
    buttons.update();

    // 2 still released & releasing 3
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(9000));
    EXPECT_CALL(midi, sendChannelMessageImpl(ChannelMessage(
                          0xB6 + 2, 0x3C + 1 + 4, 0x00, CABLE_13)));
    buttons.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}